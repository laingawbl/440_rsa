#include "monty.h"

void list(const uint32_t *a, int len){
    int i;
    printf("number:");
    for(i = len-1; i >= 0; --i){
        if((i+1)%8 == 0) printf("\n 2^%4d  ", (i+1)*32);
        printf("%#10.8x ", a[i]);
    }
    printf("\n");
}

uint32_t gte(const uint32_t *a, const uint32_t *b, int len){
	int i;
	for(i = len-1; i >= 0; i--){
		if(a[i] == b[i]) continue;
		if(a[i] > b[i]) return 1;
		return 0;
	}
	return 1;
}

void mult(const uint32_t *a, const uint32_t *b, uint32_t *T){
	uint32_t Prod[2*L] = {0};
	uint32_t c;
	int i, j;
	
	for(i = 0; i < L; i++){
		c = 0;
		for(j = 0; j < L; j++){
			uint64_t n = (uint64_t)Prod[i+j] + (uint64_t)c + ((uint64_t)a[i] * (uint64_t)b[j]);
			c = (uint32_t)(n >> 32);
			Prod[i+j] = (uint32_t) n;
		}
		Prod[i+L] = c;
	}

	mvwide(Prod, T);
}

uint32_t add(const uint32_t *A, const uint32_t *B, int len, uint32_t *T){
    uint32_t Sum[2*L] = {0};
	uint32_t c = 0;
	int i;

	for(i = 0; i < len; i++){
		uint64_t n = A[i] + B[i] + c;
		c = (c & (n == A[i])) | (n < A[i]);
		Sum[i] = (uint32_t)n;
	}
	if(len > L)
		mvwide(Sum, T);
	else
		mv(Sum, T);
	return c;
}

uint32_t sub(const uint32_t *A, const uint32_t *B, int len, uint32_t *T){
	uint32_t Sum[2*L] = {0};
	uint32_t c = 0;
	int i;
	
	for(i = 0; i < len; i++){
		uint32_t n = A[i] - B[i] - c;
		c = (c & (n == A[i])) | (n > A[i]);
		Sum[i] = (uint32_t) n;
	}
	if(len > L)
		mvwide(Sum, T);
	else
		mv(Sum, T);
	return c;
}

void iter_residue(uint32_t *a, const uint32_t *b){
    while(gte(a, b, 2*L)){
        sub(a, b, 2*L, a);
    }
}

/*
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
void redc(const uint32_t *T, const Mont *Mo, uint32_t *t) {
    uint32_t Acc[2 * L] = {0};
    uint32_t c;

    mv(T, Acc);                  //Acc = T mod R
    mult(Acc, Mo->iN, Acc);      //Acc = (T mod R) N'
    modR(Acc);                   //Acc = m := (TN') mod R
    mult(Acc, Mo->N, Acc);       //Acc = mN
    c = add(T, Acc, 2 * L, Acc); //Acc = T + mN
    divR(Acc);                   //Acc = t := (T + mN) / R

    if (c || gte(Acc, Mo->N, L)) {
        sub(Acc, Mo->N, L, Acc);
    }

    assert(gte(Mo->N, Acc, L));
	mv(Acc, t);
}

void mm_init(const uint32_t *n, Mont *Mo){
    uint32_t u[L] = {0};
    uint32_t v[L] = {0};
    int r;
    u[0] = 1;

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
    */
    for(r = Lb-1; r >= 0; r--){
        uint32_t cv = (u[0] & 1);
        uint32_t cu = cv ? add(u, n, L, u) : 0;
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
        for(k = L; k > 0; k--){
            uint32_t ncu = u[k-1] & 1;
            uint32_t ncv = v[k-1] & 1;
            u[k-1] = (cu << 31) | (u[k-1] >> 1);
            v[k-1] = (cv << 31) | (v[k-1] >> 1);
            cu = ncu;
            cv = ncv;
        }
    }

    /*
        upon exiting, r=0, and thus, 2^r = 1 = gcd(R,M) (if called properly)
    */
    mv(n, Mo->N);
    mv(v, Mo->iN);
    mv(u, Mo->iR);
}

/*
 * You MUST have t < Mo->N.
 */
