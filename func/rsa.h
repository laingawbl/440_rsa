#ifndef __RSA_H__
#define __RSA_H__

#include "bignum.h"
#include "octets.h"

#define EXP_DEFAULT 65537

typedef struct _rsa_pub_s {
    uint32_t n[_N_INTERNAL_LEN];
    uint32_t e[_N_INTERNAL_LEN];
} rsa_pub;

typedef struct _rsa_pri_s {
    uint32_t n[_N_INTERNAL_LEN];
    uint32_t d[_N_INTERNAL_LEN];
} rsa_pri;

/*
    generate an RSA keypair with an N_BITS-length modulus n, and a small fixed
    public exponent (exp).

    Keeping the public-knowledge exponent (exp) small (in this case, within 
    one uint32_t) ensures that the secret exponent d will large - at least 
    (n / exp) - improving security. 
    
    Popular choices for (exp) are the first four Fermat primes:

        {3, 7, 257, 65537}
*/
void genkeys(uint32_t exp, uint32_t mr_rounds, rsa_pub *ke, rsa_pri *kd);

/*
    transform a symbol of length len, up to SYM_SZ, into
    a SYM_SZ cipher symbol, using public key ke:

    m -> os2ip() -> rsaep(ke) -> i2osp() -> c

    see octets.h for SYM_SZ: it should be N_BITS/8, so a 2048-bit system will
    operate on 256-byte symbols.
    
    returns the number of bytes read from the symbol, from 0 to len.
*/
int rsa_enc(rsa_pub *ke, char *m, int len, char *c);

/*
    transform a SYM_SZ cipher symbol back into a plaintext symbol, using
    private key kd:

    c -> os2ip() -> rsadp(kd) -> i2osp() -> m

    returns the number of bytes written to the plaintext symbol.
*/
int rsa_dec(rsa_pri *kd, char *c, char *m);

void load_miller_rabin(uint32_t k, uint32_t *p);
void load_random(uint32_t max, uint32_t *n);
#endif
