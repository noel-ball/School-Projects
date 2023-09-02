//-----------------------------------------------------------------------------
// Noel Ball njball pa1
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "List.h"
#define MAX 500

int main(int argc, char* argv[]){
    FILE *in, *out;
    char *fileErr = "Error opening the file\n";
    char line[MAX];
    int lr = 0; //lines read

    //ensuring there are two args
    if ( argc!=3 ) {
        fprintf(stderr, "Lex takes two arguments: ./Lex <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }

    //copying input and output file names
    char *in_name = argv[1];
    char *out_name = argv[2];

    //opening the files
    if ( access(in_name, F_OK) != 0 ) {
        fprintf(stderr, "%s\n", fileErr);
        return 1;
    }
    in = fopen(in_name, "r");
    out = fopen(out_name, "w+");
    if( in == NULL || out == NULL ) {
        fprintf(stderr, "%s\n", fileErr);
        exit(EXIT_FAILURE);
    }

    //parse for number of lines read
    while(fgets(line, MAX, in) != NULL){
        lr+=1;
    }

    //allocate string and set elements to NULL
    char **str = malloc(lr * sizeof (char *));
    for( int i = 0; i < lr; i+=1 ) {
        str[i] = NULL;
    }

    //reset file parse to read lines to str
    rewind(in);
    int k = 0;
    while( fgets(line, MAX, in ) != NULL){
        char *str_end = strchr(line, '\n');
        if( str_end!=NULL ) {
            *str_end = '\0';
        }
        str[k] = malloc(strlen(line) + 1);
        if( str[k]==NULL ) {
            fprintf(stderr, "Error allocating memory\n");
            exit(EXIT_FAILURE);
        }
        strcpy(str[k], line);
        k+=1;
    }

    //create Lists and include each element
    List l = newList();
    bool included = 0;
    for( int i = 0; i < lr; i+=1 ) {
        included = 0;
        if( length(l) == 0 ) {
            append(l, 0);
            continue;
        }
        moveFront(l);
        for( int j = 0; j < length(l); j+=1 ) { //indexes through and checks if new val should go left of cursor val
            if(strcmp(str[i], str[get(l)]) <= 0) {
                insertBefore(l, i);
                included = 1;
                break;
            }
            moveNext(l);
        }
        if( included==1 ){
            continue;
        }
        //value is included at end of list because it was not included earlier
        append(l, i);
    }

    //write to outfile
    moveFront(l);
    for( int i = 0; i < lr; i+=1 ) {
        fprintf(out, "%s\n", str[get(l)]);
        moveNext(l);
    }

    //free memory
    for( int j = 0; j < lr; j+=1 ) { //freeing and closing files
        free(str[j]);
    }
    freeList(&l);
    free(str);
    //close files
    fclose(in);
    fclose(out);
    return 0;
}
