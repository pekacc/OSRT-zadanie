#include "header.h"
#include "statistic.h"
#include "functions.h"
int main() {
    int a = connect_socket(61000);
    printf("Zadajte znak\n");
    while(1) {
        char s[100];
        scanf("%s",s);
        printf("Odosielatel: Poslal som %s\n",s);
        int k = send_string(a,s);
        printf("%d\n",strcmp(s,"exit"));
        if(strcmp(s,"exit") == 0) {
            printf("Exiting connection");
            close(a);
            break;
        }
    }
}
