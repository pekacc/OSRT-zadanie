int open_socket(int port);
int connect_socket(int port);
int send_string(int sock_desc, char *s);
char * receive_string (int sock_desc);