void mm_conv(const uint32_t *t, const Mont *Mo, uint32_t *T){
    uint32_t A[2*L] = {0};
    uint32_t B[2*L] = {0};
    uint32_t bb = 0;
    int i, j, k;

    mv(t, A);
    mv(Mo->N, B);

    for(k = L-1; (k >= 0); k--) {
        if (Mo->N[k] != 0) {
            bb = Mo->N[k];
            break;
        }
    }
    assert(bb != 0);

    /*
        this loop has the invariant A = t * 2^(32k) mod N
    */
    for(i = 0; i < L; i++){
        uint32_t ha;
        uint64_t aa, Q, c;

        // A = A * 2^32
        memmove(A+1, A, L8);
        A[0] = 0;

        // take the upper two words of a, and call them aa, a truncation
        // of a to between 33 and 64 bits of precision.
        ha = A[k+1];
        if(ha == bb){
            Q = 0xFFFFFFFF;
        }
        else {
            aa = ((uint64_t)ha << 32) + (uint64_t)A[k];
            Q = (aa / (uint64_t)bb);
            if(Q > 16){
                Q -= 16;
            }
            else{
                Q = 0;
            }
        }

        c = 0;
        if(Q != 0){
            uint32_t s[2*L] = {0};
            s[0] = (uint32_t) Q;
            s[1] = (uint32_t)(Q>>32);
            mult(Mo->N, s, s);     // S = QN
            c = sub(A, s, 2*L, A); // A = A - QN

        }

        assert(c == 0);
        // now we only need to call iter_residue a small number of times.
        iter_residue(A, B);
    }
    mvwide(A, T);
}

/*
	calls:
	- mult() 3 times
	- add()/sub() 2 times
	- memcpy() 4 times
*/
void mm_mult(uint32_t *A, const uint32_t *B, const Mont *Mo) {
	mult(A, B, A);
	redc(A, Mo, A);
}

/*
	Constant-time Montgomery ladder using Montgomery multiplication (a sort of 
	Montryoshka). like regular square-and-multiplying, it works on the recurrence
	
	x^n = { x * (x^2)^(n-1)/2,  if n is odd
	      { (X^2)^(n/2),        if n is even
	      
	Each bit of B after the highest (len_b - 1) produces 2 calls to mm_mult, 
	whether that bit is even or odd. Both of those calls involve a single 
	auxiliary number (Prod, then Acc) which is only live within the scope of the
	function.
	
	So, a 2048-bit exponentiation should have:
	- 12285 calls to mult()
	- 8190  calls to add() / sub()
	- 16383 calls to memcpy() (3 outside the loop)
	- auxiliary space 6*L = 3072 bytes (T1, T2, and Prod/Acc)
	
	TODO: as A is mutable, can I replace T1 with A?
*/
void mm_exp(uint32_t *A, const uint32_t *B, int len_b, const Mont *Mo){
	uint32_t T1[2*L] = {0};
	uint32_t T2[2*L] = {0};
	int i;
	
	mv(A, T1);
	mv(A, T2);
	mm_mult(T2, T2, Mo);
	
	for(i = len_b - 1; i >= 0; --i){ 
		if(sel(B, i)){          // if B_i is odd, t1 <- (t1 t2), t2 <- (t2)^2
			mm_mult(T1, T2, Mo);
			mm_mult(T2, T2, Mo);
		}
		else {                  // else, t2 <- (t1 t2), t1 <- (t1)^2
			mm_mult(T2, T1, Mo);
			mm_mult(T1, T1, Mo);
		}
	}
	mvwide(T1, A);
}

int main(void) {
    uint32_t mod[L] = {0};
    uint32_t acc[2*L] = {0};
    uint32_t res[2*L] = {0};

    mod[15]= 0xcf6d59c9;
    mod[14]= 0xb1124b9c;
    mod[13]= 0x980b3bdb;
    mod[12]= 0xc63c7746;
    mod[11]= 0x84467c12;
    mod[10]= 0x1e385ab8;
    mod[9] = 0xf81f2395;
    mod[8] = 0x525db17a;
    mod[7] = 0xde78053c;
    mod[6] = 0x410e682b;
    mod[5] = 0xa7e50e34;
    mod[4] = 0x1664298f;
    mod[3] = 0xbbd94152;
    mod[2] = 0x07ef9494;
    mod[1] = 0x783f4d7e;
    mod[0] = 0xfe955f61;

    //mod[0] = 0x186a3; // 100003
    Mont mm;
    mm_init(mod, &mm);

    int i;
    for(i = 1; i < 65535; i++) {
        memset(acc, 0, 2*L8);
        memset(res, 0, 2*L8);
        acc[0] = (uint32_t)i;

        mm_conv(acc, &mm, acc);
        mm_mult(acc, acc, &mm);

        redc(acc, &mm, res);
        if(i%100 == 0) printf("done %d^2\n", i);
        assert(res[0] == i*i);
    }
}
