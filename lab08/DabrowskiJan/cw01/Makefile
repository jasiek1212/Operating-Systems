.PHONY: all clean

all: printer.c user.c
	make printer
	make user

printer: printer.c
	gcc -g -Wall printer.c -o printer

user: user.c
	gcc -g -Wall user.c -o user

clean:
	rm user printer