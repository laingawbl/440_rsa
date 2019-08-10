#!/bin/sh
die() { echo "$*" 1>&2; exit 1; }

(which openssl) || die 'test script requires openssl';
(which awk) || die 'test script requires awk';


# we do this because the alternative is parsing the PEM into ASN.1 form and extracting the modulus and key
# the C program assumes the public exponent is F4 (65537)

echo "generating key using openssl...";
openssl genrsa -F4 2048 | 
openssl rsa -text -noout | 
awk '/modulus:/{m=1;next}/publicExponent:/{print"!!";m=0}/privateExponent:/{p=1;next}/prime1:/{p=0}p;m' | 
tr -d ":[:blank:]\n" > monty_test_key

echo "executing mm_rsa with that key..."
./cmake_build_debug/func_mm -k monty_test_key -t corpus
