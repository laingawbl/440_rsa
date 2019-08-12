#!/bin/sh
die() { echo "$*" 1>&2; exit 1; }

(which gprof) || die 'test script requires gprof'
(which openssl) || die 'test script requires openssl'
(which qemu-arm) || die 'test script requires qemu-arm'

rm enc result gmon.out.*
touch enc result

# we do this because the alternative is parsing the PEM into ASN.1 form and extracting the modulus and key
# the C program assumes the public exponent is F4 (65537)

for x in `seq 1 10`; do
    echo "run $x"
    openssl genrsa -F4 2048 | openssl rsa -text -noout | 
    awk '/modulus:/{m=1;next}/publicExponent:/{print"!!";m=0}/privateExponent:/{p=1;next}/prime1:/{p=0}p;m' | 
    tr -d ":[:blank:]\n" > monty_test_key
    qemu-arm ./opt_mm -k monty_test_key -i corpus -o enc 
    mv gmon.out gmon.out.$x
done

gprof -s opt_mm gmon.out.*
gprof opt_mm gmon.sum > optimised.sum.stats

