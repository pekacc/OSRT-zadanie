#include <stdio.h>
#include <sys/socket.h>  
#include <sys/mman.h>
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
#include <sys/sem.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>

#define RECEIVE_BUF 100
#define MAX_CONNECTIONS 10
#define MIN_PORT_NUMBER 60000 //sockets creating from this port number
#define MAIN_SOCKET 61000
#define MAX_RECORDS 1000
#define MAX_BOT_CLIENTS 3 //no more than 9 !!!
//#define DEBUG
//#define CLIENT_DEBUG
#define CLIENT_TIMEOUT 60

//macros for client tasks
#define END_CONNECTION -1
#define MENU 0
#define ADD_RECORD 1
#define SHOW_ALL 2
#define MEAN 5
#define MAX_VALUE 6
#define MIN_VALUE 7
#define CLEAR_DATABASE 9

#define COMMANDS_LIST "List of commands:\n-1: end connection\n0: this list\n1: add record\n2: show all records\n5: show mean of salary\n6: show record with maximal salary\n7: show record with minimal salary\n9: clear database\n"

typedef struct connections {
    int port;
    pid_t pid_server;
    pid_t pid_client;
} CONNECTIONS;

typedef struct record {
    int ID;
    int age;
    int salary;
} RECORD;

typedef union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
} SEMUN;
