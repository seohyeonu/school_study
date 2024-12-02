
CC = gcc -std=c99


# 운영체제에 따라 링크 라이브러리 설정
ifeq ($(OS),Windows_NT)
    LIBS = -L. -lpdcurses
else
    LIBS = -lncurses
endif

# 빌드 규칙

viva: main.o
	$(CC) -o viva main.o $(LIBS)

main.o: main.c
	$(CC) -c main.c -o main.o

clean:
	rm -f *.o
	rm -f vite
