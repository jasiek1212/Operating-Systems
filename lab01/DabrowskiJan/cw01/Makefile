CFLAGS= -g -Wall

.PHONY: all clean_static clean_dynamic clean_shared clean_libraries

all: static shared dynamic

static: main_static.c
	gcc -c collatz_library.c
	ar rcs libcollatz_library.a collatz_library.o

	gcc $(CFLAGS) main_static.c libcollatz_library.a -o main_stat

shared: main_static.c 
	gcc -fPIC -c collatz_library.c
	gcc -shared collatz_library.o -o libcollatz_library.so

	gcc $(CFLAGS) main_static.c -L. -lcollatz_library -Wl,-rpath=. -o main_shared

dynamic: main_dynamic.c
	gcc -fPIC -c collatz_library.c
	gcc -shared collatz_library.o -o libcollatz_library.so

	gcc $(CFLAGS) main_dynamic.c -o main_dynamic -ldl

clean_static:
	rm main_stat libcollatz_library.a collatz_library.o

clean_shared:
	rm main_shared libcollatz_library.so collatz_library.o

clean_dynamic:
	rm main_dynamic libcollatz_library.so collatz_library.o

clean_libraries:
	rm libcollatz_library.a collatz_library.o libcollatz_library.so

clean:
	rm main_stat libcollatz_library.a collatz_library.o main_shared libcollatz_library.so main_dynamic

test:
	./main_stat
	./main_dynamic
	./main_shared



