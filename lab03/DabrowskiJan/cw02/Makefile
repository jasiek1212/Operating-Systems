
.PHONY: all clean test_read

readDir: readDir.c
	gcc -g -Wall readDir.c -o readDir

test_read: readDir.c
	make readDir
	./readDir

