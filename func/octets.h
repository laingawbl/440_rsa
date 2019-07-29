#ifndef __OCTETS_H__
#define __OCTETS_H__

#include "bignum.h"
#include<string.h>

#define SYM_SZ (N_BITS / 8)

unsigned long long to_dec(const uint32_t *a);
int os2ip(const char *oct, const size_t len, uint32_t *num); 
int i2osp(const uint32_t *num, const size_t len, char *oct);

#endif
