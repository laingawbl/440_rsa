CC=arm-linux-gnueabi-gcc
CFLAGS=-I. -O3 -pg -static
DEPS=octets.h bignum.h rsa.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o octets.o bignum.o rsa.o
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o main
