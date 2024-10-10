//------------------------------------------------------------------------------
//  Noel Ball njball 01/13/2024
//  httpserver.c for CSE 130 Programming Assignment 2
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <regex.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "asgn2_helper_funcs.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

Listener_Socket sock;
regmatch_t match[6];
char input[2048];
char request[6][2048];

int regex(int client);
int get(char path[], char msg[]);
int put(char path[], char len[], char msg[]);
void reply(int client, int status, char msg[]);

int main(int argc, char *argv[]) {
    if (argc < 1) {
        return 1;
    }
    // Initializing socket
    int client, port = atoi(argv[1]);
    //printf("port: %d\n", port);
    listener_init(&sock, port);
    //printf("socket initialized\n");
    while (1) {
        // Accept new connection
        client = listener_accept(&sock);
        //printf("client accepted\n");

        // Parse fields of request into buffers organized in array
        for (int i = 0; i < 6; i++) {
            memset(request[i], 0, sizeof(request[i]));
        }
        //printf("request initialized\n");
        char *msg = calloc(1000000, sizeof(char));
        int status = regex(client);
        for (int i = 1; i < 6 && match[i].rm_so != -1 && match[i].rm_eo != -1; i++) {
            int start = match[i].rm_so;
            int end = match[i].rm_eo;
            int length = end - start;
            snprintf(request[i - 1], 2048, "%.*s", length, input + start);
            //printf("request %d: '%s'\n", i, request[i - 1]);
        }
        if (status == 0) {
            // Perform request
            if (strcmp(request[0], "GET") == 0) {
                status = get(request[1], msg);
            } else if (strcmp(request[0], "PUT") == 0) {
                status = put(request[1], request[3], request[4]);
            } else {
                // Unimplemented method
                status = 501;
            }
            if (strcmp(request[2], "HTTP/1.1") != 0 && status != 501) {
                status = 505;
            }
        }
        //printf("status: %d\n", status);
        // Construct response
        reply(client, status, msg);
        // Close connection
        free(msg);
        close(client);
        //printf("client closed\n");
    }
    return 0; // added this just in case, doesn't seem ot have changed anything tho
}

int regex(int client) {
    //printf("in regex!\n");
    // Define the regular expression pattern
    const char *pattern = "^([a-zA-Z]{1,8}) /([a-zA-Z0-9.-]{1,63}) "
                          "(HTTP/[0-9]+\\.[0-9]+)\\\r\\\n([a-zA-Z0-9.-]{1,128}: [ "
                          "-~]{0,128}\\\r\\\n)*\\\r\\\n(.*)$";

    // Compile the regular expression
    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        regfree(&regex);
        return 500;
    }
    //printf("regex compiled\n");

    // Input string to be parsed
    memset(input, 0, sizeof(input));
    ssize_t read = read_until(client, input, sizeof(input), "\r\n\r\n");
    //printf("read from client:\n");
    //printf("'%s'\n", input);
    if (read == -1) {
        regfree(&regex);
        return 500;
    }

    // Execute the regular expression
    memset(match, 0, sizeof(regmatch_t) * 6);
    ret = regexec(&regex, input, 6, match, 0);
    //printf("regexec returned!\n");
    if (ret != 0) {
        // No match
        //printf("no match\n");
        regfree(&regex);
        return 400;
    }
    //printf("match successful\n");
    // Free the compiled regular expression and reset buffer
    regfree(&regex);
    return 0;
}

int get(char path[], char msg[]) {
    // Open file to read contents
    //printf("in get\n");
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        if (errno == EACCES) {
            // File not accessible
            return 403;
        } else if (errno == ENOENT) {
            // File not found
            return 404;
        } else {
            // Error opening
            return 500;
        }
    }
    // Read from file
    int read = read_until(fd, msg, 1000000, NULL);
    if (read == -1) {
        // Read error
        return 500;
    }
    return 200;
}

int put(char path[], char len[], char msg[]) {
    // Open file to write contents
    //printf("in put\n");
    int fd = open(path, O_WRONLY | O_TRUNC);
    int status = 200;
    if (fd == -1) {
        // File not found
        mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH
                      | S_IXOTH;
        fd = open(path, O_WRONLY | O_CREAT, mode);
        status = 201;
    }
    if (fd == -1) {
        // File should exist by now
        if (errno == EACCES) {
            // File not accessible
            return 403;
        } else {
            // Error opening
            return 500;
        }
    }
    //printf("writing to file\n");
    // Write to file
    int value;
    sscanf(len, "Content-Length: %d\n", &value);
    //printf("value: %d\n", value);
    int written = write_n_bytes(fd, msg, min((unsigned long) value, strlen(msg)));
    if (written == -1) {
        // Write error
        return 500;
    }
    return status;
}

void reply(int client, int status, char msg[]) {
    // Construct response with on status cases
    //printf("in reply\n");
    //fprintf(stdout, "In reply\n");
    char *response = calloc(2048, sizeof(char));
    int len = 0;
    switch (status) {
    case 200:
        if (strcmp(request[0], "GET") == 0) {
            // GET status 200 includes msg
            response = calloc(1000000, sizeof(char));
            len = snprintf(response, 1000000,
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: %lu\r\n"
                "\r\n"
                "%s",
                strlen(msg), msg);
        } else {
            // PUT status 200
            len = snprintf(response, 2048,
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: 3\r\n"
                "\r\n"
                "OK\n");
        }

        break;
    case 201:
        len = snprintf(response, 2048,
            "HTTP/1.1 201 Created\r\n"
            "Content-Length: 8\r\n"
            "\r\n"
            "Created\n");
        break;
    case 400:
        len = snprintf(response, 2048,
            "HTTP/1.1 400 Bad Request\r\n"
            "Content-Length: 12\r\n"
            "\r\n"
            "Bad Request\n");
        break;
    case 403:
        len = snprintf(response, 2048,
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "Forbidden\n");
        break;
    case 404:
        len = snprintf(response, 2048,
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 10\r\n"
            "\r\n"
            "Not Found\n");
        break;
    case 500:
        len = snprintf(response, 2048,
            "HTTP/1.1 500 Internal Server Error\r\n"
            "Content-Length: 21\r\n"
            "\r\n"
            "Internal Server Error\n");
        break;
    case 501:
        len = snprintf(response, 2048,
            "HTTP/1.1 501 Not Implemented\r\n"
            "Content-Length: 16\r\n"
            "\r\n"
            "Not Implemented\n");
        break;
    case 505:
        len = snprintf(response, 2048,
            "HTTP/1.1 505 Version Not Supported\r\n"
            "Content-Length: 22\r\n"
            "\r\n"
            "Version Not Supported\n");
    }
    // Send the response if constructed
    //fprintf(stdout, "response:\n%s\n", response);
    if (len > 0) {
        write_n_bytes(client, response, len);
    }
    free(response);
}
