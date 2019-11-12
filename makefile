CC = gcc
CFLAGS = -I. -lpthread
DEPS = statistic.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ &< $(CFLAGS)

program: main.o statistic.o functions.o
	@echo "linking"
	$(CC) main.o functions.o statistic.o -o main $(CFLAGS)

main.o: 
	@echo "compiling main"
	gcc -c main.c $(CFLAGS)

functions.o:
	@echo "compiling functions"
	gcc -c functions.c $(CFLAGS)

statistic.o:
	@echo "compiling statistic"
	gcc -c statistic.c $(CFLAGS)

clean:
	rm main
	rm functions.o
	rm main.o
	rm statistic.o
