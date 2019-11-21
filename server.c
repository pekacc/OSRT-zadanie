#include "header.h"
#include "statistic.h"
#include "functions.h"

void sigint();
void sigint_cli();
void processing(int);
int sock_desc, act_sock_desc, connections_number, shmid, records_number;
RECORD *records;
CONNECTIONS connections[MAX_CONNECTIONS];

int main() {
    signal(SIGINT, sigint);
    connections_number = 0;
    records_number = 0;

    //creating shared memory
    if ((shmid = shmget(getpid(), (MAX_RECORDS + 1)*sizeof(RECORD), IPC_CREAT | 0666)) < 0) {
        printf("Error in creating shared memory\n");
        exit(-1);
    }

    if((records = shmat(shmid, NULL, 0)) == (RECORD *) -1) {
        printf("Error in creating shared memory\n");
        exit(-1);
    }
    records[MAX_RECORDS].ID = 0;

    //creating socket
    sock_desc = open_socket(61000);
    if(sock_desc < 0) {
        printf("Socker error!\n");
        exit(-1);
    }
    char buf[100];
    #ifdef DEBUG
    printf("Server is up\n");
    #endif
    while(1) {
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
    
        #ifdef DEBUG
        printf("Waiting for another connection\n");
        #endif
    }
    close(sock_desc);
}

void processing(int my_socket) { //function to fulfill client tasks
    signal(SIGINT, sigint_cli);
    sock_desc = my_socket; 
    act_sock_desc = listen_socket(sock_desc);
    int action;
    while(1) {
        action = receive_int(act_sock_desc); //type of task from client
        #ifdef DEBUG
        printf("Received %d\n", action);
        #endif
        records_number = records[MAX_RECORDS].ID;
        switch(action) {
        case ADD_RECORD:
            records[records_number].ID = receive_int(act_sock_desc);
            records[records_number].age = receive_int(act_sock_desc);
            records[records_number].salary = receive_int(act_sock_desc);

            
            if(records[records_number].ID >= 0 && records[records_number].age >= 0 && records[records_number].salary >= 0) {
                printf("New record is: ID: %d, age: %d, salary: %d\n",records[records_number].ID, records[records_number].age, records[records_number].salary);
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

        case -1: 
            #ifdef DEBUG
            printf("Ending connection on process no.: %d!\n",getpid());
            #endif
            sigint_cli();
        break;

        default:
            send_int(act_sock_desc, 0);
        break;

        }
        records[MAX_RECORDS].ID = records_number;
    }
}

void sigint() {
    signal(SIGINT,sigint); //reset signal    
    for(int i = 0; i < connections_number; i++) {
        kill(SIGHUP,connections[i].pid_client);
        kill(SIGINT,connections[i].pid_server);
        printf("QUIT %d\n", connections[i].pid_client);
    }
    #ifdef DEBUG
    printf("\nClosing main socket! + %d kid processes\n", connections_number);
    #endif
    close(act_sock_desc);
    close(sock_desc);
    #ifdef DEBUG
    printf("Destroying shared memory\n");
    #endif
    shmctl(shmid,IPC_RMID,0);
    exit(0);
}

void sigint_cli() {
    signal(SIGINT,sigint); //reset signal    
    #ifdef DEBUG
    printf("Proces no.: %d Closing sockets!\n", getpid());
    #endif
    close(act_sock_desc);
    close(sock_desc);
    exit(0);
}
