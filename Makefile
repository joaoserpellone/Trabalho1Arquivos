all:
	gcc -o programaTrab *.c -std=c99
clean:
	rm *.o
run:
	./programaTrab
