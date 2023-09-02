//-----------------------------------------------------------------------------
// Noel Ball njball pa2
//-----------------------------------------------------------------------------

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main(int argc, char* argv[]) {
    int i, u, v;
    //setting up command line arguments for file handling
    FILE *infile, *outfile;
    if (argc != 3) {
        fprintf(stderr, "FindPath Error: incorrect arguments - takes <input file> <output file>\n");
        exit(EXIT_FAILURE);
    }
    char *in_arg = argv[1];
    char *out_arg = argv[2];
    if (access(in_arg, F_OK) != 0) {
        fprintf(stderr, "FindPath Error: could not open file\n");
        return 1;
    }
    infile = fopen(in_arg, "r");
    outfile = fopen(out_arg, "w");
    if(infile == NULL || outfile == NULL) {
        fprintf(stderr, "FindPath Error: could not open files\n");
        exit(EXIT_FAILURE);
    }

    //scanning vertices from infile to set up graph
    if(fscanf(infile, "%d", &i) != 1) {
        fprintf(stderr, "FindPath Error: could not scan vertices\n");
        exit(EXIT_FAILURE);
    }
    Graph G = newGraph(i);
    while(fscanf(infile, "%d %d", &u, &v) == 2 && u != 0 && v != 0) {
        addEdge(G, u, v);
    }

    //printing information to outfile
    printGraph(outfile, G);
    while (fscanf(infile, "%d %d", &u, &v) == 2 && u != 0 && v != 0) {
        fprintf(outfile, "\n");
        BFS(G, u);
        int distance = getDist(G, v);
        if (distance == INF) {
            fprintf(outfile, "The distance from %d to %d is infinity\n", u, v);
            fprintf(outfile, "No %d-%d path exists\n", u, v);
        } else {
            fprintf(outfile, "The distance from %d to %d is %d\n", u, v, distance);
            fprintf(outfile, "A shortest %d-%d path is: ", u, v);
            List path = newList();
            getPath(path, G, v);
            printList(outfile, path);
            freeList(&path);
        }
    }

    //closing files and clearing memory
    fprintf(outfile, "\n");
    fclose(infile);
    fclose(outfile);
    freeGraph(&G);

    return 0;
}
