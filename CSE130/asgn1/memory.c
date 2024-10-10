//------------------------------------------------------------------------------
//  Noel Ball njball 01/13/2024
//  memory.c for CSE 130 Programming Assignment 1
//------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    // setting up file handling
    FILE *infile = stdin;
    FILE *outfile = stdout;
    int fd = fileno(infile);

    // reading input string up to 1MB
    char *str = malloc(1000000);
    char *app = malloc(1000000);
    if (str == NULL || app == NULL) {
        fprintf(stderr, "Operation Failed\n");
        free(str);
        free(app);
        return 1;
    }
    // initialize str as empty
    str[0] = 0;
    app[0] = 0;

    int bytes = 0;
    bytes = read(fd, app, 1000000);
    if (bytes != 0) {
        strcpy(str, app);
    }
    while (bytes != 0) {
        bytes = read(fd, app, 1000000);
        if (bytes != 0) {
            strcat(str, app);
        }
    }
    free(app);

    //printf("str[strlen(str)]: (%s)\n", &str[strlen(str) - 1]);
    if (strcmp(&str[strlen(str) - 1], "\n") != 0) {
        fprintf(stderr, "Invalid Command\n");
        free(str);
        fclose(infile);
        return 1;
    }
    // parsing first command by "\" and skipping n, to avoid parsing by n chars as well
    char delim[] = "\n";
    char *cmd = strtok(str, delim);
    //printf("first cmd: %s\n", cmd);

    // get command -----------------------------------
    if (strcmp(cmd, "get") == 0) {
        // parse second command
        cmd = strtok(NULL, delim);
        //printf("scnd cmd: %s\n", cmd);
        infile = fopen(cmd, "r+");
        if (infile == NULL) {
            fprintf(stderr, "Invalid Command\n");
            free(str);
            fclose(infile);
            return 1;
        }

        // any extra command invalid
        cmd = strtok(NULL, "");
        //printf("extra cmd: %s\n", cmd);
        if (cmd != NULL) {
            fprintf(stderr, "Invalid Command\n");
            free(str);
            fclose(infile);
            return 1;
        }
        // reading file contents
        fd = fileno(infile);
        fseek(infile, 0, SEEK_END); // point to end of file
        long int t = ftell(infile); // length of file in bytes
        fseek(infile, 0, SEEK_SET); // point to original for read

        char *out = malloc(t); // buffer up to MB of data
        int tb = 0;
        bytes = 0;
        // looping until bytes read = total bytes
        while (tb < t) {
            bytes = pread(fd, out + tb, t - tb, tb);
            if (bytes == -1) {
                fprintf(stderr, "Invalid Command\n");
                free(str);
                free(out);
                fclose(infile);
                return 1;
            }
            tb += bytes;
        }
        if (tb != t) {
            fprintf(stderr, "Operation Failed\n");
            free(str);
            free(out);
            fclose(infile);
            return 1;
        }

        // printing to stdout
        fd = fileno(outfile);
        bytes = write(fd, out, t);
        if (bytes == -1) {
            fprintf(stderr, "Operation Failed\n");
            free(str);
            free(out);
            fclose(infile);
            return 1;
        }
        // freeing used memory
        fclose(infile);
        return 0;

        // set command -----------------------------------
    } else if (strcmp(cmd, "set") == 0) {
        // parse file location
        cmd = strtok(NULL, delim);
        //printf("scnd cmd: %s\n", cmd);
        outfile = fopen(cmd, "w+");
        if (outfile == NULL) {
            fprintf(stderr, "Invalid Command\n");
            free(str);
            fclose(outfile);
            return 1;
        }
        // parse content length
        cmd = strtok(NULL, delim);
        //printf("thrd cmd: %s\n", cmd);
        if (cmd == NULL) {
            // nothing to write
            fprintf(stdout, "OK\n");
            fclose(outfile);
            return 0;
        }
        char *stop;
        unsigned long len = strtoul(cmd, &stop, 10);

        // parse content
        cmd = strtok(NULL, delim);
        //printf("frth cmd: %s\n", cmd);
        if (cmd == NULL) {
            // nothing to write
            fprintf(stdout, "OK\n");
            fclose(outfile);
            return 0;
        }
        char *out = cmd;

        // printing content to file
        fd = fileno(outfile);
        if (strlen(out) < len) {
            // only writing out bytes
            strcat(out, "\n");
            bytes = write(fd, out, strlen(out));
            if (bytes == -1) {
                fprintf(stderr, "Operation Failed\n");
                free(str);
                fclose(outfile);
                return 1;
            }
        } else {
            // only writing "content length" bytes
            bytes = write(fd, out, len);
            int newl = write(fd, "\n", strlen("\n"));
            if (bytes == -1 || newl == -1) {
                fprintf(stderr, "Operation Failed\n");
                free(str);
                fclose(outfile);
                return 1;
            }
        }
        fprintf(stdout, "OK\n");
        free(str);
        fclose(outfile);
        return 0;

        // invalid command ---------------------------------
    } else {
        fprintf(stderr, "Invalid Command\n");
        free(str);
        return 1;
    }
}
