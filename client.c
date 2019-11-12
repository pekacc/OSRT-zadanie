#include "header.h"
#include "statistic.h"
#include "functions.h"

int main() {
    int sock_desc = connect_socket(61000);
    printf("Zadajte cislo\n");
    while(1) {
        int action;
        scanf("%d",&action);
        int k = send_int(sock_desc,action);
        #ifdef DEBUG
        printf("Odosielatel: Poslal som %d\nPocet odoslanych bitov je %d\n",action, k);
        #endif
        if(action == -1) {
            printf("Exiting connection");
            close(sock_desc);
            break;
        }

        action = receive_int(sock_desc);
        printf("Received %d\n", action);
    }
}
