.PHONY: all clean

all: 
	make processes
	./processes $(ARGS)


processes: processes.c
	gcc -g -Wall processes.c -o processes

clean:
	rm processes