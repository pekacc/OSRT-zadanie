#include "header.h"
#include "functions.h"
#include "requests.h"

void sighup();
void client();

int sock_desc;

int main(int argc, char **argv) {
    if (argc <= 1)client(1,"\0");
}

void client(int console, char *file) {
    signal(SIGHUP, sighup);
    sock_desc = connect_socket(61000); //connecting to main server
    if(sock_desc < 0) {
        printf("Cannot connect to server!\n");
        close(sock_desc);
        exit(-1);
    }

    send_int(sock_desc, getpid()); //sending PID to server
    #ifdef DEBUG
    printf("My PID is: %d\n", getpid());
    #endif

    int action = receive_int(sock_desc); //receiving new port
    #ifdef DEBUG
    printf("New port is: %d\n", action);
    #endif
    close(sock_desc);
    sleep(1); //waiting for server fork
    printf("Connecting to new socket\n");
    sock_desc = connect_socket(action);

    while(1) {
        int ID,command, age, salary;
        if(console) {
            printf("\nInsert command (0 for help):\n");
            scanf("%d", &command);
            printf("\nSending command no.: %d\n", command);
        }
        send_int(sock_desc, command);
        switch (command) {

            case ADD_RECORD:
                if (console) add_record_console(sock_desc);
            break;

            case SHOW_ALL:
                printf("Showing all records on server:\n");
                show_all(sock_desc);
            break;

            case MEAN:
                printf("Mean is: %d \n", receive_int(sock_desc));
            break;

            case CLEAR_DATABASE:
                printf("Database was cleared!\n");
            break;

            case MENU:
                printf(COMMANDS_LIST);
        }
        command = receive_int(sock_desc);
        printf("received %d\n", command);
        if (command == -1) sighup();
        
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

