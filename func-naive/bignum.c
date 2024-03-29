#include "bignum.h"

void zero(Word *n){
    int i;
    for (i = 0; i < N_SZ; ++i){
        n[i] = 0;
    }
}

void copy(Word *fr, Word *to){
    int i;
    for (i = 0; i < N_SZ; ++i){
        to[i] = fr[i];
    }
}

Word add(Word *a, Word *b, Word *r){
    int i;
    Word c = 0;

    for (i = 0; i < N_SZ; ++i){
        DWord z = (DWord)a[i] + (DWord)b[i] + c;
        r[i] = z & W_MASK;
        c = z >> W_SZ;
    }
    return c;
}

Word addw(Word *a, Word b, Word *r){
    int i;

    DWord z = (DWord)a[0] + b;
    r[0] = z & W_MASK;
    Word c = z >> W_SZ;

    // propagate the carry up
    for (i = 0; (i < N_SZ); i++){
        z = (DWord)a[i] + c;
        r[i] = z & W_MASK;
        c = z >> W_SZ;
    }
    return c;
}

Word sub(Word *a, Word *b, Word *r){
   int i;
   Word c = 0;

   for(i = 0; i < N_SZ; ++i){
        DWord z = (DWord)a[i] - (DWord)b[i] - c;
        r[i] = z & W_MASK;
        c = (z >> W_SZ) & 1;
   }
   return c;
}

Word subw(Word *a, Word b, Word *r){
    int i;
    Word res[N_SZ];
    copy(a, res);
    DWord z = (DWord)res[0] - b;
    res[0] = z & W_MASK;
    Word c = (z >> W_SZ) & 1;

    // propagate the subtraction carry up
    for (i = 0; (i < N_SZ) && c; i++){
        z = (DWord)res[i] - c;
        res[i] = z & W_MASK;
        c = (z >> W_SZ) & 1;
    }

    copy(res, r);
    return c;
}

/*
    basic long-division
*/
Word qdiv(Word *a, Word *b, Word *pq, Word *pr){
    if(zerop(b))
        return 1;

    Word q[N_SZ];
    Word r[N_SZ];
    zero(q);
    load(r, 0);
    
    int i;
    Word ha = highbit(a);

    for(i = ha; i >= 0; --i){
        twice(r);
        set(0, sel(i, a), r);
        if(gte(r, b)){
            subs(r, b);
            set(i, 1, q);
        }
    }
    copy(q, pq);
    copy(r, pr);
    return 0;
}

/*
    Peasant Method multiplication
    does NO bounds checking - you will get a garbage result if
        highbit(a) + highbit(b) > N_BITS
    but this will be communicated by returning the overflow as 1.
*/
Word mul(Word *a, Word *b, Word *r){
    Word ha = highbit(a);
    Word c = 0;

    int i;
    Word rz[N_SZ];
    zero(rz);
    Word bz[N_SZ];
    copy(b, bz);

    for(i = 0; i <= ha; i++){
        if(sel(i, a)){
            c |= adds(rz, bz);
        }
        twice(bz);
    }
    copy(rz, r);
    return c;
}

/*
    SELF-MODIFYING OPERATIONS
*/

Word adds(Word *a, Word *b){
    return add(a, b, a);
}

Word subs(Word *a, Word *b){
    return sub(a, b, a);
}

Word addws(Word *a, Word b){
    return addw(a, b, a);
}

Word subws(Word *a, Word b){
    return subw(a, b, a);
}

Word muls(Word *a, Word *b){
    return mul(a, b, a);
}

/*
    MODULAR OPERATIONS
*/

void add_mo(Word *a, Word *b, Word *m, Word *r){
    add(a, b, r);
    if(gte(r,m)) subs(r, m);
}

/*
    assuming a,b in [0, m), the lowest possible
    value for (a-b) is (-m)... so we just translate
    everything up by m, and then possibly a final
    reduce
*/
void sub_mo(Word *a, Word *b, Word *m, Word *r){
    Word d[N_SZ];
    zero(d);

    add(a, m, d);
    subs(d, b);
    if(gte(d,m)) subs(d, m);
    copy(d, r);
}

