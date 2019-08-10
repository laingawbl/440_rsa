#include "monty.h"

#include<stdlib.h>
#include<time.h>

void list(const uint32_t *a, int len){
    declaim("num", a, len);

}

void declaim(const char *msg, const uint32_t *a, int len){
    printf("%s\t", msg);
    int i;
    for(i = len-1; i >= 0; --i){
        if((i+1)%8 == 0) printf("\n 2^%4d  ", (i+1)*32);
        printf("%#10.8x ", a[i]);
    }
    printf("\n");
}

uint32_t gte(const uint32_t a[L], const uint32_t b[L]){
	int i;
	for(i = L-1; i >= 0; i--){
		if(a[i] == b[i]) continue;
		if(a[i] > b[i]) return 1;
		return 0;
	}
	return 1;
}

uint32_t iszero(const uint32_t *a) {
    int i;
    for(i = L-1; i >= 0; i--){
        if(a[i] != 0) return 0;
    }
    return 1;
}

void mult(const uint32_t a[L], const uint32_t b[L], uint32_t T[LL]){
	uint32_t Prod[LL] = {0};
	uint32_t c;
	int i, j;
	
	for(i = 0; i < L; i++){
		c = 0;
		for(j = 0; j < L; j++){
			uint64_t n = (uint64_t)Prod[i+j] + (uint64_t)c + ((uint64_t)a[i] * (uint64_t)b[j]);
			c = (uint32_t)(n >> wb);
			Prod[i+j] = (uint32_t) n;
		}
		Prod[i+L] = c;
	}

	mvwide(Prod, T);
}

uint32_t add(const uint32_t A[L], const uint32_t B[L], uint32_t T[L]){
    uint32_t Sum[L] = {0};
	uint32_t c = 0;
	int i;
	for(i = 0; i < L; i++){
		uint64_t n = A[i] + B[i] + c;
		c = (c & (n == A[i])) | (n < A[i]);
		Sum[i] = (uint32_t)n;
	}
	mv(Sum, T);
	return c;
}

uint32_t addwide(const uint32_t A[LL], const uint32_t B[LL], uint32_t T[LL]){
    uint32_t Sum[LL] = {0};
    uint32_t c = 0;
    int i;

    for(i = 0; i < LL; i++){
        uint64_t n = A[i] + B[i] + c;
        c = (c & (n == A[i])) | (n < A[i]);
        Sum[i] = (uint32_t)n;
    }
    mvwide(Sum, T);
    return c;
}

uint32_t sub(const uint32_t A[L], const uint32_t B[L], uint32_t T[L]){
	uint32_t Sum[L] = {0};
	uint32_t c = 0;
	int i;
	
	for(i = 0; i < L; i++){
		uint32_t n = A[i] - B[i] - c;
		c = (c & (n == A[i])) | (n > A[i]);
		Sum[i] = (uint32_t) n;
	}
	mv(Sum, T);
	return c;
}

uint32_t subwide(const uint32_t A[LL], const uint32_t B[LL], uint32_t T[LL]){
    uint32_t Sum[LL] = {0};
    uint32_t c = 0;
    int i;

    for(i = 0; i < LL; i++){
        uint32_t n = A[i] - B[i] - c;
        c = (c & (n == A[i])) | (n > A[i]);
        Sum[i] = (uint32_t) n;
    }
    mvwide(Sum, T);
    return c;
}

void mm_init(const uint32_t n[L], Mont *Mo){

    // FIRST PART: set up iN, iR.

    uint32_t u[L] = {0};
    uint32_t v[L] = {0};
    int r;
    u[0] = 1;

    for(r = Lb-1; r >= 0; r--){
        uint32_t cv = (u[0] & 1);
        uint32_t cu = cv ? add(u, n, u) : 0;
        int k;
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
        for(k = L; k > 0; k--){ // shift u, v right 1 place
            uint32_t ncu = u[k-1] & 1;
            uint32_t ncv = v[k-1] & 1;
            u[k-1] = (cu << 31) | (u[k-1] >> 1);
            v[k-1] = (cv << 31) | (v[k-1] >> 1);
            cu = ncu;
            cv = ncv;
        }
    } // upon exiting, r=0, and thus, 2^r = 1 = gcd(R,M) (if called properly)

    mv(n, Mo->N);
    mv(v, Mo->iN);
    mv(u, Mo->iR);

    // SECOND PART: set up CF.

    uint32_t R[L] = {0};
    R[0] = 1;
    for(r = 0; r < 2*Lb; r++){
        int k;

        for(k = L-1; k > 0; k--){ // shift R left one place
            R[k] = (R[k] << 1) | (R[k-1] >> 31);
        }
        R[0] <<= 1;

        if(gte(R, n)){  // conditionally subtract the modulus
            sub(R, n, R);
        }
    }  // upon exiting, R = (1 << 2*Lb) (mod N) = R^2 (mod N)

    mv(R, Mo->CF);
}

