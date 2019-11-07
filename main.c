#include "header.h"
#include "statistic.h"
#include "functions.h"

int main() {
    int pid;
    if((pid =fork()) == -1) {
        printf("Cannot fork\n");
        return 0;
    }

    if(pid == 0) {
        sleep(1);
        int a = connect_socket(61000);
        printf("Zadajte znak\n");
        while(1) {
            char s[100];
            scanf("%s",s);
            printf("Odosielatel: Poslal som %s\n",s);
            int k = send_string(a,s);
        }

    } else {
        int a = open_socket(61000);
        char s[100];
        while(1) {
            int k = recv(a,s,RECEIVE_BUF,0);
            if(k > 0) {
                s[k] = '\0';
                printf("prijimatel: Dostal som: %s\n",s);
            }
        }
    }
}

