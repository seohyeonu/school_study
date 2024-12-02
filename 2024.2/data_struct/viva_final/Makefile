CC = gcc -std=c99

ifeq ($(OS),Windows_NT)
    LIBS = -L. -lpdcurses
else
    LIBS = -lncurses
endif

viva: viva.o
	$(CC) -o viva viva.o $(LIBS)

viva.o: viva.c
	$(CC) -c viva.c -o viva.o

clean:
	rm -f *.o viva
	del /q *.o viva.exe
