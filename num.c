#include "num.h"

void zero(WORD *n){
    int i;
    for (i = 0; i < SZ_NUM; ++i){
        n[i] = 0;
    }
}

void copy(WORD *fr, WORD *to){
    int i;
    for (i = 0; i < SZ_NUM; ++i){
        to[i] = fr[i];
    }
}

WORD add_nn(WORD *a, WORD *b, WORD *r){
    int i;
    WORD c = 0;

    for (i = 0; i < SZ_NUM; ++i){
        DWORD z = (DWORD)a[i] + (DWORD)b[i] + c;
        r[i] = z & W_MASK;
        c = z >> W_SZ;
    }
    return c;
}

WORD add_nw(WORD *a, WORD b, WORD *r){
    int i;

    DWORD z = (DWORD)a[0] + b;
    r[0] = z & W_MASK;
    WORD c = z >> W_SZ;

    // propagate the carry up
    for (i = 0; (i < SZ_NUM) && c; i++){
        z = (DWORD)a[i] + c;
        r[i] = z & W_MASK;
        c = z >> W_SZ;
    }
    return c;
}

WORD sub_nn(WORD *a, WORD *b, WORD *r){
   int i;
   WORD c = 0;

   for(i = 0; i < SZ_NUM; ++i){
        DWORD z = (DWORD)a[i] - (DWORD)b[i] - c;
        r[i] = z & W_MASK;
        c = (z >> W_SZ) & 1;
   }
   return c;
}

WORD sub_nw(WORD *a, WORD b, WORD *r){
    int i;

    DWORD z = (DWORD)a[0] - b;
    r[0] = z & W_MASK;
    WORD c = (z >> W_SZ) & 1;

    // propagate the subtraction carry up
    for (i = 0; (i < SZ_NUM) && c; i++){
        z = (DWORD)a[i] - c;
        r[i] = z & W_MASK;
        c = (z >> W_SZ) & 1;
    }
    return c;
}

void add_mo(WORD *a, WORD *b, WORD *m, WORD *r){
    
    WORD d[SZ_NUM];
    WORD e[SZ_NUM];
    
    zero(d);
    zero(e);
    
    WORD c_1 = add_nn(a, b, d);
    WORD c_2 = sub_nn(d, m, e);
    if (c_1 == c_2){
        copy(e, r);
    }
    else{
        copy(d, r);
    }
}

void sub_mo(WORD *a, WORD *b, WORD *m, WORD *r){
    WORD d[SZ_NUM];
    WORD e[SZ_NUM];

    zero(d);
    zero(e);

    WORD c_1 = sub_nn(a, b, d);
    WORD c_2 = add_nn(d, m, e);
    
    if (c_1 == c_2){
        copy(e, r);
    }
    else{
        copy(d, r);
    }
}

WORD lsl_b(WORD *a, WORD n, WORD *r){
    int i;

    WORD z = a[SZ_NUM - 1] >> (W_SZ - n);
    
    for(i = SZ_NUM - 1; i > 0; --i){
        r[i] = (a[i] << n) | (a[i-1] >> (W_SZ - n));
    }

    r[0] = a[0] << n;

    return z;
};

WORD lsr_b(WORD *a, WORD n, WORD *r){
    int i;

    WORD z = a[0] << (W_SZ - n);

    for(i = 0; i < SZ_NUM - 1; i++){
        r[i] = (a[i] >> n) | (a[i+1] << (W_SZ - n));
    }

    r[SZ_NUM - 1] = a[SZ_NUM - 1] >> n;
};

WORD sel(WORD p, WORD *b){
    WORD idx = p / W_SZ;
    WORD bit = p % W_SZ;
    return (b[idx] >> bit) & 1;
}

void set(WORD p, WORD a, WORD *b){
   WORD idx = p / W_SZ;
   WORD bit = p % W_SZ;
   b[idx] &= ~(p << bit);
   b[idx] |=  (p << bit);
}

void and_nn(WORD *a, WORD *b, WORD *r){
    int i;
    for(i = 0; i < SZ_NUM; i++){
        r[i] = a[i] & b[i];
    }
}

void xor_nn(WORD *a, WORD *b, WORD *r){
    int i;
    for(i = 0; i < SZ_NUM; i++){
        r[i] = a[i] ^ b[i];
    }
}

WORD zerop(WORD *a){
    int i;
    for (i = 0; i < SZ_NUM; i++){
        if(a[i] != 0) return 0;
    }
    return 1;
}

WORD highbit(WORD *a){
    int i, j;
    for(i = SZ_NUM - 1; i >= 0; --i){
        if(a[i] == 0){
            continue;
        }
        for(j = W_SZ - 1; j >= 0; --j) {
            if((a[i] >> j) & 1){
                return (i * W_SZ) + j;
            }
        }
    }
    return 0;
}

/*
    returns:
    a >= b       1
    a <  b       0
*/
WORD gte(WORD *a, WORD *b){
    WORD ha = highbit(a);
    WORD hb = highbit(b);
    
    if (ha > hb)
        return 1;
    if (ha < hb)
        return 0;

    int i;
    WORD sa, sb;
    for(i = ha - 1; i >= 0; --i){
        WORD sa = sel(i, a);
        WORD sb = sel(i, b);

        if (sa > sb)
            return 1;
        if (sa < sb)
            return 0;
    }
    return 1;
}
