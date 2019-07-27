CC=gcc
CFLAGS=-I. -O3 -pg
DEPS=octets.h bignum.h rsa.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o octets.o bignum.o rsa.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o main
