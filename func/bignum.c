#include "bignum.h"

/*
    MEMORY MOVEMENT
*/
void dnum_split(const uint32_t *a, uint32_t *hi, uint32_t *lo){
    size_t i;
    if(hi){
        for(i = N_SZ*2 - 1; i >= N_SZ; --i){
            hi[i - N_SZ] = a[i];
        }
        widen(hi);
    }
    if(lo){
         for(i = N_SZ-1; i >= 0; --i){
            lo[i] = a[i];
        }
        widen(lo);
    }
}

/*
    ARITHMETIC PRIMITIVES
    
    the carry calculation is taken from BearSSL's br_i32_add.
*/
uint32_t add(uint32_t *a, const uint32_t *b){
    uint32_t c;
    size_t i, k;
    
    k = wordsize(a);
    c = 0;
    for (i = 0; i < k; ++i){
        uint32_t n = a[i] + b[i] + c; 
        c = (c & (a[i] == n)) | (n < a[i]);
        a[i] = n;
    }
    return c;
}

uint32_t addw(uint32_t *a, const uint32_t b){
    uint32_t c;
    size_t i, k;
    
    k = wordsize(a);
    uint32_t n = a[0] + b; 
    c = (n < a[0]);
    a[0] = n;

    for (i = 1; i < k; ++i){
        n = a[i] + c; 
        c = (c & (a[i] == n)) | (n < a[i]);
        a[i] = n;
    }
    return c;
}

uint32_t sub(uint32_t *a, const uint32_t *b){
    uint32_t c;
    size_t i, k;

    c = 0;
    k = wordsize(a);
    for(i = 0; i < k; ++i){
        uint32_t n = a[i] - b[i] - c;
        c = (c & (a[i] == n)) | (n > a[i]);
        a[i] = c;
   }
   return c;
}

uint32_t subw(uint32_t *a, const uint32_t b){
    uint32_t c;
    size_t i, k;
    
    k = wordsize(a);
    uint32_t n = a[0] - b; 
    c = (n < a[0]);
    a[0] = n;

    for (i = 1; i < k; ++i){
        n = a[i] - c; 
        c = (c & (a[i] == n)) | (n > a[i]);
        a[i] = n;
    }
    return c;
}

/*
    bitwise long division
*/
void long_div(const uint32_t *a, const uint32_t *b, 
    uint32_t *pq, uint32_t *pr){
    
    bignum(q);
    bignum(r);
    size_t i, k;
    
    k = bitsize(a);
    r[N_SZINFO] = 1;
    q[N_SZINFO] = k;
    for(i = k; i >= 0; --i){
        twice(r);
        set(0, sel(i, a), r);
        if(gte(r, b)){
            sub(r, b);
            set(i, 1, q);
        }
    }
    if(pq)
        copy(q, pq);
    if(pr)
        copy(r, pr);
}

/*
    bitwise long multiplication
*/
void long_mul(const uint32_t *a, const uint32_t *b, 
    uint32_t *hi, uint32_t *lo){
    
    uint32_t z[N_SZ*2] = {0};
    uint32_t c;
    size_t i, j, k, l;
    
    k = wordsize(a);
    l = wordsize(b);

    for(i = 0; i < k; ++i){
        c = 0;
        for(j = 0; j < l; j++){
            uint64_t n = z[i+j] + c + (uint64_t)a[i] * (uint64_t)b[j]; 
            z[i+j] = n & W_MASK;
            c = (n >> W_SZ) & W_MASK;
        }
        z[i+l] = c;
    }
    dnum_split(z, hi, lo);
}

/*
    MODULAR OPERATIONS
*/
void dnum_redc(const uint32_t *dn, const uint32_t *m,
    uint32_t *r){
        
        bignum(z);
        size_t i;

        widen(z);
        for(i = N_SZ*2; i >= 0; --i){
            twice(z);
            z[0] |= sel(i, m);
            if(gte(z, m)){
                sub(z, m);
            }
        }
        copy(z,r);
}

