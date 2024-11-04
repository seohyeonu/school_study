CC = gcc -std=c99

vite: main.o
	$(CC) -o vite main.o -lncurses
main.o: main.c
	$(CC) -c main.c -o main.o

clean:
	rm -f *.o
	rm -f vite
