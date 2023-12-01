#ifndef _FTP_H_
#define _FTP_H_

#define MAX_LENGTH 612

#define SERVER_PORT 21

typedef struct {
    char user[50];
    char password[50];
    char path[MAX_LENGTH];
    char host[50];
    char file[50];
    char ip[50];
} URL;

int parse_url(const char* parameters, URL* url);

int get_ip(const char* hostname, char* ip);

int get_data_socket_info(const char* resource, char* ip, int* port);

int open_socket(const char* ip, const int port, int* sockfd);

int establish_connection(const int sockfd, URL* url, char* response);

int send_data(const int sockfd, const char* buffer);

int receive_data(const int sockfd, char* response);

int close_socket(const int sockfd);

#endif
