#include "octets.h"

/*
    if you know the contents of bignum will fit inside an
    unsigned long long, use this
*/
unsigned long long to_dec(Word *a){
    return (((unsigned long long)a[1] << W_SZ) | a[0]);
}

int u82osp(Word *num, int upper, char *oct){
    int i;
    for(i = 0; i < upper; i++){
        oct[i] = num[i];
    }
    return upper;
}

int u162osp(Word *num, int upper, char *oct){
    int i;
    for(i = 0; i < upper; i++){
        if(i%2 == 0)
            oct[i] =  num[i/2]       & 0xff;
        else
            oct[i] = (num[i/2] >> 8) & 0xff;
    }
    return upper;
}

int u322osp(Word *num, int upper, char *oct){
    int i;
    for(i = 0; i < upper; i++){
        switch(i%4){
        case 0:
            oct[i] =  num[i/4]        & 0xff;
            break;
        case 1:
            oct[i] = (num[i/4] >> 8)  & 0xff;
            break;
        case 2:
            oct[i] = (num[i/4] >> 16) & 0xff;
            break;
        case 3:
            oct[i] = (num[i/4] >> 24) & 0xff;
       }
    }
    return upper;
}

int i2osp(Word *num, int len, char *oct){
    int hibyte = (highbit(num)+7)/8;
    if(hibyte > len){
        printf("number too long! %d bytes cannot fit into %d\n",
            hibyte, len);
        return 0;
    }

    if(WHICH_WORD == 1)
        return u82osp(num, hibyte, oct);
    else if (WHICH_WORD == 2)
        return u162osp(num, hibyte, oct);
    else if (WHICH_WORD == 4)
        return u322osp(num, hibyte, oct);
    else
        return 0;
}

int os2u8p(char *oct, int upper, Word *num){
    int i;
    for(i=0; i<upper; i++){
        num[i] = oct[i];
    }
    return upper;
}

/*
     0   2   4   6
    [a b c d e f g ...] =>
    [ba][dc][fe][..g]..
     0       2
*/

int os2u16p(char *oct, int upper, Word *num){
    int i;
    Word g;
    for(i=0; i<upper; i++){
        if(i%2 == 0){
            num[i/2] = oct[i];
        }
        else{
            g = oct[i];
            num[i/2] += (g << 8);
        }
    }
    return upper;
}

int os2u32p(char *oct, int upper, Word *num){
    int i;
    Word g;
    for(i=0; i<upper; i++){
        switch(i%4){
            case 0:
                num[i/4] = oct[i];
                break;
            case 1:
                g = oct[i];
                num[i/4] += (g << 8);
                break;
            case 2:
                g = oct[i];
                num[i/4] += (g << 16);
                break;
            case 3:
                g = oct[i];
                num[i/4] += (g << 24);
        }
    }
    return upper;
}

int os2ip(char *oct, int len, Word *num){

    if(len > SYM_SZ){
        printf("message too long! %d cannot fit in %d bytes\n", len, SYM_SZ);
        return 0;
    }

    if(WHICH_WORD == 1)
        return os2u8p(oct, len, num);
    else if (WHICH_WORD == 2)
        return os2u16p(oct, len, num);
    else if (WHICH_WORD == 4)
        return os2u32p(oct, len, num);
    else
        return 0;

}

