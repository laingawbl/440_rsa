#include "bignum.h"
void zerol(Word *n, int l){
    int i;
    for (i = 0; i < l; ++i){
        n[i] = 0;
    }
}

void zero(Word *n){
    zerol(n, N_SZ);
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
    if(pq)
        copy(q, pq);
    if(pr)
    copy(r, pr);
    return 0;
}

void split(Word *a, Word *hi, Word *lo){
    int i;
    for(i=N_SZ*2 - 1; i>=N_SZ; --i){
        hi[i-N_SZ] = a[i];
    }
    for(i=N_SZ-1; i>=0; --i){
        lo[i] = a[i];
    }
}

/*
    Long multiplication
*/
void mul(Word *a, Word *b, Word *hi, Word *lo){
    int i,j;

    Word wa = highbit(a)/W_SZ + 1;
    Word wb = highbit(b)/W_SZ + 1;
    DWord c;
    Word z[N_SZ*2];
    zerol(z, N_SZ*2);

    for(i = 0; i < wa; i++){
        c = 0;
        for(j = 0; j < wb; j++){
            c = a[i];
            c *= b[j];
            c += z[i+j];
            z[i+j] = c & W_MASK;
            c = (c >> W_SZ) & W_MASK;
        }
        z[i+wb] = c & W_MASK;
    }
    split(z, hi, lo);
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

void muls(Word *a, Word *b, Word *hi){
    return mul(a, b, hi, a);
}

/*
    MODULAR OPERATIONS
*/

/*
    an optimized inversion for the special case where
    (1) n is a power of 2 (of the form (100....0000)b)
    (2) r is odd

    returns n_inv, r_inv s.t.
    n(n_inv) + r(r_inv) = 1

    taken from Henry Warren's simplified version of the
    Stein GCD algorithm, from his note, "Montgomery Multiplication"
*/
void n_mod_r(Word *n, Word *r, Word *n_inv, Word *r_inv){
    Word u[N_SZ];
    Word v[N_SZ];
    Word alph[N_SZ];
    Word beta[N_SZ];
    Word c;

    load(u, 1);
    zero(v);
    copy(n, alph);
    copy(r, beta);

    while(!(zerop(n))){
        half(n);
        if(evenp(u)){
            half(u);
            half(v);
        } 
        else{
            c = adds(u, beta);
            half(u);
            set(N_BITS-1, c, u);

            half(v);
            adds(v, alph);
        }
    }
}

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
    slow non-Montgomery method
*/
void mul_mo(Word *a, Word *b, Word *m, Word *r){
    Word hi[N_SZ];
    Word lo[N_SZ];
    Word ms[N_SZ];
    Word c;
    zero(hi);
    zero(lo);
    copy(m, ms);
    
    mul(a, b, hi, lo);
    while(highbit(ms) != N_BITS - 1){
        twice(ms);
    }
    while(!(zerop(hi))){
        c = subs(lo, ms);
        subws(hi, c);
    }
    while(gte(lo, m)){
        while(highbit(ms) >= highbit(lo)){
            half(ms);
        }
        subs(lo, ms);
    }
    copy(lo, r);
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

    int i = ha/W_SZ;
    for(i; i >= 0; --i){
        if (a[i] == b[i])
            continue;
        if (a[i] < b[i])
            return 0;
        return 1;
    }
    return 1;
}

Word equ(Word *a, Word *b){
    Word ha = highbit(a);
    Word hb = highbit(b);
    
    if (ha != hb)
        return 0;

    int i = ha/W_SZ;
    for(i; i >= 0; --i){
        if (a[i] != b[i]){
            return 0;
        }
    }
    return 1;
}

Word half(Word *a){
    return lsrs(a, 1);
}

Word twice(Word *a){
    return lsls(a, 1);
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
