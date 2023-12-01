#ifndef _FTP_H_
#define _FTP_H_

#define MAX_LENGTH 100

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

#endif
