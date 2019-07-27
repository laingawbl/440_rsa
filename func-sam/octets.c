#include "octets.h"

/*
    if you know the contents of bignum will fit inside an
    unsigned long long, use this
*/
unsigned long long to_dec(Word *a){
    return (((unsigned long long)a[1] << W_SZ) | a[0]);
}

int i2osp(Word *num, int len, char *oct){
    int hibyte = (highbit(num)+7)/8;
    if(hibyte >= len){
        printf("message too long!\n");
        return 0;
    }
    int hiword = hibyte/WHICH_WORD;
    int rem = hibyte-(hiword*WHICH_WORD);
    int i;

    for(i = 0; i < hiword; i++){
#if (WHICH_WORD == 1)
        oct[i] = num[i];
#elif (WHICH_WORD == 2)
        oct[2*i] = num[i] & 0xff;
        oct[2*i + 1] = (num[i] >> 8) & 0xff;
#elif (WHICH_WORD == 4)
        oct[4*i] = num[i] & 0xff;
        oct[4*i + 1] = (num[i] >> 8) & 0xff;
        oct[4*i + 2] = (num[i] >> 16) & 0xff;
        oct[4*i + 3] = (num[i] >> 24) & 0xff;
#endif
    }

#if (WHICH_WORD == 1)
    if(rem)
        oct[hiword + 1] = num[hiword];
#elif (WHICH_WORD == 2)
    if(rem)
        oct[2*hiword + 1] = num[hiword] & 0xff;
#elif(WHICH_WORD == 4)
    if(rem > 0){
        oct[4*hiword] = num[hiword] & 0xff;
    }
    if(rem > 1){
        oct[4*hiword + 1] = (num[hiword] >> 8) & 0xff;
    }
    if(rem > 2){
        oct[4*hiword + 2] = (num[hiword] >> 16) & 0xff;
    }
#endif
    return hibyte;
}

int os2ip(char *oct, int len, Word *num){

    if(len > N_SZ){
        printf("message too long! %d doesn't fit in %d\n", len, N_SZ);
        return 0;
    }

    int hiword = len/WHICH_WORD;
    int rem = len-(hiword*WHICH_WORD);
    int i;

    for(i = 0; i < hiword; i++){
#if (WHICH_WORD == 1)
        num[i] = oct[i];
#elif (WHICH_WORD == 2)
        num[i] = ((Word)oct[2*i + 1] << 8) | oct[2*i];
#elif (WHICH_WORD == 4)
        num[i] = ((Word)oct[4*i + 3] << 24) | 
                 ((Word)oct[4*i + 2] << 16) |
                 ((Word)oct[4*i + 1] << 8 ) |
                 oct[4*i];
#endif
    }

#if (WHICH_WORD == 2)
    if(rem)
        num[hiword] = oct[2*hiword];
#elif (WHICH_WORD == 4)
    if(rem > 0){
        num[hiword] = oct[4*hiword];
    }
    if(rem > 1){
        num[hiword] |= (Word)oct[2*hiword + 1] << 8;
    }
    if(rem > 2){
        num[hiword] |= (Word)oct[2*hiword + 2] << 16;
    }
#endif
    return len;
}

