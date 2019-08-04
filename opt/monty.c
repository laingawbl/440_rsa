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
    uint32_t z[N_SZ*2] = {0};
    
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

uint32_t dadd(uint32_t *a, const uint32_t * b){
    int k;
    uint32_t c = 0;

    for(k = 0; k < N_SZ*2; ++ k) {
        uint32_t n = a[k] + b[k] + c;
        c = (c & (n == a[k])) | (n < a[k]);
        a[k] = n;
    }
    return c;
}

uint32_t dsub(uint32_t *a, const uint32_t * b){
    int k;
    uint32_t c = 0;

    for(k = 0; k < N_SZ*2; ++ k) {
        uint32_t n = a[k] - b[k] - c;
        c = (c & (n == a[k])) | (n > a[k]);
        a[k] = n;
    }
    return c;
}

void loop_reduce(uint32_t *a, int c, const uint32_t *b){
    int i;
    uint32_t is_gte;
    do {
        is_gte = 1;
        if(c == 0){
            for(i = N_SZ-1; i >= 0; -- i){
                if (a[i] != b[i]){
                    is_gte = (a[i] > b[i]);
                    break;
                }
            }
        }
        if(is_gte){
            c -= sub(a, b);
        }
        assert(c >= 0);
    } while(is_gte);
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
        uint32_t hR[N_SZ*2] = {0};
        hR[N_SZ-1] = (1 << 31);
        uint32_t Ru[N_SZ*2] = {0};
        uint32_t Mv[N_SZ*2] = {0};
        full_mul(hR, u, Ru);
        full_mul(M, v, Mv);
        zero(hR);
        dadd(hR, Ru);
        dsub(hR, Mv);
        dadd(hR, Ru);
        assert(hR[0] == 1);
        assert(hR[N_SZ-1] == 0);
        assert(hR[N_SZ*2-1] == 0);
    /*
        upon exiting, r=0, and thus, 2^r = 1 = gcd(R,M) (if called properly)
    */
    zero(mm->modulus);
    zero(mm->m_inv);
    zero(mm->r_inv);

    move(M, mm->modulus);
    sub(mm->r_inv, u);
    sub(mm->m_inv, v);
}

/*
    obtain the residue of aR mod b, where 
    (1) b > N_SZ/2 (i.e., the highmost bit of b is set)
    (2) a < b (in practice, the highmost short of each symbol is set to 0x0001)
    by the following method:
    (1) estimate q = floor(a/b) using the top bytes of a,b
        (i) if (high part of aa) == bb, a/b is in 2^32 + [-1 .. 0).
        (ii)otherwise, (a/b) ~= floor(aa/B). 
    (2) shift a <<= 2^32
    (3) calculate a - (min(approxQ-3, 0))*b. now, a' = r + {0, b, 2b}
    (4) apply a conditional final addition / subtraction of b
*/
void mm_conv(const uint32_t *a, const uint32_t *b, uint32_t *R){

    int k;
    uint32_t bb;
    uint32_t A[N_SZ*2] = {0};
    move(a, A);
    bb = b[N_SZ-1];

    assert(bb & (1 << 31));
    
    /*
        this loop has the invariants
        (1) A = a * 2^(32k) mod N
    */
    
    for(k = 0; k < N_SZ; ++ k){
        int i;
        uint32_t Q, ha;
        uint64_t aa, c;
 
        /*
            take the upper two words of a, and call them aa, a truncation
            of a to between 33 and 64 bits of precision.
        */ 
        ha = A[N_SZ-1];

        if(ha == bb){
            Q = 0xFFFFFFFF;
        }
        else {
            aa = ((uint64_t)ha << 32) + A[N_SZ-2];
            Q = (uint32_t) (aa / bb);
            if(Q > 2){
                Q -= 3;
            }
            else{
                Q = 0;
            }
        }
        printf("I think Q= %lu / %u = %u (%#10.8x)\n", aa, bb, Q, Q);

        memmove(A+1, A, (N_SZ)*sizeof *A);
        A[0] = 0;
        c = 0;

        pf8("A", A);
        pf4("b", b);
        if(Q != 0){
            for(i = 0; i <= N_SZ; ++ i){
                uint64_t p = ((uint64_t)Q * (uint64_t)b[i]) + c;
                uint32_t lp = (uint32_t) p;
                c = (uint32_t)(p >> 32);
                A[i] -= lp;
                c += (uint64_t)(A[i] > lp);
            }
        }
        pf8("A'", A);

        int d = ha - c;
        printf("%lu %u\n", c, ha);
        assert(d >= 0);
        /*
            now we only need to call loop_reduce a small number of times.
        */
        loop_reduce(A, d, b);
    }

    memset(R, 0, N_SZ*2*sizeof *R);
    move(A, R);
}

