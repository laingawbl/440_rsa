CC=gcc
CFLAGS=-I. -O3
DEPS=octets.h bignum.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

rsa: octets.o bignum.o rsa_sam.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o
