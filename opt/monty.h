//
// Created by auriga on 03/08/19.
//

#ifndef _MONTY_H
#define _MONTY_H

#include<stdint.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>

#define Lb 2048

#define L8 (Lb / 8)
#define w sizeof(uint32_t)
#define wb (w * 8)
#define L (Lb / wb)
#define LL (2*L)

typedef struct _mm_s {
    uint32_t N[L];     // modulus
    uint32_t iN[L];    // negative inverse modulus s.t. (N . iN) = -1 (mod R)
    uint32_t iR[L];    // inverse numlength (i.e., the aux. modulus) s.t. (R . iR) = 1 (mod N)
    uint32_t CF[L];    // conversion factor (R^2 (mod N)), s.t. mm_mult(a . cF) = aR (mod N) for any a < N
} Mont;

static inline void mvwide(const uint32_t *U, uint32_t *T){
    if(U != T) memcpy(T, U, 2*L8);
}

static inline void mv(const uint32_t *u, uint32_t *t){
    if(u != t) memcpy(t, u, L8);
}

/* Exported symbols */

void declaim(const char *msg, const uint32_t *a, int len);

void mm_init(const uint32_t *M, Mont *mm);

void mm_conv(const uint32_t t[L], const Mont *Mo, uint32_t T[L]);

void mm_redc(const uint32_t *T, const Mont *Mo, uint32_t *t);

void mm_mult(uint32_t *A, const uint32_t *B, const Mont *Mo);

void mm_exp(uint32_t *A, const uint32_t *b, int len_b, const Mont *Mo);

#endif //FUNC_MM_MONTY_H
