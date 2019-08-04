#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define N_BITS 512
#define W_BITS 32
#define W_SZ   4
#define N_SZ   (((N_BITS)+(W_BITS-1)) / (W_BITS))

typedef struct _mm_s {
    uint32_t modulus[N_SZ];
    uint32_t m_inv[N_SZ];
    uint32_t r_inv[N_SZ];
} mm_system;

static inline void move(const uint32_t *fr, uint32_t *to){
    memcpy(to, fr, N_SZ*sizeof *fr);
}

static inline void dmove(const uint32_t *FR, uint32_t *TO){
    memcpy(TO, FR, N_SZ*2*sizeof *FR);
}

/*
    you shall be... erased!
*/
static inline void zero(uint32_t *victim){
    memset(victim, 0, N_SZ*sizeof *victim);
}

static inline void pf2(const char *name, const uint32_t *what){
    printf("%s=\t%#10.8x %#10.8x\n", name, what[1], what[0]);
}

static inline void pf4(const char *name, const uint32_t *what){
    printf("%s=\t%#10.8x %#10.8x:%#10.8x %#10.8x\n", 
    name, what[3], what[2], what[1], what[0]);
}

static inline void pf8(const char *name, const uint32_t *what){
    printf("%s=\t%#10.8x %#10.8x:%#10.8x %#10.8x:%#10.8x %#10.8x:%#10.8x %#10.8x\n", 
    name, what[7], what[6], what[5], what[4],
          what[3], what[2], what[1], what[0]);
}


/*
    initialise a Montgomery-arithmetic system with R = 2^(N_BITS). that means
    calculating m_inv, r_inv s.t. M(m_inv) - R(r_inv) = 1, satisfying 
    Bezout's lemma.

    this is relatively expensive; but once these values are known, they can 
    be reused (thus their storage in the struct mm_system).
*/
void init(const uint32_t *M, mm_system *mm);

/*
    convert a normal-space number to Montgomery-space, and vice-versa.
    conversion-to involves normal (non-Montgomery) division; it's expensive 
    relative to the Montgomery operations.
*/
void mm_conv(const uint32_t *a, const uint32_t *b, uint32_t *R);
void mm_redc(const uint32_t *A, const mm_system mm, uint32_t *r);

/*

*/
void mm_mul(uint32_t *A, const uint32_t *B, const mm_system mm);
void mm_exp(uint32_t *A, const uint32_t *B, const mm_system mm);

/*
    perform a round-trip conversion, multiplication, and reduction on
    two non-Montgomery numbers. the result (r) is in normal space.
*/
void mod_mul(const uint32_t *a, const uint32_t *b, const mm_system mm,
    uint32_t *r);

/*
    perform a round-trip conversion, exponentiation, and reduction on
    two non-Montgomery numbers. the result (r) is in normal space.
*/
void mod_exp(const uint32_t *a, const uint32_t *b, const mm_system mm,
    uint32_t *r);

