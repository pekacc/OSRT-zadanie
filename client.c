#include "header.h"
#include "functions.h"
#include "requests.h"

void sighup();
void set_timer(timer_t, int, int);

int sock_desc;

int main() {
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
        printf("\nInsert command\n");
        int ID,command, age, salary;
        scanf("%d", &command);
        printf("\nSending command no.: %d\n", command);
        send_int(sock_desc, command);
        switch (command) {
            case ADD_RECORD:
                printf("Sending record\n");
                sleep(5);
                add_record(sock_desc, 98556, 30, 500);
            break;

            case SHOW_ALL:
                printf("Showing all records on server:\n");
                show_all(sock_desc);
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

void set_timer(timer_t timer, int sec, int interval) {
    struct itimerspec my_timer;
    my_timer.it_value.tv_sec = sec; //first signal
    my_timer.it_value.tv_nsec = sec;
    my_timer.it_interval.tv_sec = sec; //periodical
    my_timer.it_interval.tv_sec = sec;
    timer_settime(timer, CLOCK_REALTIME,&my_timer, NULL);
}
