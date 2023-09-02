//-----------------------------------------------------------------------------
// Noel Ball njball pa3
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main(int argc, char* argv[]) {
    int i, u, v;
    char vert[500];

    //setting up command line arguments for file handling
    FILE *infile, *outfile;
    if (argc != 3) {
        fprintf(stderr, "FindComponents Error: incorrect arguments - takes <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    
    char *in_arg = argv[1];
    char *out_arg = argv[2];
    if( access(in_arg, F_OK) != 0 ){
        fprintf(stderr, "FindComponents Error: could not open file\n");
        exit(EXIT_FAILURE);
    }
    infile = fopen(in_arg, "r");
    outfile = fopen(out_arg, "w+");
    if( infile == NULL || outfile == NULL ){
        fprintf(stderr, "FindComponents Error: could not open files\n");
        exit(EXIT_FAILURE);
    }

    //scanning vertices from infile to set up graph, source list, and transpose
    fgets(vert, 500, infile);
    i = atoi(vert);
    Graph G = newGraph(i);
    List S = newList();
    while( fgets(vert, 500, infile) != NULL ){
        sscanf(vert, "%d %d", &u, &v);
        if( u==0 && v==0 ){
            break;
        }
        addArc(G, u, v);
    }
    Graph gT = transpose(G);
    

    //printing information to outfile
    fprintf(outfile, "Adjacency list representation of G:\n");
    printGraph(outfile, G);
    fprintf(outfile, "\n");

    //preparing source list
    for( int i = 1; i <= getOrder(G); i+=1 ){
        append(S, i);
    }
    

    //running DFS to compute SCC
    DFS(G, S);
    DFS(gT, S);
    int scc = 0;
    moveFront(S);
    while( index(S) >= 0 ){
        if( getParent(gT, get(S)) == NIL ){
            scc += 1;
        }
        moveNext(S);
    }
    fprintf(outfile, "G contains %d strongly connected components:\n", scc);
    

    //printing scc
    moveBack(S);
    int pc = 1; //printed components
    
    while( length(S) > 0 ){
        if( getParent(gT, get(S)) == NIL ){
            //printing down
            List Scc = newList();
            fprintf(outfile, "Component %d:", pc);
            pc += 1;
            int cr = 0; //components read
            while( index(S) != -1 ){
                append(Scc, get(S));
                moveNext(S);
                cr += 1;
            }
            printList(outfile, Scc);
            fprintf(outfile, "\n");
            for( int cd = 0; cd < cr; cd+=1 ){ //components deleted
                deleteBack(S);
            }
            freeList(&Scc);
            moveBack(S);
        } else {
            //searching for NIL parent
            movePrev(S);
        }
    }

    //closing files and clearing memory
    fprintf(outfile, "\n");
    fclose(infile);
    fclose(outfile);
    freeGraph(&G);
    freeGraph(&gT);
    freeList(&S);

    return(0);
}

