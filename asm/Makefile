CC=arm-linux-gnueabi-gcc
CFLAGS=-I. -O3 -static -pg
DEPS=monty.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

asm_mm: main.o monty.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o opt_mm
