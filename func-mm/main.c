#include "monty.h"
#include<ctype.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

typedef struct _rsak_s {
    uint32_t mod[L];
    uint32_t exp[L];
} rsa_key;

void os2ip(const char *in, uint32_t out[L]){
    memcpy(out, in, L8);
}

void i2osp(const uint32_t in[L], char *out){
    memcpy(out, in, L8);
}

void rsa_edp(const char *in, char *out, const Mont mm, const uint32_t *e){
    uint32_t in_rep[L] = {0};
    uint32_t out_rep[L] = {0};
    uint32_t monty[LL] = {0};

    os2ip(in, in_rep);
    mm_conv(in_rep, &mm, monty);

    mm_exp(monty, e, (Lb-1), &mm);

    mm_redc(monty, &mm, out_rep);
    i2osp(out_rep, out);
}

void rsa_round_trip(FILE *in, FILE *out, const rsa_key k){
    Mont mm;
    mm_init(k.mod, &mm);
}

rsa_key getkey(FILE *keyfile){
    char a[3] = {0};
    char buf[L8] = {0};
    rsa_key k;

    fscanf(keyfile, "%c%c", a, a+1);
    if(a[0] != '0' || a[1] != '0'){
        exit(1);
    }

    int i = 0;
    while(fscanf(keyfile, "%c%c", a, a+1)){
        if(a[0] == '!' || a[1] == '!') break;
        if(i >= L8){
            printf("oh bollocks!\n");
            exit(1);
        }
        buf[i] = (char)strtoul(a, NULL, 16);
        printf("%d: %s\n", i, a);
        i++;
    }
    os2ip(buf, k.mod);

    i = 0;
    memset(buf, 0, L8);
    while((i < L8) && fscanf(keyfile, "%c%c", a, a+1)){
        buf[i] = (char)strtoul(a, NULL, 16);
        printf("%d: %s\n", i, a);
        i++;
    }
    os2ip(buf, k.exp);
}

int main(int argc, char *argv[]){
    int print_result = 0;
    char *keyfile = 0;
    char *infile = 0;
    char *outfile = 0;

    int c = 0;
    while((c = getopt(argc, argv, "k:i:o:p")) != -1){
        switch(c){
            case 'k':
                keyfile = optarg;
                break;
            case 'i':
                infile = optarg;
                break;
            case 'o':
                outfile = optarg;
                break;
            case 'p':
                print_result = 1;
                break;
            case '?':
                printf("usage: ./func_mm -k <keyfile> -i <input file> [-o output file] [-p]\n");
            default:
                return 1;
        }
    }
    if(!keyfile){
        printf("no keyfile specified!\n");
        return 1;
    }
    if(!infile){
        printf("no textfile specified!\n");
        return 1;
    }

    FILE * fk = fopen(keyfile, "r");
    FILE * ft = fopen(infile, "r");

    rsa_key kd = getkey(fk);
    declaim("modulus", kd.mod, L);
    declaim("private exp", kd.exp, L);
    rsa_key ke;
    mv(kd.mod, ke.mod);
    ke.exp[0] = 0x100001;

    return 0;
}

