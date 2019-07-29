#include "rsa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    Functional, optimised implementation of the RSA encryption and
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
uint32_t euclid_inv(uint32_t *a, uint32_t *m, uint32_t *r){
    bignum(t0);
    bignum(t1);
    bignum(r0);
    bignum(r1);
    bignum(quo);
    bignum(tmp);
    
    addw(t1, 1);
    copy(m, r0);
    copy(a, r1);
    while(!(zerop(r1))){
        long_div(r0, r1, quo, tmp);
        
        //(t0, t1) <= (t1, t0 - q*t1)
        copy(t1, tmp);
        copy(t0, t1);
        copy(tmp, t0);
        long_mul_mod(tmp, quo, m, tmp);
        sub_mod(t1,  tmp, m, t1);

        //(r0, r1) <= (r1, r0 - q*r1)
        copy(r1, tmp);
        copy(r0, r1);
        copy(tmp, r0);
        long_mul_mod(tmp, quo, m, tmp);
        sub_mod(r1,  tmp, m, r1);
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
void spow_nn(uint32_t *pb, uint32_t *pe, uint32_t *m, uint32_t *r){
   
   bignum(e);
   bignum(b);
   bignum(z);
  
   addw(z, 1);
   if(zerop(pe)){
        copy(z, r);
        return;
   }
   
   copy(pb, b);
   copy(pe, e);
 
   while(highbit(e) > 0){
        if(oddp(e)){
            long_mul_mod(b, z, m, z);
            long_mul_mod(b, b, m, b);
            subw(e, 1);
            half(e);
        }
        else {
            long_mul_mod(b, b, m, b);
            half(e);
        }
   }
   long_mul_mod(b, z, m, r); 
}

/*
    fill words 0 through [max] of n with pseudorandom data from random()
*/
void load_random(uint32_t max, uint32_t *n){
    int i;
    zero(n);
    for(i = 0; i < max; i++){
        n[i] = random() & W_MASK;
    }
}

uint32_t rabin_test(uint32_t k, uint32_t *n){
    size_t i, j, l;
    bignum(a);
    bignum(d);
    bignum(x);
    uint32_t r = 0;
    
    l = wordsize(n);
    copy(n, d);
    subw(d, 1);
    while(evenp(d)){
        half(d);
        r += 1;
    }
   // now we have n = (2^r * d) + 1
    for(i = 0; i < k; i++){

        do {
            load_random(l, a);
            addw(a, 4);
        } while(gte(a, n));
        subw(a, 2);
        
        spow_nn(a, d, n, x);
         
        // if x = 1, continue
        if((highbit(x) == 0) && x[0] == 1){
           continue;
        }

        // if x = n-1 (represented by now unused a), continue
        copy(n, a);
        subw(a, 1);
        if(equ(x,a)){ 
           continue;
        }
        
        for(j = 0; j < r; j++){
            long_mul_mod(x, x, n, x);
            if(equ(x,a)){ 
                break;
            }
        }
        if(j == r){
            return 0;
        }
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
void load_miller_rabin(uint32_t k, uint32_t *p){
    bignum(n);
    size_t i, j;
    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);
    srandom(t.tv_sec ^ t.tv_nsec);;

    j = PRIME_BITS / W_SZ;
    load_random(j, n);
    set(0, 1, n); //enforce that n is odd
    set(PRIME_BITS-1, 1, n); //enforce that n > 2^b-1
    for(i=0; i < N_BITS; ++i){             
        if(rabin_test(k, n)){
            copy(n, p);
            return;
        }
        addw(n, j); //max ~= ln(2^b)
    }
    die("tried N_BITS numbers with no luck!\n");
}

/* 
    k must be valid, i.e.
        k->n = pq, p,q in P-{2}
        k->e in [3, n-1] in Z, s.t. 
            gcd(e, \lambda(p)) === gcd(e, lcm(p-1, q-1)) = 1
*/
void rsaep(rsa_pub *k, uint32_t *m, uint32_t *c){
    if(gte(m, k->n)){
        die("message too large for this exponent!\n");
    }
    spow_nn(m, k->e, k->n, c);    
}

void rsadp(rsa_pri *k, uint32_t *c, uint32_t *m){ 
    if(gte(m, k->n)){
        die("cipher too large for this exponent!\n");
    }
    spow_nn(c, k->d, k->n, m);
}

void genkeys(uint32_t exp, uint32_t mr_rounds, rsa_pub *ke, rsa_pri *kd){
    uint32_t quot; 
    bignum(a);
    bignum(b);
    bignum(e);
    bignum(d);
    bignum(n);

    addw(e, exp);
    do {
        // pick two PRIME_BITS-length primes through miller-rabin testing
        load_miller_rabin(mr_rounds, a);
        load_miller_rabin(mr_rounds, b);
        long_mul(a, b, d, n);
        zero(d);
        
        // calculate phi(n) = (a-1)(b-1), overwriting secret b as we do so
        subw(a, 1);
        subw(b, 1);
        long_mul(a, b, a, b);
        printf("calculated phi\n");

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
    
    uint32_t mr[N_SZ];
    uint32_t cr[N_SZ];
    
    zero(mr);
    zero(cr);

    os2ip(m, len, mr);
    
    rsaep(ke, mr, cr);
   
    i2osp(cr, SYM_SZ, c);

    return len;
}

int rsa_dec(rsa_pri *kd, char *c, char *m){ 
    
    uint32_t mr[N_SZ];
    uint32_t cr[N_SZ];
    
    zero(mr);
    zero(cr);

    os2ip(c, SYM_SZ, cr);

    rsadp(kd, cr, mr);

    int written = i2osp(mr, SYM_SZ, m);

    return written;
}
