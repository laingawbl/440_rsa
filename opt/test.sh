#!/bin/sh
die() { echo "$*" 1>&2; exit 1; }

(which openssl) || die 'test script requires openssl'
(which awk) || die 'test script requires awk'

rm enc result
touch enc result

# we do this because the alternative is parsing the PEM into ASN.1 form and extracting the modulus and key
# the C program assumes the public exponent is F4 (65537)

echo "generating key using openssl..."
openssl genrsa -F4 2048 | 
openssl rsa -text -noout | 
awk '/modulus:/{m=1;next}/publicExponent:/{print"!!";m=0}/privateExponent:/{p=1;next}/prime1:/{p=0}p;m' | 
tr -d ":[:blank:]\n" > monty_test_key

echo "executing mm_rsa with that key..."
echo "encrypting corpus..."
time -v qemu-arm ./opt_mm -k monty_test_key -i corpus_small -o enc 
echo "done."
echo "decrypting..."
time -v qemu-arm ./opt_mm -k monty_test_key -i enc -o result -d
echo "done."
echo "diff between original text and result follows..."
diff -war corpus_small result

