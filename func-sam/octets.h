#ifndef __OCTETS_H__
#define __OCTETS_H__

#include "bignum.h"
#include<string.h>

unsigned long long to_dec(Word *a);
int os2ip(char *oct, int len, Word *num); 
int i2osp(Word *num, int len, char *oct);

#endif
