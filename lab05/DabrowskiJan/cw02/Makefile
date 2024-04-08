.PHONY: all clean run_catcher

all:
	make sender
	make catcher
	make run_catcher

sender: sender.c
	gcc -g -Wall sender.c -o sender 

catcher: catcher.c 
	gcc -g -Wall catcher.c -o catcher

run_catcher:
	./catcher &

clean:
	rm sender catcher