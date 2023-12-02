#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "ftp.h"

#define SERVER_PORT 21

int get_ip(const char* hostname, char* ip);

int get_data_socket_info(const char* resource, char* ip, int* port);

int open_socket(const char* ip, const int port, int* sockfd);

int establish_connection(const int sockfd, URL* url, char* response);

int send_data(const int sockfd, const char* buffer);

int receive_data(const int sockfd, const char* expected, char* response);

int close_socket(const int sockfd);

int request_download(const int sockfd, const char* path);

int download(const int controlSockfd, const int dataSockfd, const char* file, const char* expected);

#endif
