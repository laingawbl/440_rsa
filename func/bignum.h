#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include<stdint.h>
#include<stdio.h>
#include<string.h>

/*
    N_BITS is 65537 at max 
*/

#define N_BITS 128
#define WHICH_WORD 4

/*
    we are going to be generating primes in the range
    2^(keylength/4), 2^(keylength/2) - 1
    so the minimum prime bitlength is PRIME_BITS - 1
*/

#define PRIME_BITS ((N_BITS) >> 1)
    
#define WORD_T  uint32_t
#define DWORD_T uint64_t
#define W_MASK  0xFFFFFFFF
#define W_SZ    32

#define N_SZ (N_BITS / W_SZ)
#define _N_INTERNAL_LEN N_SZ + 2 

#define N_GUARD _N_INTERNAL_LEN - 2
#define N_SZINFO _N_INTERNAL_LEN - 1

#define bitsize(a) (a[N_SZINFO])
#define wordsize(a) ((bitsize(a)) + (W_SZ - 1) / W_SZ)

#define widen(a) a[N_SZINFO] = N_BITS-1
#define bignum(a)  uint32_t a[_N_INTERNAL_LEN] = {0}; widen(a);

inline void zero(uint32_t *n){
    memset(n, 0, _N_INTERNAL_LEN);
}

inline void copy(const uint32_t *fr, uint32_t *to){
    memcpy(to, fr, _N_INTERNAL_LEN);
}

/*
    these operations all assign the result to a.
*/
// add two big nums
uint32_t add(uint32_t *a, const uint32_t *b);
// add uint32_t to big num
uint32_t addw(uint32_t *a, const uint32_t b);
// subtract two big nums
uint32_t sub(uint32_t *a, const uint32_t *b);
// subtract uint32_t from big num
uint32_t subw(uint32_t *a, const uint32_t  b);
// long-divide uint32_t by uint32_t (quotient and remainder)
void long_div(const uint32_t *a, const uint32_t *b, 
    uint32_t *pq, uint32_t *pr);
// long-multiply uint32_t by uint32_t
void long_mul(const uint32_t *a, const  uint32_t *b, 
    uint32_t *hi, uint32_t *lo);

// add/sub with correction modulo m (assumes 0 <= a,b < m)
void add_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r);
void sub_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r);
void long_mul_mod(const uint32_t *a, const uint32_t *b, 
    const uint32_t *m, uint32_t *r);

/*
    logical shift left/right by a certain number of bits less than W_SZ
    (i.e., it cannot be used for shifts larger than one uint32_t wide)
*/
uint32_t lsl(uint32_t *a, const size_t n);
uint32_t lsr(uint32_t *a, const size_t n);

inline uint32_t sel(const size_t p, const uint32_t *b){
    return (b[p / W_SZ] >> (p % W_SZ)) & 1;
}

inline void set(const size_t p, const uint32_t a, uint32_t *b){
    b[p / W_SZ] = (b[p / W_SZ] & ~(a << (p % W_SZ))) | (a << (p % W_SZ));
}
/*
    zero predicate, i.e., test if *a is zero
*/
uint32_t zerop(const uint32_t *a);

/*
    even/odd predicates, i.e., test if the lsb of *a is 0 or 1
*/
inline uint32_t evenp(const uint32_t *a){
    return ((a[0] & 1) == 0);
}

inline uint32_t oddp(const uint32_t *a){
    return ((a[0] & 1) == 1);
}
/*
    return the position of the highest nonzero bit in a big num.
*/
size_t highbit(const uint32_t *a);
uint32_t gte(const uint32_t *a, const uint32_t *b);
uint32_t equ(const uint32_t *a, const uint32_t *b);

/*
    sugar for lsl/lsr(a, 1).
*/
uint32_t half(uint32_t *a);
uint32_t twice(uint32_t *a);

void print_num(const uint32_t *a);

#endif