/*
    an optimized inversion for the special case where
    (1) n is a power of 2 (of the form (100....0000)b)
    (2) r is odd

    returns n_inv, r_inv s.t.
    n(n_inv) + r(r_inv) = 1

    taken from Henry Warren's simplified version of the
    Stein GCD algorithm, from his note, "Montgomery Multiplication"
*/
void bo_invert(const uint32_t  *n, const uint32_t *r, 
    uint32_t *n_inv, uint32_t *r_inv){
    
    bignum(u);
    bignum(v);
    bignum(alph);
    bignum(beta);
    addw(u, 1);
    copy(n, alph);
    copy(r, beta);
    
    while(!(zerop(alph))){
        half(alph);
        if(evenp(u)){
            half(u);
            half(v);
        } 
        else{
            widen(u);
            widen(v);

            uint32_t c = add(u, beta);
            half(u);
            set(N_BITS-1, c, u);

            half(v);
            add(v, alph);
        }
    }

    if(n_inv){
        copy(u, n_inv);
    }
    if(r_inv){
        copy(v, r_inv);
    }
}

void add_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r){
    bignum(z);
    copy(a,z);
    add(z, b);
    if(gte(z,m)) 
        sub(z, m);
    copy(z,r);
}

/*
    assuming a,b in [0, m), the lowest possible
    value for (a-b) is (-m)... so we just translate
    everything up by m, and then possibly a final
    reduce
*/
void sub_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r){
    uint32_t c;
    bignum(z);

    copy(a,z);
    c = sub(z, b);
    if(c){
        add(z, m);
    }
    copy(z, r);
}

/*
    slow non-Montgomery method
*/
void long_mul_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r){
    
    uint32_t z[N_SZ*2] = {0};
    size_t i, j, k, l;
    
    k = wordsize(a);
    l = wordsize(b);

    for(i = 0; i < k; ++i){
        uint32_t c = 0;
        for(j = 0; j < l; j++){
            uint64_t n = z[i+j] + c + (uint64_t)a[i] * (uint64_t)b[j]; 
            z[i+j] = n & W_MASK;
            c = (n >> W_SZ) & W_MASK;
        }
        z[i+l] = c;
    }
   
    dnum_redc(z, m, r);
}

uint32_t lsl(uint32_t *a, const size_t n){
    size_t i, k;
    uint32_t c;

    k = N_SZ - 1;
    c = 0;
    for(i = 0; i < k; ++i){
        uint32_t z = (a[i] << n) | c;
        c = a[i] >> (W_SZ - n);
        a[i] = z;
    }

    return c;
};

uint32_t lsr(uint32_t *a, const size_t n){
    size_t i;
    uint32_t c;

    c = 0;
    for(i = N_SZ - 1; i >= 0; --i){
        uint32_t z = c | (a[i] >> n);
        c = a[i] << (W_SZ - n);
        a[i] = z;
    }
    
    return c;
};

uint32_t zerop(const uint32_t *a){
    size_t i;

    /*
        we want to check the N_GUARD as well, so we go up to N_SZ
    */
    for (i = 0; i <= N_SZ; i++){
        if(a[i] != 0) return 0;
    }
    return 1;
}

size_t highbit(const uint32_t *a){ 
    size_t i, j;
    
    for(i = N_SZ - 1; i >= 0; --i){
        if(a[i] == 0){
            continue;
        }
        for(j = W_SZ - 1; j >= 0; --j) {
            if((a[i] >> j) & 1){
                return (i * W_SZ) + j;
            }
        }
    }
    return 0;
}

uint32_t gte(const uint32_t *a, const uint32_t *b){
    size_t i;
    
    for(i = N_SZ - 1; i >= 0; --i){
        if(a[i] == b[i]){
            continue;
        }
        return (a[i] > b[i]);
    }
    return 1;
}

uint32_t equ(const uint32_t *a, const uint32_t *b){
    size_t i;

    for(i = N_SZ - 1; i >= 0; --i){
        if (a[i] != b[i]){
            return 0;
        }
    }
    return 1;
}

uint32_t half(uint32_t *a){
    return lsr(a, 1);
}

uint32_t twice(uint32_t *a){
    return lsl(a, 1);
}

void print_num(const uint32_t *a){
    size_t i;

    if(N_SZ > 2) {
        i = highbit(a) / W_SZ;
    }
    else {
        i = N_SZ - 1;
    }
    for(; i >= 0; --i){
        printf("%#10.8x ", a[i]);
        if(!(i % 8))
            printf("\n");
    }
}
