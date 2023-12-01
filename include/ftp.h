#ifndef _FTP_H_
#define _FTP_H_

#define MAX_LENGTH 612

#define SERVER_PORT 21

typedef struct {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char path[MAX_LENGTH];
    char host[MAX_LENGTH];
    char file[MAX_LENGTH];
    char ip[MAX_LENGTH];
} URL;

int parse_url(const char* parameters, URL* url);

int get_ip(const char* hostname, char* ip);

int open_socket(const URL* url, int* sockfd);

int establish_connection(const int sockfd, const URL* url);

int send_data(const int sockfd, const char* buffer);

int receive_data(const int sockfd, char* response);

int close_socket(const int sockfd);

#endif
