#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include<stdint.h>
#include<stdio.h>

#define N_BITS 128
#define WHICH_WORD 4

/*
    we are going to be generating primes in the range
    2^(keylength/4), 2^(keylength/2) - 1
    so the minimum prime bitlength is PRIME_BITS / 2
*/

#define PRIME_BITS ((N_BITS) >> 1)

#ifndef WHICH_WORD
    #define WHICH_WORD 4
#endif

#if (WHICH_WORD == 4)
    #define WORD_T  uint32_t
    #define DWORD_T uint64_t
    #define W_MASK  0xFFFFFFFF
    #define W_SZ    32
#elif (WHICH_WORD == 2)
    #define WORD_T  uint16_t
    #define DWORD_T uint32_t
    #define W_MASK  0xFFFF
    #define W_SZ    16
#elif (WHICH_WORD == 1)
    #define WORD_T  uint8_t
    #define DWORD_T uint16_t
    #define W_MASK  0xFF
    #define W_SZ    8
#endif

#ifndef WORD_T
    #error must define WHICH_WORD in {1, 2, 4} bytes
#endif

#define N_SZ (N_BITS / W_SZ) 

typedef WORD_T  Word;
typedef DWORD_T DWord;

/* 
    zero out a big num. at -O3 and higher, gcc replaces this with a call to 
    memset.
*/
void zero(Word *n);

/*
    deep copy one big num into another.
*/
void copy(Word *fr, Word *to);

/*
    these operations all allow a and r to be the same number, i.e.,
    carrying out a += b or a -= b.
*/
// add two big nums
Word add(Word *a, Word *b, Word *r);
// add Word to big num
Word addw(Word *a, Word  b, Word *r);
// subtract two big nums
Word sub(Word *a, Word *b, Word *r);
// subtract Word from big num
Word subw(Word *a, Word  b, Word *r);
// long-divide Word by Word (quotient and remainder)
Word qdiv(Word *a, Word *b, Word *q, Word *r);
// long-multiply Word by Word
void mul(Word *a, Word *b, Word *hi, Word *lo);

/*
    syntactic sugar for the above with a=r explicitly
*/
Word adds(Word *a, Word *b);
Word addws(Word *a, Word b);
Word subs(Word *a, Word *b);
Word subws(Word *a, Word b);
void muls(Word *a, Word *b, Word *hi);

// add/sub with correction modulo m (assumes 0 <= a,b < m)
void add_mo(Word *a, Word *b, Word *m, Word *r);
void sub_mo(Word *a, Word *b, Word *m, Word *r);
void mul_mo(Word *a, Word *b, Word *m, Word *r);

/*
    logical shift left/right by a certain number of bits less than W_SZ
    (i.e., it cannot be used for shifts larger than one Word wide)
*/
Word lsl(Word *a, Word n, Word *r);
Word lsr(Word *a, Word n, Word *r);
Word lsls(Word *a, Word n);
Word lsrs(Word *a, Word n);

Word sel(Word p, Word *b);
void set(Word p, Word a, Word *b);

void andb(Word *a, Word *b, Word *r);
void xorb(Word *a, Word *b, Word *r);

/*
    zero predicate, i.e., test if *a is zero
*/
Word zerop(Word *a);

/*
    even/odd predicates, i.e., test if the lsb of *a is 0 or 1
*/
Word evenp(Word *a);
Word oddp(Word *a);

/*
    return the position of the highest nonzero bit in a big num.
*/
Word highbit(Word *a);

Word gte(Word *a, Word *b);
Word equ(Word *a, Word *b);

/*
    sugar for lsl/lsr(a, 1).
*/
Word half(Word *a);
Word twice(Word *a);

/*
    zero followed by addw.
*/
void load(Word *a, Word b);

void print_num(Word *a);

#endif
