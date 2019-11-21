CC = gcc
CFLAGS = -I. -lpthread
DEPS = statistic.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ &< $(CFLAGS)

program: server.o statistic.o functions.o client.o requests.o
	@echo "linking"
	$(CC) server.o functions.o statistic.o -o server $(CFLAGS)
	$(CC) client.o functions.o statistic.o requests.o -o client $(CFLAGS)
	@echo "removing object files"
	rm *.o

server.o: 
	@echo "compiling server"
	$(CC) -c server.c $(CFLAGS)

functions.o:
	@echo "compiling functions"
	$(CC) -c functions.c $(CFLAGS)

statistic.o:
	@echo "compiling statistic"
	$(CC) -c statistic.c $(CFLAGS)

client.o:
	@echo "compiling client"
	$(CC) -c client.c $(CFLAGS)

requests.o:
	echo "compiling requests"
	$(CC) -c requests.c $(CFLAGS)


clean:
	rm server
	rm client
	rm *.o
