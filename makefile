CC = gcc
CFLAGS = -I. -lpthread -lrt
DEPS = statistic.h
ccgreen = @echo "\033[0;32m"
ccstand = @echo "\033[0m"

%.o: %.c $(DEPS)
	$(CC) -c -o $@ &< $(CFLAGS)

program: server.o statistic.o functions.o client.o requests.o
	@echo "linking"
	@$(CC) server.o functions.o statistic.o -o server $(CFLAGS)
	@$(CC) client.o functions.o statistic.o requests.o -o client $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)
	@echo "removing object files"
	@rm *.o
	$(ccgreen)
	@echo "done!"
	$(ccstand)

server.o: 
	@echo "compiling server"
	@$(CC) -c server.c $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)

functions.o:
	@echo "compiling functions"
	@$(CC) -c functions.c $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)

statistic.o:
	@echo "compiling statistic"
	@$(CC) -c statistic.c $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)

client.o:
	@echo "compiling client"
	@$(CC) -c client.c $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)

requests.o:
	@echo "compiling requests"
	@$(CC) -c requests.c $(CFLAGS)
	$(ccgreen)
	@echo "done!"
	$(ccstand)

clean:
	@echo "cleaning all files"
	@rm *.o
	@rm server
	@rm client
	$(ccgreen)
	@echo "done!"
	$(ccstand)
