#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    printf("User: %s\n"
            "Password: %s\n"
            "Host: %s\n"
            "Path: %s\n"
            "File: %s\n",
            url.user,
            url.password,
            url.host,
            url.path,
            url.file);

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