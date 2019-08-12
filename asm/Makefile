CC=arm-linux-gnueabi-gcc -march=armv5
CFLAGS=-I. -O3 -static -pg
DEPS=monty.h

%.o: %.s $(DEPS)
	$(CC) -o $@ -c $< $(CFLAGS)

asm_mm: main.o monty.o 
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm *.o asm_mm
