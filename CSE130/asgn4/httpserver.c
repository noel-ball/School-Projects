//------------------------------------------------------------------------------
//  Noel Ball njball 03/08/2024
//  httpserver.c for CSE 130 Programming Assignment 4
//------------------------------------------------------------------------------

// Including libraries and initializing values
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <regex.h>
#include "asgn2_helper_funcs.h"
#include "debug.h"
#include "queue.h"
#include "rwlock.h"
#define THREADS       4
#define QUEUE_SIZE    100
#define BUFFER_SIZE   4096
#define RESPONSE_SIZE 1024
#define min(a, b)     ((a) < (b) ? (a) : (b))
Listener_Socket listener;
queue_t *connection_q;
pthread_t *worker_t;
int num_threads;
sem_t sem;
rwlock_t *rwlock = NULL;

// Function declarations
void start(int port, int num_threads);
void *worker(void *arg);
int regex(int client, char input[], regmatch_t match[]);
int handle_get(char path[], char msg[]);
int handle_put(char path[], int len, char *msg);
void reply(int client, int status, char msg[], char *request);
void audit(char *method, char *uri, int status, int id);
void cleanup(void);

// Server functionality
int main(int argc, char *argv[]) {
    //fdebug(stdout, "in main!");
    int port;
    int opt;
    num_threads = THREADS;
    //fdebug(stdout, "parsing args");
    // Parse arguments
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't': num_threads = atoi(optarg); break;
        default: ferror(stdout, "Usage: %s [-t threads] <port>\n", argv[0]); exit(EXIT_FAILURE);
        }
    }
    //fdebug(stdout, "past opt");
    if (optind >= argc) {
        ferror(stdout, "Expected argument after options\n");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[optind]);
    //fdebug(stdout, "port: %d", port);
    // Start the HTTP server as dispatcher thread
    start(port, num_threads);
    //fdebug(stdout, "server start");
    while (1) {
        int client = listener_accept(&listener);
        if (client == -1) {
            ferror(stdout, "Failed to accept client connection\n");
            continue; // skip to next client
        }
        sem_wait(&sem);
        if (!queue_push(connection_q, (void *) (intptr_t) client)) {
            ferror(stdout, "Failed to enqueue client connection\n");
            close(client); // close if enqueue failure
        }
        sem_post(&sem);
        //fdebug(stdout, "ending dispatcher");
    }
    // Clean and close
    //fdebug(stdout, "cleaning");
    cleanup();
    //fdebug(stdout, "returning 0");
    return 0;
}