/*
    The same method as lmul, but with correction modulo m at each step.
    We shift the result rz to preserve the value of bz.
*/
void mul_mo(Word *a, Word *b, Word *m, Word *r){
    int i;
    Word rz[N_SZ];
    zero(rz);

    Word bz[N_SZ];
    copy(b, bz);
    
    for(i = N_BITS - 1; i >= 0; --i){ 
        twice(rz);
        if(gte(rz, m))
            subs(rz, m);
        if(sel(i, a))
            adds(rz, bz);
        if(gte(rz, m))
            subs(rz, m);
    }
    copy(rz, r);
}


Word lsl(Word *a, Word n, Word *r){
    int i;

    Word z = a[N_SZ - 1] >> (W_SZ - n);
    
    for(i = N_SZ - 1; i > 0; --i){
        r[i] = (a[i] << n) | (a[i-1] >> (W_SZ - n));
    }

    r[0] = a[0] << n;

    return z;
};

Word lsr(Word *a, Word n, Word *r){
    int i;

    Word z = a[0] << (W_SZ - n);

    for(i = 0; i < N_SZ - 1; i++){
        r[i] = (a[i] >> n) | (a[i+1] << (W_SZ - n));
    }

    r[N_SZ - 1] = a[N_SZ - 1] >> n;
};

Word lsls(Word *a, Word b){
    return lsl(a, b, a);
}

Word lsrs(Word *a, Word b){
    return lsr(a, b, a);
}

Word sel(Word p, Word *b){
    Word idx = p / W_SZ;
    Word bit = p % W_SZ;
    Word res = (b[idx] >> bit) & 1;
    return res;
}

void set(Word p, Word a, Word *b){
   Word idx = p / W_SZ;
   Word bit = p % W_SZ;
   b[idx] &= ~(a << bit);
   b[idx] |=  (a << bit);
}

void andb(Word *a, Word *b, Word *r){
    int i;
    for(i = 0; i < N_SZ; i++){
        r[i] = a[i] & b[i];
    }
}

void xorb(Word *a, Word *b, Word *r){
    int i;
    for(i = 0; i < N_SZ; i++){
        r[i] = a[i] ^ b[i];
    }
}

Word zerop(Word *a){
    int i;
    for (i = 0; i < N_SZ; i++){
        if(a[i] != 0) return 0;
    }
    return 1;
}

Word evenp(Word *a){
    return (!(a[0] & 1));
}

Word oddp(Word *a){
    return a[0] & 1;
}

Word highbit(Word *a){
    int i, j;
    for(i = N_SZ - 1; i >= 0; --i){
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

Word gte(Word *a, Word *b){
    Word ha = highbit(a);
    Word hb = highbit(b);

    if (ha > hb)
        return 1;
    if (ha < hb)
        return 0;

    int i;
    Word sa, sb;
    for(i = ha; i >= 0; --i){
        Word sa = sel(i, a);
        Word sb = sel(i, b);

        if (sa > sb)
            return 1;
        if (sa < sb)
            return 0;
    }
    return 1;
}

Word equ(Word *a, Word *b){
    Word ha = highbit(a);
    Word hb = highbit(b);
    
    if (ha != hb)
        return 0;

    int i;
    for(i = ha - 1; i >= 0; --i){
        if (sel(i, a) != sel(i,b)){
            return 0;
        }
    }
    return 1;
}

void half(Word *a){
    lsrs(a, 1);
}

void twice(Word *a){
    lsls(a, 1);
}

void load(Word *a, Word val){
    zero(a);
    addws(a, val);
}

void print_num(Word *a){
    int i;
    if(N_SZ > 2) {
        i = highbit(a) / W_SZ;
    }
    else {
        i = N_SZ - 1;
    }
    for(i; i >= 0; --i){
        printf("%#10.8x ", a[i]);
        if(!(i % 8))
            printf("\n");
    }
}
