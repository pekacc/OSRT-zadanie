CC = gcc
CFLAGS = -I. -lpthread -lrt
ccgreen = @echo -n "\033[0;32m"
ccstand = @echo -n "\033[0m"


all: server.o statistic.o functions.o client.o requests.o server.c client.c
	@echo "linking"
	@$(CC) server.o functions.o statistic.o -o server $(CFLAGS)
	@$(CC) client.o functions.o statistic.o requests.o -o client $(CFLAGS)
	@echo -n "linking "
	$(ccgreen)
	@echo "done!"
	$(ccstand)

server.o: server.c
	@echo "compiling server"
	@$(CC) -c server.c $(CFLAGS)
	@echo -n "server "
	$(ccgreen)
	@echo "done!"
	$(ccstand)

functions.o: functions.c
	@echo "compiling functions"
	@$(CC) -c functions.c $(CFLAGS)
	@echo -n "functions "
	$(ccgreen)
	@echo "done!"
	$(ccstand)

statistic.o: statistic.c
	@echo "compiling statistic"
	@$(CC) -c statistic.c $(CFLAGS)
	@echo -n "statistic "
	$(ccgreen)
	@echo "done!"
	$(ccstand)

client.o: client.c
	@echo "compiling client"
	@$(CC) -c client.c $(CFLAGS)
	@echo -n "client "
	$(ccgreen)
	@echo "done!"
	$(ccstand)

requests.o: requests.c
	@echo "compiling requests"
	@$(CC) -c requests.c $(CFLAGS)
	@echo -n "requests "
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
