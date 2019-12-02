#include "header.h"

int open_socket(int port) {  

    // vytvorenie socketu
    int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
    char c = '\n';
    char *p_buf;
    if (sock_desc == -1)
    {
        printf("cannot create socket!\n");
        return 0;
    }

    // nastavenie socketu
    struct sockaddr_in server;  
    memset(&server, 0, sizeof(server));  
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;  
    server.sin_port = htons(port);  
    if (bind(sock_desc, (struct sockaddr*)&server, sizeof(server)) != 0) {
        printf("cannot bind socket!\n");
        close(sock_desc);  
        return -1;
    }
    return sock_desc;
}

int listen_socket(int sock_desc) {
    if (listen(sock_desc, 20) != 0) {
        printf("cannot listen on socket!\n");
        close(sock_desc);  
        return -1;
    }
    struct sockaddr_in client;  
    memset(&client, 0, sizeof(client));  
    socklen_t len = sizeof(client); 
    int temp_sock_desc = accept(sock_desc, (struct sockaddr*)&client, &len);  
    if (temp_sock_desc == -1)
    {
        printf("cannot accept client!\n");
        close(sock_desc);  
        return -1;
    }
    return temp_sock_desc;

}

int connect_socket(int port) {

    int sock_desc = socket(AF_INET, SOCK_STREAM, 0); 
    if (sock_desc == -1) {
        printf("cannot create socket!\n");
        return -1;
    }


    // nastavenie socketu
    struct sockaddr_in client;  
    memset(&client, 0, sizeof(client));  
    client.sin_family = AF_INET;  
    client.sin_addr.s_addr = inet_addr("127.0.0.1");  
    client.sin_port = htons(port);  

    // pripojenie socketu
    if (connect(sock_desc, (struct sockaddr*)&client, sizeof(client)) != 0) {
        printf("cannot connect to server!\n");
        close(sock_desc);
	return -1;
    }
    return sock_desc;
}

int send_string(int sock_desc, char *s) {
    int k = send(sock_desc,s,strlen(s),0);
    if(k != strlen(s) || k < 0) {
        printf("Sending error!\n");
    }
    return k;
}

int receive_string (int sock_desc, char *s) {
    int k = 0;
        k = recv(sock_desc,s,RECEIVE_BUF-1,0); //RECEIVE_BUF -1 due to \0 
        if(k <= 0) {
            printf("Receiving error! k = %d\n", k);
            return -1;
        }
    s[k] = '\0';
    return k;
    
}
int send_int(int sock_desc, int i) {
    int k = send(sock_desc,&i,sizeof(int),0);
    if(k != sizeof(int) || k <= 0) {
        printf("Sending error!\n");
    }
    return k;
}

int receive_int(int sock_desc) {
    int i;
    int k = recv(sock_desc,&i,sizeof(int),0);
    if(k <= 0) { //bad received format
        //printf("Receiving error! k = %d\n", k);
        return -1;
    }
    return i;
}

int new_socket(int *port) {
    int sock_desc;
    int try_port = MIN_PORT_NUMBER;
    do {
        sock_desc = open_socket(try_port);
        if(sock_desc <= 0) {
            #ifdef DEBUG
            printf("Cannot open socket on port %d, trying another\n", try_port);
            #endif
            try_port++;
        }
    } while(sock_desc <= 0);
    (*port) = try_port;
    return sock_desc;
} 

RECORD *create_shm(int pid) { //creating shared memory
    int shmid;
    RECORD *shm;
    if ((shmid = shmget(pid, (MAX_RECORDS)*sizeof(RECORD), IPC_CREAT | 0666)) < 0) {
        printf("Error in creating shared memory\n");
        return((RECORD *)-1);
    }   

    if((shm = shmat(shmid, NULL, 0)) == (RECORD *) -1) {
        printf("Error in creating shared memory\n");
        return((RECORD *)-1);
    }   
    return shm;
}

timer_t create_my_timer(int signal) {
    struct sigevent ev;
    ev.sigev_notify=SIGEV_SIGNAL;
    ev.sigev_signo=signal;

    timer_t my_timer;
    timer_create(CLOCK_REALTIME, &ev, &my_timer);
    return(my_timer);
}

void set_my_timer(timer_t my_timer, int sec, int interval) {
    struct itimerspec tim;
    tim.it_value.tv_sec=sec; //first start
    tim.it_value.tv_nsec=0;
    tim.it_interval.tv_sec=interval; //periode of start
    tim.it_interval.tv_nsec=0;
    timer_settime(my_timer,CLOCK_REALTIME,&tim,NULL);
}


void remove_connection(CONNECTIONS *connections, int connections_number, pid_t pid) {
    int num = 0;
    for (int i = 0; i < connections_number; i++) {
        if(connections[i].pid_server == pid) {
            num = i;
        }
    }
    kill(connections[num].pid_client, SIGHUP);
    for (int i = num+1; i < connections_number; i++) {
        connections[i-1] = connections[i];
    }
}
