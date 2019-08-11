CC=arm-linux-gnueabi-gcc
CFLAGS=-I. -O3 -static
DEPS=monty.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

func_mm: main.o montyfunc.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o func_mm
