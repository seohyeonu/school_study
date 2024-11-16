CC = gcc -std=c99

vite: main.o
	$(CC) -o viva main.o -lncurses
main.o: main.c
	$(CC) -c main.c -o main.o

clean:
	rm -f *.o
	rm -f viva
