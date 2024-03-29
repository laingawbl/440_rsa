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
        printf("%8.8x ", a[i]);
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

void mult(const uint32_t a[L], const uint32_t b[L], uint32_t T[LL]){
	uint32_t Prod[LL] = {0};
	uint32_t c;
	int i, j;
	
	for(i = 0; i < L; i++){
		c = 0;
		for(j = 0; j < L; j+=8){
		    uint64_t la = (uint64_t) a[i];
		    uint64_t n;

				n = (uint64_t)Prod[i+j] + (uint64_t)c + (la * (uint64_t)b[j]);
				Prod[i+j] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+1] + (n >> wb) + (la * (uint64_t)b[j+1]);
        Prod[i+j+1] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+2] + (n >> wb) + (la * (uint64_t)b[j+2]);
        Prod[i+j+2] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+3] + (n >> wb) + (la * (uint64_t)b[j+3]);
        Prod[i+j+3] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+4] + (n >> wb) + (la * (uint64_t)b[j+4]);
        Prod[i+j+4] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+5] + (n >> wb) + (la * (uint64_t)b[j+5]);
        Prod[i+j+5] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+6] + (n >> wb) + (la * (uint64_t)b[j+6]);
        Prod[i+j+6] = (uint32_t) n;

        n = (uint64_t)Prod[i+j+7] + (n >> wb) + (la * (uint64_t)b[j+7]);
        Prod[i+j+7] = (uint32_t) n;
        c = (uint32_t)(n >> wb);
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

    uint32_t R[L+1] = {0};
    R[0] = 1;
    for(r = 0; r < 2*Lb; r++){
        int k;

        for(k = L; k > 0; k--){ // shift R left one place
            R[k] = (R[k] << 1) | (R[k-1] >> 31);
        }
        R[0] <<= 1;

        if(R[L] || gte(R, n)){  // conditionally subtract the modulus
            sub(R, n, R);
            R[L] = 0;
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
    uint32_t Tl[L];
    uint32_t Nl[L];
    uint32_t Nnl[L];
    uint32_t i, j, c;

    mv(T, Tl);
    mv(Mo->N, Nl);
    mv(Mo->iN, Nnl);

    for(i = 0; i < L; i++){       //Acc = narrow (T half-mul N') = (TN') mod R
        c = 0;
        for(j = 0; j < (L-i); j++){
            uint64_t n = (uint64_t)Acc[i+j] + (uint64_t)c + ((uint64_t)Nnl[i] * (uint64_t)Tl[j]);
            c = (uint32_t)(n >> wb);
            Acc[i+j] = (uint32_t) n;
        }
    }

    mult(Acc, Mo->N, Acc);      //Acc = wide   mN

    c = 0;
    for(i = 0; i < L; i++){     // this lower half is done solely to compute the carry (for the LSB of t).
        uint64_t n1 = T[i] + Acc[i] + c;
        c = (c & (n1 == T[i])) | (n1 < T[i]);
        Acc[i] = (uint32_t)n1;
    }
    for(; i < LL; i++){    //narrow t := (T + mN) / R
        uint64_t n1 = T[i] + Acc[i] + c;
        c = (c & (n1 == T[i])) | (n1 < T[i]);
        t[i-L] = (uint32_t)n1;
    }

    if (c || gte(t, Nl)) {
        sub(t, Nl, t);
    }
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
    mv(t, T);
    mm_mult(T, Mo->CF, Mo);
}

void mm_mult(uint32_t A[L], const uint32_t B[L], const Mont *Mo) {
    uint32_t tmp[LL] = {0};
	mult(A, B, tmp);
    mm_redc(tmp, Mo, A);
}

/*
	Square-and-multiply exponentiation using Montgomery multiplication. it
    works on the recurrence
	
	x^n = { x * (x^2)^(n-1)/2,  if n is odd
	      { (X^2)^(n/2),        if n is even

    Remember! b should NOT be a Montgomery-form number.
*/
void mm_exp(uint32_t A[L], const uint32_t b[L], int len_b, const Mont *Mo){
	uint32_t T1[L] = {0};
	uint32_t T2[L];
	int i;

	T1[0] = 1;
	mm_mult(T1, Mo->CF, Mo);
    mv(A, T2);

    for(i = 0; i < (len_b/wb); i++){
        uint32_t e = b[i];
				// for each bit in b, if b_i is odd, t1 <- (t1 t2), t2 <- (t2)^2	
        if(e & 1)  
            mm_mult(T1, T2, Mo);
        mm_mult(T2, T2, Mo);

        if(e >>  1 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  2 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  3 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  4 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  5 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  6 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  7 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);

        if(e >>  8 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >>  9 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 10 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 11 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 12 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 13 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 14 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 15 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);

        if(e >> 16 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 17 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 18 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 19 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 20 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 21 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 22 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 23 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);

        if(e >> 24 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 25 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 26 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 27 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 28 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 29 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 30 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
        if(e >> 31 & 1) mm_mult(T1, T2, Mo); mm_mult(T2, T2, Mo);
    }

    uint32_t e = b[i];
	for(i = 0; i <= len_b%wb; i++){
        if(e >> i & 1){
            mm_mult(T1, T2, Mo);
		}
		mm_mult(T2, T2, Mo);
	}
	mv(T1, A);
}
