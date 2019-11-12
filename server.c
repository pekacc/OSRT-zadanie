#include "header.h"
#include "statistic.h"
#include "functions.h"

int main() {
    int b = open_socket(61000);
    int a = listen_socket(b);
    char s[100];
    while(1) {
        int k = recv(a,s,RECEIVE_BUF,0);
        if(strcmp(s,"exit") == 0) {
            printf("%d\n", strcmp(s,"exit"));
            printf("Ending connection!\n");
            close(a);
            break;
        }
        if(k > 0) {
            s[k] = '\0';
            printf("prijimatel: Dostal som: %s\n",s);
        }
    }
    close(b);
}

