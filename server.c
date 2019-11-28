#include "header.h"
#include "statistic.h"
#include "functions.h"

void sigint();
void sigint_cli();
void sigusr1();
void sigchld();
void processing(int);
void * status_thread();
int sock_desc, act_sock_desc, connections_number, shmid, shmid_sem, records_number, client_action;
pthread_t thread_id;
sem_t *semaphore;
RECORD *records;
CONNECTIONS connections[MAX_CONNECTIONS];

int main() {
    signal(SIGCHLD, sigchld);
    signal(SIGINT, sigint);
    connections_number = 0;
    records_number = 0;

    //creating status thread
    printf("Creating status thread    ");
    if ((pthread_create(&thread_id, NULL, status_thread, NULL)) < 0) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    printf("\033[1;32m[OK]\033[0m\n");


    //connections = malloc(sizeof(CONNECTIONS)*MAX_CONNECTIONS);

    //creating shared memory
    printf("Creating shared memory    ");
    if ((shmid = shmget(getpid(), (MAX_RECORDS + 1)*sizeof(RECORD), IPC_CREAT | 0666)) < 0) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    printf("\033[1;32m[OK]\033[0m\n");

    if ((records = shmat(shmid, NULL, 0)) == (RECORD *) -1) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    records[MAX_RECORDS].ID = 0;

    //creating semaphore
    printf("Creating semaphore    ");
    if ((shmid_sem = shmget(getpid()+1, sizeof(sem_t), IPC_CREAT | 0666)) < 0) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }

    if ((semaphore = shmat(shmid_sem, NULL, 0)) == (sem_t *) -1) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    if ((sem_init(semaphore, 1, 1)) != 0) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    printf("\033[1;32m[OK]\033[0m\n");

 
    //creating socket
    printf("Creating socket    ");
    sock_desc = open_socket(61000);
    if(sock_desc < 0) {
        printf("\033[1;31m[ERROR]\033[0m\n");
        sigint();
    }
    printf("\033[1;32m[OK]\033[0m\n");
    char buf[100];

    printf("\033[1;32m"); printf("Server is up!\n");
    printf("\033[0m");
    while(1) {
        #ifdef DEBUG
        printf("Waiting for connection\n");
        #endif

        act_sock_desc = listen_socket(sock_desc);
        int actual_connection = connections_number;
        connections_number++;
            
        int pid_connecting = receive_int(act_sock_desc);
        if(pid_connecting == -1) { //No message
            continue;
        }
        else if(pid_connecting == 0) { //Close from client side
            #ifdef DEBUG
            printf("Ending connection!\n");
            #endif
            close(act_sock_desc);
            continue;
        }
        connections[actual_connection].pid_client = pid_connecting;
        #ifdef DEBUG
        printf("Received: %d\n", pid_connecting);
        #endif
        
        int port;
        int new_sock_desc = new_socket(&port); //creating new socket for new proccess
        printf("New port is: %d\n",port);
        connections[actual_connection].port = port;
        send_int(act_sock_desc, port);
        close(act_sock_desc);

        int pid;
        if((pid = fork()) == 0) {
            processing(new_sock_desc);
        } else {
            connections[actual_connection].pid_server = pid;
        }
    
    }
    close(sock_desc);
}

