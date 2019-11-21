#include "header.h"
#include "functions.h"
#include "requests.h"

void sighup();

int sock_desc;

int main() {
    signal(SIGHUP, sighup);
    sock_desc = connect_socket(61000);
    if(sock_desc < 0) {
        printf("Cannot connect to server!\n");
        close(sock_desc);
        exit(-1);
    }
    printf("Zadajte cislo\n");

    int action = getpid();
    int k = send_int(sock_desc,action);
    #ifdef DEBUG
    printf("Sent: %d\nBytes sent: je %d\n",action, k);
    #endif
    if(action == 0) {
        printf("Exiting connection");
        close(sock_desc);
        exit(-1);
    }

    action = receive_int(sock_desc);
    #ifdef DEBUG
    printf("New port is: %d\n", action);
    #endif
    close(sock_desc);
    sleep(1);
    printf("Connecting to new socket\n");
    sock_desc = connect_socket(action);
    printf("Insert command\n");
    while(1) {
        int ID,command, age, salary;
        scanf("%d", &command);
        printf("Sending command no.: %d\n", command);
        send_int(sock_desc, command);
        switch (command) {
            case ADD_RECORD:
                printf("Sending record\n");
                add_record(sock_desc, 98556, 30, 500);
        }
        command = receive_int(sock_desc);
        printf("Dostal som %d\n", command);
    }
}

void sighup() {
    signal(SIGHUP, sighup);
    #ifdef DEBUG
    printf("Hang up from server side\n");
    #endif
    close(sock_desc);
    exit(0);
}
