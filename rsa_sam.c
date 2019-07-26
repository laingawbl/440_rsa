#include "bignum/bignum.h"
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

typedef struct _rsa_pub_s {
    Word n[N_SZ];
    Word e[N_SZ];
} rsa_pub;

typedef struct _rsa_pri_s {
    Word n[N_SZ];
    Word d[N_SZ];
} rsa_pri;

struct rsa_num {
    Word n[N_SZ];
};

/*
    basic long-division
*/
Word div_nn(Word *a, Word *b, Word *pq, Word *pr){
    if(zerop(b)) 
        die("div by zero");

    Word q[N_SZ];
    Word r[N_SZ];
    zero(q);
    load(r, 0);
    
    int i;
    Word ha = highbit(a);

    for(i = ha; i >= 0; --i){
        twice(r);
        set(0, sel(i, a), r);
        if(gte(r, b)){
            subs(r, b);
            set(i, 1, q);
        }
    }
    copy(q, pq);
    copy(r, pr);
}

/*
    Peasant Method multiplication
    does NO bounds checking - you will get a garbage result if
        highbit(a) + highbit(b) > N_BITS
    but this will be communicated by returning the overflow as 1.
*/
Word mul_nn(Word *a, Word *b, Word *r){
    Word ha = highbit(a);
    Word c = 0;

    int i;
    Word rz[N_SZ];
    zero(rz);
    Word bz[N_SZ];
    copy(b, bz);

    for(i = 0; i <= ha; i++){
        if(sel(i, a)){
            c |= adds(rz, bz);
        }
        twice(bz);
    }
    copy(rz, r);
    return c;
}

/*
    The same method, but with correction modulo m at each step.
    We shift the result rz to preserve the value of bz.
*/
void mul_mo(Word *a, Word *b, Word *m, Word *r){
    int i;
    Word rz[N_SZ];
    zero(rz);

    Word bz[N_SZ];
    copy(b, bz);
    
    for(i = N_BITS - 1; i >= 0; --i){ 
        twice(rz);
        if(gte(rz, m))
            subs(rz, m);
        if(sel(i, a))
            adds(rz, bz);
        if(gte(rz, m))
            subs(rz, m);
    }
    copy(rz, r);
}

void redc(Word *a, Word *m, Word *r){
    Word az[N_SZ];
    copy(a, az);
    while(gte(az, m))
        subs(az, m);
    copy(az, r);
}

/*
    returns r s.t.
    
    a*r = 1 (mod b)

    by way of Bezout's identity

    [ au + bv = 1 ] < = > [au = 1 (mod b)
*/
void euclid_inv(Word *a, Word *m, Word *r){
    
    Word t0[N_SZ];
    Word t1[N_SZ];
    Word r0[N_SZ];
    Word r1[N_SZ];
    Word quo[N_SZ];
    Word tmp[N_SZ];

    load(t0, 0);
    load(t1, 1);
    copy(m, r0);
    copy(a, r1);
    zero(quo);
    zero(tmp);
   
    int i = 0;
    while(!(zerop(r1)) && (i < 12)){
        printf("r0,r1:\n");
        print_num(r0);
        print_num(r1);
        printf("t0,t1:\n");
        print_num(t0);
        print_num(t1);

        div_nn(r0, r1, quo, tmp);

        printf("qu,re:\n");
        print_num(quo);
        print_num(tmp);

        //(t0, t1) <= (t1, t0 - q*t1)
        copy(t1, tmp);
        copy(t0, t1);
        copy(tmp, t0);
        mul_mo(tmp, quo, m, tmp);
        sub_mo(t1,  tmp, m, t1);

        //(r0, r1) <= (r1, r0 - q*r1)
        copy(r1, tmp);
        copy(r0, r1);
        copy(tmp, r0);
        mul_mo(tmp, quo, m, tmp);
        sub_mo(r1,  tmp, m, r1);

        i++;
    }
    copy(t0, r);
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
void spow_nn(Word *b, Word *e, Word *m, Word *r){
   
   Word z[N_SZ];
   z[0] = 1;

   int i = N_SZ - 1;

   if(zerop(e)){
        copy(z, r);
        return;
   }

   while(highbit(e) > 0){
        if(e[0] & 1){
            mul_mo(b, z, m, z);
            subws(e, 1);
        }
        mul_mo(b, b, m, b);
        twice(e);
   }

   mul_mo(b, z, m, r);
}

/* 
    k must be valid, i.e.
        k->n = pq, p,q in P-{2}
        k->e in [3, n-1] in Z, s.t. 
            gcd(e, \lambda(p)) === gcd(e, lcm(p-1, q-1)) = 1
*/
void rsaep(rsa_pub *k, Word *m, Word *c){
    spow_nn(m, k->e, k->n, c);    
}

void rsadp(rsa_pri *k, Word *c, Word *m){ 
    spow_nn(c, k->d, k->n, m);
}

int main(){
    Word p[N_SZ];
    Word q[N_SZ];
    Word n[N_SZ];

    load(p, 103);
    load(q, 107);
    zero(n);
    
    // 102 = 2 * 3 * 17
    // 106 = 2 * 53
    // lcm(102, 106) = 5406

    mul_nn(p, q, n);

    print_num(n);

    rsa_pub k_1; 
    
    copy(n, k_1.n);
    load(k_1.e, 257);

    // gcd(2703, 257) = gcd(2*3*17*53, 257) = 1

    rsa_pri k_2;

    copy(n, k_2.n);
    
    printf("inv start\n");

    euclid_inv(k_1.e, n, k_2.d);

    printf("inv done\n");

    print_num(k_1.e);
    print_num(k_2.d);

    //mul_mo(k_1.e, k_2.d, n, q);

    //print_num(q);

    return 0;
}
