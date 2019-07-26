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

void die(const char * what){
    printf("error: %s", what);
    exit(1);
}

WORD evenp(WORD *a){
    return (!(a[0] & 1));
}

WORD oddp(WORD *a){
    return a[0] & 1;
}

void half(WORD *a){
    lsr_b(a, 1, a);
}

void load(WORD *a, WORD val){
    zero(a);
    add_nw(a, val, a);
}

void print_num(WORD *a){
    int i = highbit(a) / 32;
    for(i; i >= 0; --i){
        printf("%#10.8x ", a[i]);
        if(!(i % 8))
            printf("\n");
    }
}

typedef struct _rsa_pub_s {
    WORD n[SZ_NUM];
    WORD e[SZ_NUM];
} rsa_pub;

typedef struct _rsa_pri_s {
    WORD n[SZ_NUM];
    WORD d[SZ_NUM];
} rsa_pri;

struct rsa_num {
    WORD n[SZ_NUM];
};

/*
    returns r s.t.
    
    a*r = 1 (mod m)

    by way of Bezout's identity

    [ au + bv = 1 ] < = > [au = 1 (mod b)]

    optimised for the case in which a,m are guaranteed coprime (i.e.,
    they are never both even).
*/
void stein_inv(WORD *a, WORD *m, WORD *r){
    if(zerop(a)) {
        copy(a, r);
        return;
    }
    if(zerop(m)) {
        copy(m, r);
        return;
    }
    if(evenp(a) && evenp(m)){
        die("improper call to stein_inv: a, m both even");
    }

    WORD u[SZ_NUM];
    WORD v[SZ_NUM];
    WORD A[SZ_NUM];
    WORD B[SZ_NUM];

    load(u, 1);
    zero(v);
    copy(a, A);
    copy(m, B);

    do{
        while(evenp(A)){
            half(A);
            if(evenp(v)){
                add_nn(v, B, v);
                half(v);
            }
        }
        while(evenp(B)){
            half(B);
            if(evenp(u)){
                add_nn(u, A, u);
                half(u);
            }
        }
        if(gte(A, B)){
            sub_nn(A, B, A);
            add_nn(v, u, v);
        }
        else{
            sub_nn(B, A, B);
            add_nn(u, v, u);
        }    
    } while (!(zerop(A)));
    
    printf("u:\n");
    print_num(u);
    printf("v:\n");
    print_num(v);
    copy(u, r);
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
    zero(rz);
    WORD bz[SZ_NUM];
    copy(b, bz);

    for(i = 0; i <= ha; i++){
        if(sel(i, a)){
            c |= add_nn(rz, bz, rz);
        }
        lsl_b(bz, 1, bz);
    }
    copy(rz, r);
    return c;
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

void redc(WORD *a, WORD *m, WORD *r){
    WORD az[SZ_NUM];
    copy(a, az);
    while(gte(az, m)){
        sub_nn(az, m, az);
    }
    copy(az, r);
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

   mul_mo(b, z, m, r);
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

int main(){
    WORD p[SZ_NUM];
    WORD q[SZ_NUM];
    WORD n[SZ_NUM];

    zero(p);
    zero(q);
    zero(n);

    p[0] = 103;
    q[0] = 107;
    
    // 102 = 2 * 3 * 17
    // 106 = 2 * 53
    // lcm(102, 106) = 5406

    mul_nn(p, q, n);

    print_num(n);

    rsa_pub k_1; 
    
    copy(n, k_1.n);
    zero(k_1.e);
    k_1.e[0] = 257;

    // gcd(2703, 257) = gcd(2*3*17*53, 257) = 1

    rsa_pri k_2;

    copy(n, k_2.n);
    stein_inv(k_1.e, n, k_2.d);

    print_num(k_1.e);
    print_num(k_2.d);

    return 0;
}
