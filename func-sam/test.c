#include "bignum.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

void aeq(Word *a, Word *b, const char *msg){
    if(!(equ(a,b))){
        printf("assert failed!:%s\n", msg);
        print_num(a);
        printf("\t!=\n");
        print_num(b);
        exit(1);
    }
}

void test_add(){
    Word e[N_SZ];
    Word p[N_SZ];
    Word q[N_SZ];

    load(e,1);
    
    load(p,0);
    load(q,1);
    add(p,q,q);
    aeq(e,q, "0+1");

    load(p,0);
    load(q,1);
    add(q,p,q);
    aeq(e,q, "1+0");

    load(e,257);

    load(p,100);
    load(q,157);
    add(p,q,q);
    aeq(e,q, "100+157");

    load(p,100);
    load(q,157);
    add(q,p,q);
    aeq(e,q, "157+100");

    zero(e);
    e[N_SZ-1] = (1 << (W_SZ-1));
    e[0] = 1;
   //e = 2^N_BITS + 1

    zero(p);
    zero(q);
    p[N_SZ-1] = (1 << (W_SZ-2));
    //p = (1/2) 2^N_BITS
    q[N_SZ-1] = (1 << (W_SZ-2));
    q[0] = 1;

    add(q,p,q);
    aeq(e,q, "2^(n-1) + (2^(n-1) + 1)");

    zero(e);
    zero(q);
    zero(p);
    p[N_SZ-1]=W_MASK;
    q[N_SZ-1]=1;    
    
    Word ovf = add(q,p,q);

    aeq(e,q, "overflow - val");
    
    load(q,ovf);
    load(e, 1);
    aeq(e,q, "overflow - carry flag");
}

int main(){
    printf("testing, N_BITS=%u, N_SZ=%u\n", N_BITS, N_SZ);

    test_add();

    return 0;
}
