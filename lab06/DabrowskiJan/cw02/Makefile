.PHONY: all clean

all: catcher sender

sender: sender.c
	gcc -g -Wall sender.c -o sender

catcher: catcher.c
	gcc -g -Wall catcher.c -o catcher

clean: 
	rm catcher sender catcher_to_sender sender_to_catcher