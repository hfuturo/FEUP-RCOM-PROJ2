#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../include/ftp.h"
#include "../include/network.h"

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: ./main ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }
    
    printf("\n");
    
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

    char response[MAX_LENGTH];
    int controlSockfd;
    if (open_socket(url.ip, SERVER_PORT, &controlSockfd) == -1) {
        printf("Error, open_socket() in \"%s()\"\n", __func__);
        exit(-1);
    }
    if (receive_data(controlSockfd, "220", response) == -1) {
        printf("Error, receive_data() in \"%s()\"\n", __func__);
        exit(-1);
    }

    // faz login e entra em modo passivo
    if (establish_connection(controlSockfd, &url, response) == -1) {
        printf("Error, establish_connection() in \"%s()\"\n", __func__);
        close_socket(controlSockfd);
        exit(-1);
    }

    int dataSockPort;
    char dataSockIp[50];
    if (get_data_socket_info(response, dataSockIp, &dataSockPort) == -1) {
        printf("Error, get_data_socket_info() in \"%s()\"\n", __func__);
        close_socket(controlSockfd);
        exit(-1);
    }

    int dataSockfd;

    // open data socket
    if (open_socket(dataSockIp, dataSockPort, &dataSockfd) == -1) {
        printf("Error, open_socket(data) in \"%s()\"\n", __func__);
        close_socket(controlSockfd);
        exit(-1);
    }

    if (request_download(controlSockfd, url.path) == -1) {
        printf("Error, request_download() in \"%s()\"\n", __func__);
        close_socket(controlSockfd);
        close_socket(dataSockfd);
        exit(-1);
    }

    if (download(controlSockfd, dataSockfd, url.file , "226") == -1) {
        printf("Error, download() in \"%s()\"\n", __func__);
        close_socket(controlSockfd);
        close_socket(dataSockfd);
        exit(-1);
    }

    return close_socket(controlSockfd) || close_socket(dataSockfd);
}

// ftp://host/path/file.txt                 user: anonymous     pass: anonymous
// ftp://user:password@host/path/file.txt   user: anonymous     pass: anonymous
// ftp://foo:@host.com/                     user: foo           pass: "" (empty)  
// ftp://@host.com/                         user: empty("")     pass: anonymous
int parse_url(const char* parameters, URL* url) {
    if (!url) {
        printf("Error, url is NULL in \"%s()\"\n", __func__);
        return -1;
    }

    char param[MAX_LENGTH];
    strcpy(param, parameters);
    char* token = param;

    // resta /<user>:<password>@host/path/file.txt
    strtok(token, "/");

    // token = <user>:<password>@host/path/file.txt
    token = strtok(NULL, "") + 1;

    if (strstr(token, "@")) {

        if (strstr(token, ":")) {
            // resta <password>@host/path/file.txt
            strcpy(url->user, strtok(token, ":"));

            // token = <password>@host/path/file.txt
            token = strtok(NULL, "");

            if (token[0] == '@') {
                strcpy(url->password, "");
                token = strtok(token, "@");  // remove @ do token ; token = host/path/file.txt
            }
            else {
                strcpy(url->password, strtok(token, "@"));
                token = strtok(NULL, "");   // token = host/path/file.txt
            }
        }
        else {
            strcpy(url->user, "");
            strcpy(url->password, "anonymous");
            token = strtok(token, "@");   // remove @ do token ; token = host/path/file.txt
        }
    }
    else {
        strcpy(url->user, "anonymous");
        strcpy(url->password, "anonymous");
    }

    strcpy(url->host, strtok(token, "/"));

    token = strtok(NULL, "");
    strcpy(url->path, token);

    char* file = strrchr(token, '/');
    strcpy(url->file, file == NULL ? token : file+1);

    return 0;
}

