#include "bignum/bignum.h"
#include "octets.h"
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

typedef struct _rsa_pub_s {
    Word n[N_SZ];
    Word e[N_SZ];
} rsa_pub;

typedef struct _rsa_pri_s {
    Word n[N_SZ];
    Word d[N_SZ];
} rsa_pri;

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
    for(i=0; i < N_BITS; i++){             
        if(rabin_test(k, n)){
            copy(n, p);
            return;
        }
        if(!(i % 50))
            printf("tried %d numbers...\n", i);
        addws(n, 2);
    }
    die("tried N_BITS numbers with no luck!\n");
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

int main(){
    Word p[N_SZ];
    Word q[N_SZ];
    Word n[N_SZ];
    Word phi[N_SZ];

    load_miller_rabin(32, p);
    load_miller_rabin(32, q);
    zero(n);
    
    mul(p, q, n);

    printf("pq = %d-digit n\n", highbit(n));
    print_num(n);

    rsa_pub ke; 
    rsa_pri kd;

    copy(n, ke.n);
    copy(n, kd.n);

    subws(p,1);
    subws(q,1);

    mul(p,q,phi);

    // even without Carmichael's lambda, we can strip out powers of 2
    while(evenp(p) && evenp(q)){
        half(p);
        half(q);
        half(phi);
    }

    zero(p);
    zero(q);

    int i;
    Word inv;

    unsigned long long dec_phi = to_dec(phi);

    for(i=3; i<=257; i+=2){
        load(ke.e, i);
        inv = !(euclid_inv(ke.e, phi, kd.d));
        if(inv){
            break;
        }
    }
    if(i == 258){
        printf("great scott! you've found a super-composite number!\n");
        print_num(phi);
        die("mathematical discovery\n");
    }

    printf("\nmodulus is:\n");
    print_num(n);

    char *msg = "UVic";
    int mlen = strlen(msg);
    
    char out[4096];
    
    printf("\nplaintext:\n%s\n", msg);

    Word mrep[N_SZ];
    zero(mrep);
    
    os2ip(msg, mlen, mrep);
    printf("\nmessage representative:\n");
    print_num(mrep);

    rsaep(&ke, mrep, phi);
    
    printf("\nc = m^%llu (mod %llu)\nciphertext representative:\n",
        to_dec(ke.e), to_dec(ke.n));
    print_num(phi);

    rsadp(&kd, phi, mrep);

    printf("\nm'= c^%llu (mod %llu)\nrecovered plaintext representative:\n",
        to_dec(kd.d), to_dec(kd.n));
    print_num(mrep);

    i2osp(mrep, mlen+1, out); 
    out[mlen] = 0;

    printf("\nrecovered plaintext:\n%s\n", out);

    return 0;
}
