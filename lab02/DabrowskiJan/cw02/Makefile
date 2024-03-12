
.PHONY: all clean test

readDir: readDir.c
	gcc -g -Wall readDir.c -o readDir

test_read: readDir.c
	make readDir
	./readDir

