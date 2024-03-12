
.PHONY: clean

time: time.c
	gcc -c copy.c
	gcc -c copy_block.c
	ar rcs libcopy.a copy.o
	ar rcs libcopy_block.a copy_block.o

	gcc -g -Wall time.c libcopy.a libcopy_block.a -o time

time_test: time.c
	make time
	make clean
	./time
	nano ./results/pomiar_zad_2.txt

clean: 
	rm copy.o libcopy.a copy_block.o libcopy_block.a

