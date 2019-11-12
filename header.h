#include <stdio.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <stdio.h>  
#include <string.h> 
#include <stdlib.h>  
#include <arpa/inet.h> 
#include <unistd.h>
#include <string.h> 
#include <signal.h>
#include <sys/wait.h>

#define RECEIVE_BUF 100
#define MAX_CONNECTIONS 10
#define MIN_PORT_NUMBER 60000 //sockets creating from this port number
#define DEBUG

typedef struct connections {
    int port;
    int pid_server;
    int pid_client;
} CONNECTIONS;
