#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#include "ftp.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./main ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    URL url;
    if (!memset(&url, 0, sizeof(URL))) {
        printf("Error, memset() in \"%s()\"\n", __func__);
        exit(-1);
    }

    if (parse_url(argv[1], &url) == -1) {
        printf("Error, parse_url() in \"%s()\"\n", __func__);
        exit(-1);
    }

    if (get_ip(url.host, url.ip) == -1) {
        printf("Error, get_ip() in \"%s()\"\n", __func__);
        exit(-1);
    }

    printf("User: %s\n"
            "Password: %s\n"
            "Host: %s\n"
            "Path: %s\n"
            "File: %s\n"
            "IP: %s\n",
            url.user,
            url.password,
            url.host,
            url.path,
            url.file,
            url.ip);

    int socketfd;

    if (open_socket(&url, &socketfd) == -1) {
        printf("Error, open_socket() in \"%s()\"\n", __func__);
        exit(-1);
    }

    if (close_socket(socketfd) == -1) {
        printf("Error, close_socket() in \"%s()\"\n", __func__);
        exit(-1);
    }

    return 0;
}

// ftp://[user:password@]host/path/file.txt
int parse_url(const char* parameters, URL* url) {
    if (!url) {
        printf("Error, url is NULL in \"%s()\"\n", __func__);
        return -1;
    }

    char param[500];
    strcpy(param, parameters);

    // resta: user:password@]host/path/file.txt
    strtok(param, "[");

    // resta: password@]host/path/file.txt
    strcpy(url->user, strtok(NULL, ":"));

    // resta: ]host/path/file.txt
    strcpy(url->password, strtok(NULL, "@"));

    // resta: path/file.txt
    strcpy(url->host, strtok(NULL, "/") + 1);

    char* token = strtok(NULL, "");
    strcpy(url->path, token);

    char* file = strrchr(token, '/');
    strcpy(url->file, file == NULL ? token : file+1);

    return 0;
}

int get_ip(const char* hostname, char* ip) {
    if (!hostname) {
        printf("Error, hostname is NULL in \"%s()\"\n", __func__);
        return -1;
    }

    struct hostent* h;

    // get IP from host
    if ((h = gethostbyname(hostname)) == NULL) {
        printf("Error, gethostbyname() in \"%s()\"\n", __func__);
        return -1;
    }

    strcpy(ip, inet_ntoa(*(struct in_addr *) h->h_addr));

    return 0;
}

int open_socket(const URL* url, int* socketid) {
    int sockfd;
    struct sockaddr_in server_addr;

    /* server address handling */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(url->ip);    /* 32 bit Internet address network byte ordered */
    server_addr.sin_port = htons(SERVER_PORT);        /* server TCP port must be network byte ordered */

    /* open a TCP socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error, socket() in \"%s()\"\n", __func__);
        return -1;
    }
    
    /* connect to the server */
    if (connect(sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        printf("Error, connect() in \"%s()\"\n", __func__);
        return -1;
    }

    return 0;
}

int close_socket(const int socketfd) {
    if (close(socketfd) < 0) {
        printf("Error, close() in \"%s()\"\n", __func__);
        return -1;
    }

    return 0;
}
