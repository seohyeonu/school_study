all: scoreProcessing scoreAnalyze

scoreProcessing: main.o
	gcc -o scoreProcessing main.o

main.o: main.c
	gcc -c main.c -o main.o

scoreAnalyze: scoreAnalyze.o
	gcc -o scoreAnalyze scoreAnalyze.o

scoreAnalyze.o: scoreAnalyze.c
	gcc -c scoreAnalyze.c -o scoreAnalyze.o

clean:
	rm -f main.o scoreAnalyze.o scoreProcessing scoreAnalyze

