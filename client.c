#include "header.h"
#include "statistic.h"
#include "functions.h"

void sigquit();

int sock_desc;

int main() {
    signal(SIGQUIT, sigquit);
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
    while(1) {
        int a = 5;
        printf("Posielam %d\n",a);
        send_int(sock_desc,a);
        a = receive_int(sock_desc);
        printf("Dostal som %d\n",a);
        sleep(1);
    }
}

void sigquit() {
    signal(SIGQUIT, sigquit);
    #ifdef DEBUG
    printf("Hang up from server side\n");
    #endif
    close(sock_desc);
    exit(0);
}

