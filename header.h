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
#define DEBUG

typedef struct connections {
    int socket;
    int pid_server;
    int pid_client;
} CONNECTIONS;
