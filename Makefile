all: main

main: main.o funzioni.o

	gcc -o main main.o funzioni.o

main.o: main.c funzioni.h

	gcc -c -o main.o main.c

funzioni.o: funzioni.c funzioni.h

	gcc -c -o funzioni.o funzioni.c

clean: main main.o funzioni.o

	rm main main.o funzioni.o


