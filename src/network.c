#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/network.h"

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

int get_data_socket_info(const char* resource, char* ip, int* port) {
    if (!resource || !ip || !port) {
        printf("Error, argument is NULL in \"%s()\"\n", __func__);
        return -1;
    }

    char param[60];
    strcpy(param, resource);

    strtok(param, "(");

    for (int i = 0; i < 4; i++) {
        char* token = strtok(NULL, ",");
        strcat(ip, token);
        if (i < 3) strcat(ip, ".");
    }

    int port1 = atoi(strtok(NULL, ","));
    int port2 = atoi(strtok(NULL, ")"));

    *port = port1 * 256 + port2;

    return 0;
}

int open_socket(const char* ip, const int port, int* sockfd) {
    struct sockaddr_in server_addr;

    /* server address handling */
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);    /* 32 bit Internet address network byte ordered */
    server_addr.sin_port = htons(port);        /* server TCP port must be network byte ordered */

    /* open a TCP socket */
    if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Error, socket() in \"%s()\"\n", __func__);
        return -1;
    }

    /* connect to the server */
    if (connect(*sockfd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        printf("Error, connect() in \"%s()\"\n", __func__);
        return -1;
    }

    return 0;
}

int establish_connection(const int sockfd, URL* url, char* response) {
    char user[100], password[100], passive[] = "pasv\n";

    snprintf(user, 100, "USER %s\n", url->user);
    snprintf(password, 100, "PASS %s\n", url->password);

    char responseAux[MAX_LENGTH];

    // user
    if (send_data(sockfd, user) == -1) return -1;
    if (receive_data(sockfd, "331", responseAux) == -1) return -1;

    // password
    if (send_data(sockfd, password) == -1) return -1;
    if (receive_data(sockfd, "230", responseAux) == -1) return -1;

    // passive mode
    if (send_data(sockfd, passive) == -1) return -1;
    if (receive_data(sockfd, "227", response) == -1) return -1;

    return 0;
}

int send_data(const int sockfd, const char* buffer) {
    if (!buffer) {
        printf("Error, buffer is NULL in \"%s()\"\n", __func__);
        return -1;
    }

    size_t bytes;

    if ((bytes = write(sockfd, buffer, strlen(buffer))) < 0) {
        printf("Error, write() in \"%s()\"\n", __func__);
        return -1;
    }
    printf("\nSend (%ld bytes):\n%s", bytes, buffer);

    if ((bytes = write(sockfd, "\n", 1)) < 0) {
        printf("Error, write() in \"%s()\"\n", __func__);
        return -1;
    }

    return 0;
}

int receive_data(const int sockfd, const char* expected, char* response) {
    size_t bytes;

    while (true) {
        memset(response, 0, MAX_LENGTH);
        bytes = read(sockfd, response, MAX_LENGTH);
        if (bytes > 0) {
            printf("Resposta (%ld bytes):\n%s", bytes, response);
        }

        // se inicio da resposta nao for um codigo continua a ler
        if (!(response[0] >= '1' && response[0] <= '5') || response[2] != ' ') break;
    }

    if (expected != NULL) {
        if (strncmp(response, expected, 3) != 0) {
            printf("\nError, server responded with wrong code:\n"
                "Expected: %s\n"
                "Received: %s\n",
                expected,
                response
            );
            return -1;
        }
    }

    return 0;
}

int close_socket(const int sockfd) {
    if (close(sockfd) < 0) {
        printf("Error, close() in \"%s()\"\n", __func__);
        return -1;
    }

    return 0;
}

int request_download(const int sockfd, const char* path) {
    char command[MAX_LENGTH], response[MAX_LENGTH];

    snprintf(command, MAX_LENGTH, "retr %s\n", path);
    if (send_data(sockfd, command) == -1) {
        printf("Error, send_data() in \"%s()\"\n", __func__);
        return -1;
    }

    if (receive_data(sockfd, "150", response) == -1) {
        printf("Error, receive_data() in \"%s()\"\n", __func__);
        return -1;
    }
    return 0;
}

int download(const int controlSockfd, const int dataSockfd, const char* file ,const char* expected) {
    FILE* fd = fopen(file, "a+");
    if (!fd) {
        printf("Error creating file in \"%s()\"\n", __func__);
        return -1;
    }

    char buffer[MAX_LENGTH];

    while (true) {
        if (receive_data(dataSockfd, NULL, buffer) == -1) {
            printf("Error, receive_data() in \"%s()\"\n", __func__);
            return -1;
        }
        if (strlen(buffer) == 0) break;
        fwrite(buffer, strlen(buffer), sizeof(char), fd);
        memset(buffer, 0, MAX_LENGTH);
    }

    char response[50];
    if (receive_data(controlSockfd, "226", response) == -1) {
        printf("Error, receive_data() in \"%s()\"\n", __func__);
        return -1;
    }

    fclose(fd);
    
    return 0;
}
