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

#define sel(T,n) ((T[n/wb] >> (n%wb)) & 1)

typedef struct _mm_s {
    uint32_t N[L];
    uint32_t iN[L];
    uint32_t iR[L];
} Mont;

/* Static inlines */

static inline void mvwide(const uint32_t *U, uint32_t *T){
    if(U != T) memcpy(T, U, 2*L8);
}

static inline void mv(const uint32_t *u, uint32_t *t){
    if(u != t) memcpy(t, u, L8);
}

/*
	take a wide number T = [hi][lo] -> [0][hi]
*/
static inline void divR(uint32_t *T){
    memcpy(T, T+L, L8);
    memset(T+L, 0, L8);
}

/*
	take a wide number T = [hi][lo] -> [0][lo]
*/
static inline void modR(uint32_t *T){
    memset(T+L, 0, L8);
}

/* Exported symbols */

void mm_init(const uint32_t *M, Mont *mm);

void mm_mult(uint32_t *A, const uint32_t *B, const Mont *Mo);

void mm_exp(uint32_t *A, const uint32_t *B, int len_b, const Mont *Mo);

#endif //FUNC_MM_MONTY_H
