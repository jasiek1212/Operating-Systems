.PHONY: clean all

all:
	make program
	./program .

program: program.c
	gcc -g -Wall program.c -o program

clean:
	rm program