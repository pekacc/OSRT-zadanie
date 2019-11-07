CC = gcc
CFLAGS = -I.
DEPS = statistic.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ &< $(CFLAGS)

program: main.c statistic.c
	$(CC) main.c statistic.c -o main $(CFLAGS)

clean:
	rm main
