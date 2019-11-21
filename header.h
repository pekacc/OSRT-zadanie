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
#include <sys/types.h>
#include <sys/shm.h>

#define RECEIVE_BUF 100
#define MAX_CONNECTIONS 10
#define MIN_PORT_NUMBER 60000 //sockets creating from this port number
#define MAX_RECORDS 1000
#define DEBUG

//macros for client tasks
#define ADD_RECORD 1
#define SHOW_ALL 2

typedef struct connections {
    int port;
    int pid_server;
    int pid_client;
} CONNECTIONS;

typedef struct record {
    int ID;
    int age;
    int salary;
} RECORD;
