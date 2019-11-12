int open_socket(int port);
int connect_socket(int port);
int send_string(int sock_desc, char *s);
char * receive_string (int sock_desc);
int listen_socket(int sock_desc);
int send_int(int, int);
int receive_int(int);
