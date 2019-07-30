#include "monty.h"
#include <assert.h>

/*
    unexported functions
*/
/*
    a half-width truncated multiplication. the principle is normally:
    (a * b) = (a3 a2 a1 a0 * b3 b2 b1 b0)

    =             a3a2 a1a0 * b0
        +      a3 a2a1 a0   * b1
        +    a3a2 a1a0      * b2
        + a3 a2a1 a0        * b3
        ---------------
        c7c6 c5c4 c3c2 c1c0

    but for numbers in Montgomery space, we can perform operations
    modulo the wordsize and toss the upper half of the result (and the
    corresponding half of the operations needed).

    (A * B) = (aa aa * bb bb)
    =     a3a2 a1a0 * b0
        + a2a1 a0   * b1
        + a1a0      * b2
        + a0        * b3
        ---------------
          c3c2 c1c0     (== c7...c0 (mod B))
*/
void half_mul(uint32_t *a, const uint32_t *b){
    
    int i, j;
    uint32_t c;
    uint32_t z[N_SZ];
    
    for(i = 0; i < N_SZ; i++){
        c = 0;
        for(j = 0; j < (N_SZ-i); j++){
            uint64_t p = z[i+j] + c + (uint64_t)b[i] * (uint64_t)a[j];
            uint32_t n = (uint32_t)p;
            c = (uint32_t)(p >> 32);
            z[i+j] = n;
        }
    }

    move(z,a);
}

/*
    as above, but we are restrained from doing anything fancy and must
    calculate the entire double-width product.
*/
void full_mul(const uint32_t *a, const uint32_t *b, uint32_t *r){
    
    int i, j;
    uint32_t c;
    uint32_t z[N_SZ*2];
    
    for(i = 0; i < N_SZ; i++){
        c = 0;
        for(j = 0; j < N_SZ; j++){
            uint64_t p = z[i+j] + c + (uint64_t)b[i] * (uint64_t)a[j];
            uint32_t n = (uint32_t)p;
            c = (uint32_t)(p >> 32);
            z[i+j] = n;
        }
        z[i+N_SZ] = c;
    }
    dmove(z, r);
}

uint32_t add(uint32_t *a, const uint32_t * b){
    int k;
    uint32_t c = 0;

    for(k = 0; k < N_SZ; ++ k) {
        uint32_t n = a[k] + b[k] + c;
        c = (c & (n == a[k])) | (n < a[k]);
        a[k] = n;
    }
    return c;
}

uint32_t sub(uint32_t *a, const uint32_t * b){
    int k;
    uint32_t c = 0;

    for(k = 0; k < N_SZ; ++ k) {
        uint32_t n = a[k] - b[k] - c;
        c = (c & (n == a[k])) | (n > a[k]);
        a[k] = n;
    }
    return c;
}

