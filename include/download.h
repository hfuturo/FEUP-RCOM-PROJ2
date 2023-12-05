#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#define MAX_LENGTH 612

typedef struct {
    char user[50];
    char password[50];
    char path[MAX_LENGTH];
    char host[50];
    char file[50];
    char ip[50];
} URL;

int parse_url(const char* parameters, URL* url);

#endif