void processing(int my_socket) { //function to fulfill client tasks
    signal(SIGINT, sigint_cli);
    signal(SIGUSR1, sigusr1);
    sock_desc = my_socket; 
    act_sock_desc = listen_socket(sock_desc);
    
    //creating timer
    timer_t timer;
    timer = create_my_timer(SIGUSR1);
    set_my_timer(timer,CLIENT_TIMEOUT,CLIENT_TIMEOUT);

    int action;
    client_action = 0;

    while(1) {
        action = receive_int(act_sock_desc); //type of task from client
        client_action = 1;
        #ifdef DEBUG
        printf("Received command %d\n%d: Waiting for semaphore\n", action, getpid());
        #endif
        sem_wait(semaphore); //waiting to work with shared memory
        #ifdef DEBUG
        printf("%d: Semaphore passed\n", getpid());
        #endif
        records_number = records[MAX_RECORDS].ID;
        switch(action) {
        case ADD_RECORD:
            records[records_number].ID = receive_int(act_sock_desc);
            records[records_number].age = receive_int(act_sock_desc);
            records[records_number].salary = receive_int(act_sock_desc);

            if (records_number >= (MAX_RECORDS - 1)) {
                #ifdef DEUG
                printf("Database is full!\n");
                #endif
                send_int(act_sock_desc, 0);
                break;
            }
            if(records[records_number].ID >= 0 && records[records_number].age >= 0 && records[records_number].salary >= 0) {
                #ifdef DEBUG
                printf("New record is: ID: %d, age: %d, salary: %d\n",records[records_number].ID, records[records_number].age, records[records_number].salary);
                #endif
                records_number++;
                send_int(act_sock_desc, 1);
            } else {
                send_int(act_sock_desc, 0);
            }
        break;

        case SHOW_ALL:
            send_int(act_sock_desc, records_number);
            for(int i = 0; i < records_number; i++) {
                send_int(act_sock_desc, records[i].ID);
                send_int(act_sock_desc, records[i].age);
                send_int(act_sock_desc, records[i].salary);
            }
            send_int(act_sock_desc,1);
        break;

        case MEAN:
            send_int(act_sock_desc, mean(records));
            send_int(act_sock_desc, 1);
        break;

        case END_CONNECTION: 
            sem_post(semaphore);
            #ifdef DEBUG
            printf("%d: Semaphore is free\n", getpid());
            #endif
            #ifdef DEBUG
            printf("Ending connection on process no.: %d!\n", getpid());
            #endif
            sigint_cli();
        break;

        case CLEAR_DATABASE:
            records_number = 0;
            send_int(act_sock_desc, 1);
        break;

        default:
            send_int(act_sock_desc, 0);
        break;

        }
        records[MAX_RECORDS].ID = records_number;
        sem_post(semaphore);
        #ifdef DEBUG
        printf("%d: Semaphore is free\n", getpid());
        #endif
    }
}

void sigint() {
    signal(SIGINT,sigint); //reset signal    
    for(int i = 0; i < connections_number; i++) {
        kill(connections[i].pid_server, SIGINT);
        kill(connections[i].pid_client, SIGHUP);
        printf("SIGHUP to client with PID %d\n", connections[i].pid_client);
    }
    printf("\nClosing main socket! + %d kid processes    ", connections_number);
    close(act_sock_desc);
    close(sock_desc);
    printf("\033[1;32m[OK]\033[0m\n");

    printf("Destroying shared memory    ");
    shmctl(shmid,IPC_RMID,0);
    printf("\033[1;32m[OK]\033[0m\n");

    printf("Destroying semaphore    ");
    sem_destroy(semaphore);
    shmctl(shmid_sem,IPC_RMID,0);
    printf("\033[1;32m[OK]\033[0m\n");

    exit(0);
}

void sigint_cli() {
    signal(SIGINT,sigint); //reset signal    
    #ifdef DEBUG
    printf("Proces no.: %d Closing sockets!\n", getpid());
    #endif
    close(act_sock_desc);
    close(sock_desc);
    records[MAX_RECORDS].age = getpid();
    exit(0);
}

void sigusr1() {
    signal(SIGUSR1, sigusr1); //reset signal
    if(client_action) {
        #ifdef DEBUG
        printf("Client timeout test passed\n");
        #endif
        client_action = 0;
        return;
    } else {
        printf("Client timeout, closing server\n");
        send_int(act_sock_desc, -1);
        sigint_cli();
    }
}

void sigchld() {
    signal(SIGCHLD, sigchld); //reset signal
    remove_connection(connections, connections_number, records[MAX_RECORDS].age);
    connections_number--;
    #ifdef DEBUG
    printf("Table of connections:\n");
    for(int i = 0; i < connections_number; i++) {
        printf("PID server: %d PID client: %d\n", connections[i].pid_server, connections[i].pid_client);
    }
    printf("\n\n\n");
    #endif
}

void * status_thread() {
    while(1) {
        int command;
        scanf("%d", &command);
        switch(command) {
            case 1:
                printf("There are %d active connections: \n", connections_number);
                for (int i = 0; i < connections_number; i++) {
                    printf("Conneciton %d: server PID: %d, client PID: %d\n", i, connections[i].pid_server, connections[i].pid_client);
                }
            break;
            case 2:
                printf("There are %d records in memory:\n", records[MAX_RECORDS].ID);
                for(int i = 0; i < records[MAX_RECORDS].ID; i++) {
                    printf("Record no. %d: ID: %d, age: %d, salary: %d\n", i, records[i].ID, records[i].age, records[i].salary);
                }
            break;
        }
    }
}