/*
    exported functions
*/
/*
    checked on godbolt

    find (m_inv, r_inv) such that m(m_inv) - r(r_inv) = 1.
    (one might expect m(m_inv) + r(r_inv) = 1, as in Bezout's lemma, but:
        m(m_inv) - r(r_inv)   = 1  <=>
        m(m_inv) + r(m-r_inv) = 1  ).
*/
void init(const uint32_t *M, mm_system *mm){ 
    
    size_t r;
    uint32_t u[N_SZ] = {0};
    uint32_t v[N_SZ] = {0}; 
    u[0] = 1;
    
    for(r = N_BITS; r > 0; -- r){
        uint32_t is_odd = (u[0] & 1); 
        uint32_t cu = 0;
        uint32_t cv = is_odd;
        size_t k;
        /*
            we have the invariants that
                (1) v is even (except maybe on exiting the loop);
                (2) 2^r = Ru - Mv.
            if u is odd, we set (u' = u + M, v' = v + R); M is an odd prime, 
            so u' is even. then
            u' = u+M / 2, v' = v+R / 2
            Ru' - Mv' = R(u+M / 2) - M(v+R / 2)
            Ru' - Mv' = Ru/2 + RM/2 - Mv/2 - RM/2
            2^r (1/2) = Ru/2 + Mv/2
            2^r       = 2(Ru' - Mv')
            the carry calculation is taken from BearSSL's br_i32_add.
        */
        if(is_odd){
            cu = add(u, M);
        }

        assert((u[0] & 1) == 0);
        assert((v[0] & 1) == 0);
        /*
            then, we may delete a power of two from both u, v. note that
            possible overflow from (u+M) has been stored in (cu). similarly,
            if odd, (R) has been stored in (cv).
        */        
        for(k = N_SZ; k > 0; -- k){
            uint32_t ncu = u[k-1] & 1;
            uint32_t ncv = v[k-1] & 1;
            u[k-1] = (cu << 31) | (u[k-1] >> 1);
            v[k-1] = (cv << 31) | (v[k-1] >> 1);
            cu = ncu;
            cv = ncv;
        }

        assert (cu == 0);
        assert (cv == 0);
    }
    /*
        upon exiting, r=0, and thus, 2^r = 1 = gcd(R,M) (if called properly)
    */
    zero(mm->modulus);
    zero(mm->m_inv);
    zero(mm->r_inv);

    move(M, mm->modulus);
    move(u, mm->m_inv);
    move(v, mm->r_inv);
}

/*
    obtain the residue of (a * 2^2048) mod b = (a||0)/b, where 
    (1) b > 2^2047 (i.e., the highmost bit of b is set)
    (2) a < b (in practice, the highmost short of each symbol is set to 0x0001,
        so 2^2032 <= a < 2^2033, and the "working" symbol size is 62 bytes)

    by the following method:
    (1) estimate q = floor(a/b) using the top bytes of a,b - a sort of 0-pass
        Newton-Raphson method
        (i) if (high part of aa) == bb, a/b is in 2^32 + [-1 .. 0)
        (ii)otherwise, (a/b) = floor(aa/B) + [0 .. 1), as
            - aa within (a - 2^-64 .. a]
            - B  within (b - 2^-32 .. b]
            - aa/B in (a - 2^-64 / b .. a/(b - 2^-32)]
                   in (a/b - (2^64 / 2^-64) .. a/b + (2^64 / 2^-64)]
                   in a/b - (1 .. 1]   
    (2) calculate a - (estimate-q)*b
*/
void mm_conv(const uint32_t *a, const uint32_t *b, uint32_t *R){

    int k;
    uint32_t bb;
    uint32_t A[N_SZ*2] = {0};
    move(a, A);
    bb = b[N_SZ-1];

    assert(bb & (1 << 31));
    for(k = 0; k < N_SZ; ++ k){
        int i;
        uint32_t Q, uf, ha;
        uint64_t aa, c;
         
        /*
            take the upper two words of a, and call them aa, a truncation
            of a to between 33 and 64 bits of precision.
        */ 
        ha = A[N_SZ-1];

        assert(ha < bb);

        if(ha == bb){
            Q = 0xFFFFFFFF;
        }
        else {
            aa = ((uint64_t)ha << 32) + A[N_SZ-2];
            Q = (uint32_t) (aa / bb);
        }
        /*
            shift over a by one word (the highmost word is saved in ha).
        */
        memmove(A+1, A, (N_SZ)*sizeof *A);
        A[0] = 0;
        c = 0;
        uf = 0;

        /*
            if Q != 0, subtract Q*b from a.
        */
        if(Q != 0){
            for(i = 0; i < N_SZ; ++ i){
                uint64_t p = ((uint64_t)Q * (uint64_t)b[i]) + c;
                uint32_t lp = (uint32_t) p;
                A[i] -= lp;
                c = (uint32_t)(p >> 32); 
                c += (uint64_t)(A[i] > lp);
                uf = (A[i] > lp) ? 1 : (A[i] == lp);
            }
        }
        
        /*
            either add or subtract extra copies of m.
        */ 
        while(uf || (c < (uint64_t)ha)){
            c += sub(A, b);
            uf = 0;
        }
        while (c > (uint64_t)ha){
            c -= add(A, b);
        }
        assert((c - ha) == 0);
    }
    zero(R+N_SZ);
    move(A, R);
}

