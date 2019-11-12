#include "header.h"
#include "statistic.h"
#include "functions.h"

void sigint();
int sock_desc, act_sock_desc, connections_number;
CONNECTIONS connections[MAX_CONNECTIONS];

int main() {
    signal(SIGINT, sigint);
    connections_number = 0;

    sock_desc = open_socket(61000);
    if(sock_desc < 0) {
        printf("Socker error!\n");
        exit(-1);
    }
    char buf[100];
    while(1) {
        act_sock_desc = listen_socket(sock_desc);
        while(1) {
            int action = receive_int(act_sock_desc);
            if(action == -1) { //No message
                continue;
            }
            else if(action == 0) { //Close from client side
                #ifdef DEBUG
                printf("Ending connection!\n");
                #endif
                close(act_sock_desc);
                break;
            }
            #ifdef DEBUG
            printf("Received: %d\n", action);
            #endif

            action += 1;
            send_int(act_sock_desc, action);
        }
        #ifdef DEBUG
        printf("Waiting for another connection\n");
        #endif
    }
    close(sock_desc);
}

void sigint() {
    signal(SIGINT,sigint); //reset signal    
    #ifdef DEBUG
    printf("Closing sockets!\n");
    #endif
    close(sock_desc);
    close(act_sock_desc);
    exit(0);
}
