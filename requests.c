#include "header.h"
#include "functions.h"

int add_record(int sock_desc, int ID, int age, int salary) {
    if (send_int(sock_desc, ID) <= 0) {
        return -1;
    } 
    if (send_int(sock_desc, age) <= 0) {
        return -1;
    } 
    if (send_int(sock_desc, salary) <= 0) {
        return -1;
    } 
    #ifdef DEBUG
    printf("Data succesfully sent\n");
    #endif
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
    if (send_int(sock_desc, age) <= 0) {
        return -1;
    } 
    if (send_int(sock_desc, salary) <= 0) {
        return -1;
    } 

    #ifdef DEBUG
    printf("Data succesfully sent\n");
    #endif
    return 0;
}

void show_all(int sock_desc) {
    int number = receive_int(sock_desc);    
    for(int i = 0; i < number; i++) {
        int ID,age,salary;
        ID = receive_int(sock_desc);
        age = receive_int(sock_desc);
        salary = receive_int(sock_desc);
        printf("Record no. %d has ID %d, age %d and salary %d\n", i, ID, age, salary);
    }
}