void mm_redc(const uint32_t *A, const mm_system mm, uint32_t *r){
   
   int i, j;
   uint32_t c, ha;
   uint32_t b[N_SZ];

   for(i = 0; i < N_SZ; ++ i){
        c = 0;
        zero(b);
        b[0] = A[i];
        half_mul(b, mm.m_inv);
       /* 
        for(j = 0; j < N_SZ; j++){
            uint32_t x = A[i+j];
        }*/
   }
}

/*
    modified from Algorithm 14.36, A. J. Menzes, et. al., Handbook
    of Applied Cryptography
*/
void mm_mul(uint32_t *A, const uint32_t *B, const mm_system mm){
    int i;
    uint32_t c, cl;
    uint32_t Z[N_SZ*2] = {0};
    uint32_t Y[N_SZ*2] = {0};
    
    /* 
        Z = AB
        lz = Zm' (mod R)
        note - we move(Z,Y), not dmove, as we only need its bottom half
    */
    full_mul(A, B, Z);
    move(Z, Y);
    half_mul(Y, mm.m_inv);
    full_mul(Y, mm.modulus, Y);

    /* 
        Z = Z + ((Zm' mod R)m)
        observe that now Z = (abR mod m) R, i.e., the
        lower half will be zero after the following addition
    */
    c = 0;
    for(i = 0; i < N_SZ*2; ++ i){
        uint32_t p = Y[i] + c;
        Z[i] += p;
        c = (c && (Z[i] == p)) | (Z[i] < p);
    }

    /*
        we must check if Z >= m, but we can skip this step
        if c is nonzero; then we know Z >= m.
    */
    if(c == 0){
        for(i = N_SZ; i >= 0; -- i){
            uint32_t k = mm.modulus[i];
            if(Z[i] == k){
                continue;
            }
            else{
                c = (Z[i] > k);
                break;
            }
        }
    }

    if(c == 0){
        cl = 0;
        for(i = 0; i < N_SZ; ++ i){
            uint32_t k = mm.modulus[i] + cl;
            Z[i] -= k;
            cl = (cl && (Z[i] == k)) | (Z[i] > k);
        }
    }
    assert(cl == 0);
    assert(Z[N_SZ] == 0);

    zero(A+N_SZ);
    move(Z+N_SZ, A);
}

int main(void){
    mm_system Mont;
    uint32_t mod[N_SZ] = {0};
    /*
        note to self: 2^64 - a is prime for the following a:
        (by https://primes.utm.edu/lists/2small/0bit.html)
        { 59, 83, 95, 179, 189, 257, 279, 323, 353, 363 }
        2^128 - a:
        { 159, 173, 233, 237, 275, 357, 675, 713, 797, 1193 }
        2^256 - a:
        { 189, 357, 435, 587, 617, 923, 1053, 1299, 1539, 1883 }
    */
    mod[3] = -1;
    mod[2] = -1;
    mod[1] = -1;
    mod[0] = -1193;
    init(mod, &Mont);
    printf("inverted:\n");
    pf4("mod", Mont.modulus);
    pf4("(im)", Mont.m_inv);
    pf4("-(ir)", Mont.r_inv);
    
    uint32_t val[N_SZ] = {0};
    uint32_t val2[N_SZ*2] = {0};
    
    uint64_t i;
    for(i = (1L<<63)+1; i != 0; i+= (1<<30)+21){
        //printf("converting %d\n", i);
        zero(val);
        val[0] = (uint32_t) i;
        val[1] = (uint32_t) (i >> 32);
        //pf2("v", val);
        mm_conv(val, Mont.modulus, val2);
    }
    pf4("V", val2);
}
