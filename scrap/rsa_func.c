#include "num.h"
#include <stdio.h>
#include <stdlib.h>

/*
    Functional, unoptimised implementation of the RSA encryption and
    decryption primitives (except for whatever simple improvements I couldn't
    help but make, absent gprof or assembly examination)

    the full RSA cryptosystem, including RSAES-OAEP, is NOT implemented!
    this code should NEVER be used for cryptographic purposes.
*/

inline void die(const char * what){
    printf("error: %s", what);
    exit(1);
}

struct rsa_pub {
    WORD n[SZ_NUM];
    WORD e[SZ_NUM];
};

struct rsa_pri {
    WORD n[SZ_NUM];
    WORD d[SZ_NUM];
};

struct rsa_num {
    WORD n[SZ_NUM];
};

/*
    a = 2^n,    n in Z
    b = 2m + 1, m in Z
*/
void stein_gcd(WORD *a, WORD *b, WORD *ru, WORD *rv){
    WORD alpha[SZ_NUM];
    WORD beta[SZ_NUM];
    WORD u[SZ_NUM];
    WORD v[SZ_NUM];

    copy(a, alpha);
    copy(b, beta);
    zero(u);
    zero(v);

    while !(zerop(a)){
        lsr_b(a, 1, a);
        if(sel(1, u) == 0){
            lsr_b(u, 1, u);
            lsr_b(v, 1, v);
        }
        else {
            add_nn(u, beta, u);
            lsr_b(u, 1, u);
            lsr_b(v, 1, v);
            add_nn(v, alpha, v);
        }
    }

    copy(u, ru);
    copy(v, rv);
}

/*
    Peasant Method multiplication
    does NO bounds checking - you will get a garbage result if
        highbit(a) + highbit(b) > MOD_MAX_SZ
    but this will be communicated by returning the overflow as 1.
*/
WORD mul_nn(WORD *a, WORD *b, WORD *r){
    WORD ha = highbit(a);
    WORD c = 0;

    int i;
    WORD rz[SZ_NUM];
    WORD bz[SZ_NUM];
    copy(b, bz);

    for(i = 0; i <= ha; i++){
        if(sel(i, a)){
            c |= add_nn(rz, bz, rz);
        }
        lsl_b(bz, 1, bz);
    }
    copy(rz, r);
    return WORD;
}

/*
    modular multiplication by logical shifts
    adapted from the hardware division alg in Hacker's Delight, as suggested
    by Henry Warren in his note on Montgomery multiplication, for long nums

    this routine computes (hi || lo) / z, where hi, lo, z are all nums (so
    (hi || lo) is a double-width num, e.g. if MOD_MAX_SZ is 4096, then it's
    computing the division of an 8192-bit number by a 4096-bit one

    upon completion:
    - hi contains the residue (modulo z)
    - lo contains the quotient
*/
void mod_dnum(WORD *hi, WORD *lo, WORD *m){
    int i;
    WORD t;
    WORD u;

    for(i = 0; i < MOD_MAX_SZ; i++){
        // logical shift (hi || lo) left by one bit
        // equivalent (hi || lo) * 2 (mod 2^MOD_MAX_SZ)
        t = ((hi[SZ_NUM] >> W_SZ - 1) & 1);
        u = ((lo[SZ_NUM] >> W_SZ - 1) & 1);
        lsr_b(hi, 1, hi);
        lsr_b(lo, 1, lo);
        hi[0] &= u;
        
        hi[0] |= t;
        if(gte(hi, m)){
            set(0, u, hi);
            sub_nn(hi, z, hi);
            lo[0] &= 1;
        }
    }
}

/*
    Montgomery-multiply two Montgomery-space big nums together.
    here we have chosen the scale factor R = 2^MOD_MAX_SZ, so that
    (a.) it is impossible to choose a modulus m > R; and
    (b.) each num is of the form

        nR(mod m) = (n << MOD_MAX_SZ) (mod m)
        = (n || 0) (mod m)

    and so can be calculated from a regular big num n by

        mod_dnum(n, 0, m);

    so mod_dnum is also our conversion into Montgomery space.
    to convert back, note that
    
        mmul(n, 1) = ((nR)*1 / R) mod m
        = n mod m

    so mmul_nn is also our conversion out of Montgomery space.
    note however that in the general case, also

        mmul(n, R^2) = (nR)R / R mod m = nR mod m

    but this requires precomputation of (R^2 mod m), which we don't do.
*/
void mmul_nn(WORD *ma, WORD *mb, WORD *r){
    
}

/* 
    k must be valid, i.e.
        k->n = pq, p,q in P-{2}
        k->e in [3, n-1] in Z, s.t. 
            gcd(e, \lambda(p)) === gcd(e, lcm(p-1, q-1)) = 1
*/
void rsaep(rsa_pub *k, WORD *m, WORD *c){
    
}

void rsadp(rsa_pri *k, WORD *c, WORD *m){

}
