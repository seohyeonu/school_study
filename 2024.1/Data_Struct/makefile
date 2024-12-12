CC = gcc
calculator: bst.o calc.o main.o
	gcc -o calculator bst.o calc.o main.o -lm

bst.o: bst.c
	gcc -c bst.c -o bst.o

calc.o: calc.c
	gcc -c calc.c -o calc.o

main.o: main.c
	gcc -c main.c -o main.o

clean:
	rm *.o calulator
