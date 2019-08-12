CC=arm-linux-gnueabi-gcc
CFLAGS=-I. -O3 -static -march=armv5
DEPS=monty.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

opt_mm: main.o monty.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o opt_mm
