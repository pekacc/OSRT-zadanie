#include "header.h"
#include "functions.h"

int add_record(int sock_desc, int ID, int age, int salary) {
    if (send_int(sock_desc, ID) <= 0) {
        return -1;
    } 
    printf("sent\n");
    if (send_int(sock_desc, age) <= 0) {
        return -1;
    } 
    printf("sent\n");
    if (send_int(sock_desc, salary) <= 0) {
        return -1;
    } 
    printf("sent\n");
    return 0;
}

int add_record_console(int sock_desc) {
    int ID,age,salary;

    printf("Please insert ID, age and salary:\n");
    scanf("%d",&ID);
    scanf("%d",&age);
    scanf("%d",&salary);

    if (send_int(sock_desc, ID) <= 0) {
        return -1;
    } 
    printf("sent\n");
    if (send_int(sock_desc, age) <= 0) {
        return -1;
    } 
    printf("sent\n");
    if (send_int(sock_desc, salary) <= 0) {
        return -1;
    } 
    printf("sent\n");
    return 0;
}
