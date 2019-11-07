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
        printf("Chyba pri posielani!\n");
    }
    return k;
}

char *receive_string (int sock_desc) {
    char *s[RECEIVE_BUF]; 
    int k = recv(sock_desc,s,RECEIVE_BUF,0);
    if(k < 0) {
        printf("Chyba pri posielani!\n");
    }
    s[k] = '\0';
    //return s;
    
}
