#include "header.h"
#include "statistic.h"
#include "functions.h"

int main() {
    int sock_desc = connect_socket(61000);
    printf("Zadajte cislo\n");

    int action = getpid();
    int k = send_int(sock_desc,action);
    #ifdef DEBUG
    printf("Odosielatel: Poslal som %d\nPocet odoslanych bitov je %d\n",action, k);
    #endif
    if(action == 0) {
        printf("Exiting connection");
        close(sock_desc);
        exit(-1);
    }

    action = receive_int(sock_desc);
    printf("Received %d\n", action);
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