void start(int port, int num_threads) {
    // Initialize client socket
    if (listener_init(&listener, port) != 0) {
        ferror(stdout, "Failed to initialize listener socket\n");
        exit(EXIT_FAILURE);
    }

    // Initialize connection queue
    connection_q = queue_new(QUEUE_SIZE);
    if (connection_q == NULL) {
        ferror(stdout, "Failed to create connection queue\n");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for threads
    worker_t = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    if (worker_t == NULL) {
        ferror(stdout, "Failed to allocate memory for worker threads\n");
        exit(EXIT_FAILURE);
    }

    // Create worker threads
    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&worker_t[i], NULL, worker, NULL) != 0) {
            ferror(stdout, "Failed to create worker thread\n");
            exit(EXIT_FAILURE);
        }
    }

    // Initialize semaphore as up
    if (sem_init(&sem, 0, 1) != 0) {
        ferror(stdout, "Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    // Initialize rwlock
    rwlock = rwlock_new(READERS, 0);
    if (!rwlock) {
        ferror(stdout, "Failed to initialize rwlock\n");
        exit(EXIT_FAILURE);
    }
}

void *worker(void *arg) {
    (void) arg;
    while (1) {
        int client;
        // Dequeue client sockets to handle
        if (queue_pop(connection_q, (void **) &client)) {
            // Handle the HTTP client request
            regmatch_t match[8];
            char input[2048];
            char request[8][2048];
            //fdebug(stdout, "client accepted");

            // Parse fields of request into buffers organized in array
            for (int i = 0; i < 8; i++) {
                memset(request[i], 0, sizeof(request[i]));
            }
            //fdebug(stdout, "request initialized");
            char *msg = calloc(1000000, sizeof(char));
            int status = regex(client, input, match);
            for (int i = 1; i < 8 && match[i].rm_so != -1 && match[i].rm_eo != -1; i++) {
                int start = match[i].rm_so;
                int end = match[i].rm_eo;
                int length = end - start;
                snprintf(request[i - 1], 2048, "%.*s", length, input + start);
                //fdebug(stdout, "request %d: '%s'", i - 1, request[i - 1]);
            }
            //fdebug(stdout, "request compiled!");
            // Parse Request-Id and Content-Length headers
            int req_id = 0;
            int cont_len = 0;
            if (strcmp(request[0], "PUT") == 0) {
                sscanf(request[3], "Request-Id: %d\n", &req_id);
                if (req_id == 0) {
                    sscanf(request[4], "Request-Id: %d\n", &req_id);
                }
                //fdebug(stdout, "req_id: %d", req_id);
                sscanf(request[3], "Content-Length: %d\n", &cont_len);
                if (cont_len == 0) {
                    sscanf(request[4], "Content-Length: %d\n", &cont_len);
                }
                //fdebug(stdout, "cont_len: %d", cont_len);
                if (req_id != 0 && cont_len != 0) {
                    msg = request[5];
                } else if (req_id == 0 && cont_len != 0) {
                    msg = request[4];
                }
            }
            if (status == 0) {
                // Perform request
                if (strcmp(request[0], "GET") == 0) {
                    status = handle_get(request[1], msg);
                } else if (strcmp(request[0], "PUT") == 0) {
                    status = handle_put(request[1], cont_len, msg);
                } else {
                    // Unimplemented method
                    status = 501;
                }
                if (strcmp(request[2], "HTTP/1.1") != 0 && status != 501) {
                    status = 505;
                }
            }
            //fdebug(stdout, "status: %d", status);
            // Construct response and audit
            char uri[2049] = "/";
            strcat(uri, request[1]);
            //fdebug(stdout, "uri: %s", uri);
            sem_wait(&sem);
            reply(client, status, msg, request[0]);
            audit(request[0], uri, status, req_id);
            sem_post(&sem);
            // Close connection
            //fdebug(stdout, "freeing msg");
            msg = NULL;
            free(msg);
            //fdebug(stdout, "freeing client");
            close(client);
            //fdebug(stdout, "client closed");
        }
    }
    return NULL;
}

int regex(int client, char input[], regmatch_t match[]) {
    //fdebug(stdout, "in regex!");
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
    //fdebug(stdout, "regex compiled");

    // Input string to be parsed
    memset(input, 0, 2048);
    ssize_t read = read_until(client, input, 2048, "\r\n\r\n");
    //fdebug(stdout, "read from client:");
    //fdebug(stdout, "'%s'", input);
    if (read == -1) {
        regfree(&regex);
        return 500;
    }

    // Execute the regular expression
    memset(match, 0, sizeof(regmatch_t) * 8);
    ret = regexec(&regex, input, 8, match, 0);
    //fdebug(stdout, "regexec returned!");
    if (ret != 0) {
        // No match
        //fdebug(stdout, "no match");
        regfree(&regex);
        return 400;
    }
    //fdebug(stdout, "match successful");
    // Free the compiled regular expression and reset buffer
    regfree(&regex);
    return 0;
}

int handle_get(char path[], char msg[]) {
    // Open file to read contents
    //fdebug(stdout, "in get");
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
    reader_lock(rwlock);
    int read = read_until(fd, msg, 1000000, NULL);
    if (read == -1) {
        // Read error
        reader_unlock(rwlock);
        return 500;
    }
    reader_unlock(rwlock);
    return 200;
}

int handle_put(char path[], int len, char *msg) {
    // Open file to write contents
    //fdebug(stdout, "in put");
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
    //fdebug(stdout, "writing to file");
    // Write to file
    writer_lock(rwlock);
    //fdebug(stdout, "bytes: %lu\n", min((unsigned long) len, strlen(msg)));
    int written = write_n_bytes(fd, msg, min((unsigned long) len, strlen(msg)));
    if (written == -1) {
        // Write error
        writer_unlock(rwlock);
        return 500;
    }
    writer_unlock(rwlock);
    return status;
}

void reply(int client, int status, char msg[], char *request) {
    // Construct response with on status cases
    //fdebug(stdout, "in reply");
    char *response = calloc(2048, sizeof(char));
    int len = 0;
    switch (status) {
    case 200:
        if (strcmp(request, "GET") == 0) {
            // GET status 200 includes msg
            free(response);
            response = calloc(1002048, sizeof(char));
            len = snprintf(response, 1002048,
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
    //fdebug(stdout, "out switch");
    // Send the response if constructed
    //fdebug(stdout, "response:\n%s\n", response);
    if (len > 0) {
        write_n_bytes(client, response, len);
        //fdebug(stdout, "sent to client");
    }
    free(response);
    //fdebug(stdout, "response freed");
}

void audit(char *method, char *uri, int status, int id) {
    //fdebug(stdout, "in audit, id: %d", id);
    fprintf(stderr, "%s,%s,%d,%d\n", method, uri, status, id);
    //fdebug(stdout, "audited");
}

void cleanup(void) {
    // join all threads
    sem_wait(&sem);
    for (int i = 0; i < num_threads; i++) {
        pthread_join(worker_t[i], NULL);
    }
    sem_post(&sem);

    // free resources and close socket
    free(worker_t);
    queue_delete(&connection_q);
    sem_destroy(&sem);
    rwlock_delete(&rwlock);
    close(listener.fd);
}