/*
    scale down a Montgomery number from a product t \in [0 .. NR]
*/
void mm_redc(const uint32_t *A, const mm_system mm, uint32_t *r){
    uint32_t B[N_SZ*2] = {0};
    uint32_t Z[N_SZ*2] = {0};

    /*
        B = ((A mod R)m') mod R
    */
    move(A, B);
    half_mul(B, mm.m_inv);
    pf4("iM", mm.m_inv);
    pf4("B", B);
    pf4("*M", mm.modulus);
    /*
        t = (A + BN) mod R
    */
    full_mul(B, mm.modulus, Z);
    pf8("Z", Z);
    dadd(Z, A);
    pf8("+A", A);
    pf8("Z'", Z);
    move(Z+(N_SZ*sizeof *Z), Z);

    loop_reduce(Z, 0, mm.modulus);
    move(Z, r);
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
        cl = sub(Z, mm.modulus);
    }
    assert(cl == 0);
    assert(Z[N_SZ] == 0);

    memset(A, 0, N_SZ*2*sizeof *A);
    move(Z+(N_SZ*sizeof *Z), A);
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
    
    mod[15]= 0xcf6d59c9;
    mod[14]= 0xb1124b9c;
    mod[13]= 0x980b3bdb;
    mod[12]= 0xc63c7746;
    mod[11]= 0x84467c12;
    mod[10]= 0x1e385ab8;
    mod[9] = 0xf81f2395;
    mod[8] = 0x525db17a;
		mod[7] = 0xde78053c;
		mod[6] = 0x410e682b;
		mod[5] = 0xa7e50e34;
		mod[4] = 0x1664298f; 
		mod[3] = 0xbbd94152;
		mod[2] = 0x07ef9494;
		mod[1] = 0x783f4d7e;
		mod[0] = 0xfe955f61;
    
    init(mod, &Mont);
    printf("inverted:\n");
    pf4("mod", Mont.modulus);
    pf4("(im)", Mont.m_inv);
    pf4("-(ir)", Mont.r_inv);
    
    uint32_t val[N_SZ] = {0};
    val[0] = 1;
    uint32_t val2[N_SZ*2] = {0};
    
    mm_conv(val, Mont.modulus, val2);

    pf8("R%N", val2);
    // 1 -> 1R mod N = R mod N

    full_mul(val2, Mont.r_inv, val2);
    pf8("R(iR)", val2);
    /*for(i = 0; i < 16; i++){
        printf("converting %lu\n", i);
        zero(val);
        val[0] = (uint32_t) i + (1<<31);
        val[1] = (uint32_t) (i >> 32);
        
        pf4("v", val);
        
        mm_conv(val, Mont.modulus, val2);
        pf8("V", val2);

        zero(val);

        mm_redc(val2, Mont, val);
        pf4("v'", val);
    }*/
    mm_conv(mod, Mont.modulus, val2);
    pf8("M", val2);
    mm_conv(Mont.m_inv, Mont.modulus, val2);
    pf8("iM", val2);
    mm_conv(Mont.r_inv, Mont.modulus, val2);
    pf8("-iR", val2);
    zero(val2);
    full_mul(Mont.m_inv, Mont.modulus, val2);
    pf4("M*iM", val2);
}
