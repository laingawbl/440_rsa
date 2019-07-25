#include "num.h"
#include <stdio.h>
#include <stdlib.h>

/*
    Functional, unoptimised implementation of the RSA encryption and
    decryption primitives (except for whatever simple improvements I couldn't
    help but make, absent gprof or assembly examination).

    this version uses the square-and-multiply method, including a lookup
    table for the window optimisation.

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
void stein_gcd(WORD *a, WORD *b, WORD *u, WORD *v){
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

    copy(u, p);
    copy(v, q);
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
    The same method, but with correction modulo m at each step.
    We shift the result rz to preserve the value of bz.
*/
void mul_mo(WORD *a, WORD *b, WORD *m, WORD *r){
    int i;
    WORD rz[SZ_NUM];
    zero(rz);

    WORD bz[SZ_NUM];
    copy(b, bz);
    
    for(i = SZ_NUM - 1; i >= 0; --i){
        
        lsl_b(rz, 1, rz);
        if(gte(rz, m)){
            sub_nn(rz, m, rz);
        }
        
        if(sel(i, a)){
            add_nn(rz, bz, rz);
        }
        if(gte(rz, m)){
            sub_nn(rz, m, rz);
        }
    }
    copy(rz, r);
}

/*
    note that

          { x * (x^2) ^ (e-1 / 2), if n is odd
    x^e = {
          { (x^2) ^ (e / 2),       if n is even

    which is simplified here to repeatedly carrying out
        
        e odd ?
            x_next *= x
            e -= 1            
        x *= x^2
        e /= 2
        
*/
void spow_nn(WORD *b, WORD *e, WORD *m, WORD *r){
   
   WORD z[SZ_NUM];
   z[0] = 1;

   int i = SZ_NUM - 1;

   if(zerop(e)){
        copy(z, r);
        return;
   }

   while(highbit(e) > 0){
        if(e[0] & 1){
            mul_mo(b, z, m, z);
            sub_nw(e, 1, e);
        }
        mul_mo(b, b, m, b);
        lsl_b(e, 1, e);
   }

   mul_mo(b, z, r);
}

/* 
    k must be valid, i.e.
        k->n = pq, p,q in P-{2}
        k->e in [3, n-1] in Z, s.t. 
            gcd(e, \lambda(p)) === gcd(e, lcm(p-1, q-1)) = 1
*/
void rsaep(rsa_pub *k, WORD *m, WORD *c){
    spow_nn(m, k->e, k->n, c);    
}

void rsadp(rsa_pri *k, WORD *c, WORD *m){ 
    spow_nn(c, k->d, k->n, m);
}
