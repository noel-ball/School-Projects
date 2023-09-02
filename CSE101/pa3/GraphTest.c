//------------------------------------------------------------------------------
// Noel Ball njball pa3
//------------------------------------------------------------------------------

#include <stdio.h>
#include "Graph.h"

int main(int argc, char* argv[]){
    Graph A = newGraph(1);
    Graph B = newGraph(2);
    Graph C = newGraph(3);
    Graph G = newGraph(50);
    List L = newList();

    //tests getSize with addEdge and addArc
    addEdge(B, 1, 2);
    addArc(B, 2, 1);


    //print adjacency list representation of G and order
    printGraph(stdout, B);
    
    //tests DFS and Graph transpose, copy, and print
    Graph gT = transpose(A);
    for( int i = 1; i <= getOrder(A); i+=1 ){
        append(L, i);
    }
    DFS(A, L);
    DFS(gT, L);
    Graph D = copyGraph(A);

    printGraph(stdout, D);
    printf("Passed Graph Changes!\n");

    // Free objects
    freeGraph(&A);
    freeGraph(&B);
    freeGraph(&C);
    freeGraph(&D);
    freeGraph(&G);
    freeGraph(&gT);
    freeList(&L);

    printf("All tests passed!\n");
    return(0);
}

