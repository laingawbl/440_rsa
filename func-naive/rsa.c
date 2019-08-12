#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

/*
    returns r s.t.
    
    a*r = 1 (mod b)

    by way of Bezout's identity

    [ au + bv = 1 ] < = > [au = 1 (mod b)
*/
Word euclid_inv(Word *a, Word *m, Word *r){
    
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
        qdiv(r0, r1, quo, tmp);
        
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
    
    return highbit(r0);
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
void spow_nn(Word *pb, Word *pe, Word *m, Word *r){
   
   Word e[N_SZ];
   Word b[N_SZ];
   Word z[N_SZ];
   copy(pb, b);
   copy(pe, e);
   load(z, 1);
   
   if(zerop(e)){
        copy(z, r);
        return;
   }

   while(highbit(e) > 0){
        if(oddp(e)){
            mul_mo(b, z, m, z);
            mul_mo(b, b, m, b);
            subws(e, 1);
            half(e);
        }
        else {
            mul_mo(b, b, m, b);
            half(e);
        }
   }
   mul_mo(b, z, m, r); 
}

/*
    fill words 0 through [max] of n with pseudorandom data from random()
*/
void load_random(Word max, Word *n){
    int i;
    zero(n);
    for(i = 0; i < max; i++){
        n[i] = random() & W_MASK;
    }
}

Word rabin_test(Word k, Word *n){
    Word max = PRIME_BITS / W_SZ;

    Word a[N_SZ];
    Word x[N_SZ];
    Word d[N_SZ];
    zero(d);
    zero(x);
    zero(a);
    Word r = 0;
    Word wits[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41
    };
    int i, j;

    subw(n, 1, d);
    while(evenp(d)){
        half(d);
        r += 1;
    }
   // now we have n = (2^r * d) + 1
    for(i = 0; i < k; i++){

        do {
            load_random(max, a);
            addws(a, 4);
        } while(gte(a, n));
        subws(a, 2);
        
        spow_nn(a, d, n, x);
         
        // if x = 1, continue
        if((highbit(x) == 0) && x[0] == 1){
           continue;
        }

        // if x = n-1 (represented by now unused a), continue
        subw(n, 1, a);
        if(equ(x,a)){ 
           continue;
        }
        
        for(j = 0; j < r; j++){
            mul_mo(x, x, n, x);
            if(equ(x,a)){ 
                break;
            }
        }
        if(j == r) 
            return 0;
    }
    return 1;
}

/*
    perform k-round miller-rabin tests on randomly picked numbers between
    2^(b/2) and 2^(b-1), trying to find a probable prime. the chance of
    mistakenly returning a composite number is no greater than 4^(-k).

    I think (??) 32 rounds should be sufficient, but to be really sure
    the upper bound becomes much smaller when k >= b/4 (so in the case of
    a 4096-bit key, b=2048, and so set k >= 512)
*/
void load_miller_rabin(Word k, Word *p){
    Word max = PRIME_BITS / W_SZ;
    /*
        TODO: replace this if i figure out where the urandom is on arm32
    */
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    srandom(t.tv_sec ^ t.tv_nsec);
    int i;
    Word n[N_SZ];
    load_random(max, n);
    set(0, 1, n); //enforce that n is odd
    set(PRIME_BITS-1, 1, n); //enforce that n > 2^b-1
    for(i=0; i < 4*N_BITS; i++){             
        if(rabin_test(k, n)){
            copy(n, p);
            return;
        }
        addws(n, max); //max ~= ln(2^b)
    }
    die("tried 4*N_BITS numbers with no luck!\n");
}

/* 
    k must be valid, i.e.
        k->n = pq, p,q in P-{2}
        k->e in [3, n-1] in Z, s.t. 
            gcd(e, \lambda(p)) === gcd(e, lcm(p-1, q-1)) = 1
*/
void rsaep(rsa_pub *k, Word *m, Word *c){
    if(gte(m, k->n)){
        die("message too large for this exponent!\n");
    }
    spow_nn(m, k->e, k->n, c);    
}

void rsadp(rsa_pri *k, Word *c, Word *m){ 
    if(gte(m, k->n)){
        die("cipher too large for this exponent!\n");
    }
    spow_nn(c, k->d, k->n, m);
}

void genkeys(Word exp, Word mr_rounds, rsa_pub *ke, rsa_pri *kd){
    
    Word a[N_SZ];
    Word b[N_SZ];
    Word e[N_SZ];
    Word d[N_SZ];
    Word n[N_SZ];

    zero(a);
    zero(b);
    load(e, exp);
    zero(d);
    zero(n);

    Word quot;
    do {
        // pick two PRIME_BITS-length primes through miller-rabin testing
        load_miller_rabin(mr_rounds, a);
        load_miller_rabin(mr_rounds, b);
        mul(a, b, n);
        
        // calculate phi(n) = (a-1)(b-1), overwriting secret b as we do so
        subws(a, 1);
        subws(b, 1);
        mul(a, b, b);
        zero(a);

        // [ (exp) is unit (mod phi(n)) ] <=> [ d = inv(exp), quot = 0 ].
        quot = euclid_inv(e, b, d);
        
        // also zero out secret phi(n)
        zero(b);
    } while (quot != 0);
   
    // now d is the only secret - (n, e) form the public key
    copy(d, kd->d);
    zero(d);
    
    copy(n, ke->n);
    copy(n, kd->n);

    copy(e, ke->e);
}

int rsa_enc(rsa_pub *ke, char *m, int len, char *c){
    if(len > SYM_SZ){
        printf("message too long\n");
        return 0;
    }
    
    Word mr[N_SZ];
    Word cr[N_SZ];
    
    zero(mr);
    zero(cr);

    os2ip(m, len, mr);
    
    rsaep(ke, mr, cr);
   
    i2osp(cr, SYM_SZ, c);

    return len;
}

int rsa_dec(rsa_pri *kd, char *c, char *m){ 
    
    Word mr[N_SZ];
    Word cr[N_SZ];
    
    zero(mr);
    zero(cr);

    os2ip(c, SYM_SZ, cr);

    rsadp(kd, cr, mr);

    int written = i2osp(mr, SYM_SZ, m);

    return written;
}