/*
    Given wide T, computes narrow t = T/R mod N.

	Assuming T < N^2 < R^2, we only have to do either 0 or 1 subtractions in the
	last 3 lines:
	    TN' mod R < R
	->  (TN' mod R)N < RN
	->  T + (TN' mod R)N  < (N^2 + RN)
	->  (T + (TN' mod R)N)/R < (N^2 + RN)/R < 2N
	->  Acc < 2N

	The only thing we leak is the word size L.

	calls:
		mult() 2 times
		add()/sub() 2 times (at worst)
		memcpy() 4 times
*/
void mm_redc(const uint32_t T[LL], const Mont *Mo, uint32_t t[L]) {
    uint32_t Acc[LL] = {0};
    uint32_t c;

    mv(T, Acc);                 //Acc = narrow T mod R
    mult(Acc, Mo->iN, Acc);     //Acc = wide  (T mod R) N'
    modR(Acc);                  //Acc = narrow m := (TN') mod R
    mult(Acc, Mo->N, Acc);      //Acc = wide   mN
    addwide(T, Acc, Acc);       //Acc = wide   T + mN
    divR(Acc);                  //Acc = narrow t := (T + mN) / R

    if (gte(Acc, Mo->N)) {
        sub(Acc, Mo->N, Acc);
    }
    assert(gte(Mo->N, Acc));

    mv(Acc, t);
}

/*
 * Given narrow t, computes narrow T:
 *  T = mm_mult(t . CF)
 *    = mm_mult(t . R^2 mod N)
 *    = (((tR^2 mod N) / R) mod N)
 *    = tR mod N
 * You MUST have t < Mo->N.
 */
void mm_conv(const uint32_t t[L], const Mont *Mo, uint32_t T[L]){
    mv(Mo->CF, T);
    mm_mult(T, t, Mo);
}

/*
	calls:
	- mult() 3 times
	- add()/sub() 2 times
	- memcpy() 4 times
*/
void mm_mult(uint32_t A[L], const uint32_t B[L], const Mont *Mo) {
    uint32_t tmp[LL] = {0};
	mult(A, B, tmp);
    mm_redc(tmp, Mo, A);
    assert(gte(Mo->N, A));
}

/*
	Constant-time Montgomery ladder using Montgomery multiplication (a sort of 
	Montryoshka). like regular square-and-multiplying, it works on the recurrence
	
	x^n = { x * (x^2)^(n-1)/2,  if n is odd
	      { (X^2)^(n/2),        if n is even
	      
	Each bit of b after the highest (len_b - 1) produces 2 calls to mm_mult,
	whether that bit is even or odd. Both of those calls involve a single 
	auxiliary number (Prod, then Acc) which is only live within the scope of the
	function.
	
	So, a 2048-bit exponentiation should have:
	- 12285 calls to mult()
	- 8190  calls to add() / sub()
	- 16383 calls to memcpy() (3 outside the loop)
	- auxiliary space 6*L = 3072 bytes (T1, T2, and Prod/Acc)

    Remember! b should NOT be a Montgomery-form number.
	
	TODO: as A is mutable, can I replace T1 with A?
    TODO: update call counts
*/
void mm_exp(uint32_t A[L], const uint32_t b[L], int len_b, const Mont *Mo){
	uint32_t T1[L] = {0};
	uint32_t T2[L] = {0};
	int i;
	
	mv(A, T1);
	mv(A, T2);
	mm_mult(T2, T2, Mo);
	
	for(i = len_b - 1; i >= 0; i--){
		if(sel(b, i)){          // if B_i is odd, t1 <- (t1 t2), t2 <- (t2)^2
			mm_mult(T1, T2, Mo);
			mm_mult(T2, T2, Mo);
		}
		else {                  // else, t2 <- (t1 t2), t1 <- (t1)^2
			mm_mult(T2, T1, Mo);
			mm_mult(T1, T1, Mo);
		}
		declaim("T1", T1, L);
		declaim("T2", T2, L);
	}
	mv(T1, A);
}