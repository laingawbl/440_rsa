#include<stdint.h>

#define MOD_MAX_SZ 4096
#define MOD_MAX_PRIME ((MAX_RSA_SZ) >> 1)

#define WORD   uint32_t
#define DWORD  uint64_t
#define W_MASK 0xFFFFFFFF
#define W_SZ   32

#define SZ_NUM (MOD_MAX_SZ / W_SZ) 

/* 
    zero out a big num. at -O3 and higher, gcc replaces this with a call to 
    memset.
*/
void zero(WORD *n);

/*
    deep copy one big num into another.
*/
void copy(WORD *n);

/*
    these 4 operations all allow a and r to be the same number, i.e.,
    carrying out a += b or a -= b.
*/

// add two big nums
WORD add_nn(WORD *a, WORD *b, WORD *r);
// add WORD to big num
WORD add_nw(WORD *a, WORD  b, WORD *r);
// subtract two big nums
WORD sub_nn(WORD *a, WORD *b, WORD *r);
// subtract WORD from big num
WORD sub_nw(WORD *a, WORD  b, WORD *r);

// add/sub with correction modulo m (slow!)
WORD add_mo(WORD *a, WORD *b, WORD *m, WORD *r);
WORD sub_mo(WORD *a, WORD *b, WORD *m, WORD *r);

/*
    logical shift left/right by a certain number of bits less than W_SZ
    (i.e., it cannot be used for shifts larger than one WORD wide)
*/
WORD lsl_b(WORD *a, WORD b, WORD *r);
WORD lsr_b(WORD *a, WORD b, WORD *r);

WORD sel(WORD p, WORD *b);
WORD set(WORD p, WORD a, WORD *b);

void and_nn(WORD *a, WORD *b, WORD *r);
void xor_nn(WORD *a, WORD *b, WORD *r);
WORD zerop(WORD *a);

/*
    return the position of the highest nonzero bit in a big num.
*/
WORD highbit(WORD *a);
WORD gte(WORD *a, WORD *b);
