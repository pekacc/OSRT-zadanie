#include "header.h"
#include "functions.h"
#include "requests.h"

void sighup();
void sighup_console();
void client();

int sock_desc;
FILE *input;

int main(int argc, char **argv) {
    if (argc <= 1)client(1, "\0");
    else {
        int num = *argv[1] - '0';
        if(num < 1 || num > 4 ) {
            printf("Bad argument!\n");
            return -1;
        }
        printf("Argument is: %d\n", num);
        for (int i = 1; i < num; i++) {
            char file[] = "client__";
            file[6] = i + '0';
            file[7] = '\0';
            #ifdef CLIENT_DEBUG
            printf("File is: %s\n", file);
            #endif
            if(fork() == 0) client(0, file); //creating bot client
        }
        client(1, "\0"); //console client
    }
}

void client(int console, char *file) {
    if(console) {
        signal(SIGHUP, sighup_console);
        signal(SIGINT, sighup_console);
    } else {
        signal(SIGHUP, sighup);
        signal(SIGINT, sighup);
    }

    sock_desc = connect_socket(MAIN_SOCKET); //connecting to main server
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
    if(!console) {
        if ((input = fopen(file, "r")) == NULL) {
            printf("Error in opening file %s\n",file);
            sighup();
        }
        #ifndef CLIENT_DEBUG
        fclose(stdout);
        #endif
    }

    while(1) {
        int ID,command, age, salary;
        if(console) {
            printf("\nInsert command (0 for help):\n");
            scanf("%d", &command);
            printf("\nSending command no.: %d\n", command);
        } else {
            sleep(3);
            fscanf(input, "%d", &command);
        }
        send_int(sock_desc, command);
        switch (command) {

            case ADD_RECORD:
                if (console) add_record_console(sock_desc);
                else {
                    int id,age, salary;
                    fscanf(input, "%d", &id);
                    fscanf(input, "%d", &age);
                    fscanf(input, "%d", &salary);
                    add_record(sock_desc, id, age, salary);
                }
            break;

            case SHOW_ALL:
                printf("Showing all records on server:\n");
                show_all(sock_desc);
            break;

            case MEAN:
                printf("Mean is: %d \n", receive_int(sock_desc));
            break;

            case MAX_VALUE:
                printf("Record with maximal salary: ");
                show_single_record(sock_desc);
            break;

            case MIN_VALUE:
                printf("Record with minimal salary: ");
                show_single_record(sock_desc);
            break;

            case CLEAR_DATABASE:
                printf("Database was cleared!\n");
            break;

            case MENU:
                printf(COMMANDS_LIST);
        }
        command = receive_int(sock_desc);
        printf("received %d\n", command);
        if (command == -1) {
            if (console) sighup_console();
            else sighup();
        }
        
    }
}

void sighup() {
    signal(SIGHUP, sighup);
    #ifdef DEBUG
    printf("Closing client\n");
    #endif
    close(sock_desc);
    fclose(input);
    exit(0);
}

void sighup_console() {
    signal(SIGHUP, sighup_console);
    #ifdef DEBUG
    printf("Closing client\n");
    #endif
    close(sock_desc);
    exit(0);
}
