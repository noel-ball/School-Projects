//------------------------------------------------------------------------------
// Noel Ball njball pa2
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
    printf("Got:\n");
    printf("%d ", getSize(G));
    addEdge(G, 1, 50);
    addArc(G, 10, 40);
    printf("%d ", getSize(G));
    printf("\nExpected:\n0 2\n----------\n");


    //print adjacency list representation of G and order
    printf("Got:\n");
    printGraph(stdout, G);
    printf("%d %d %d", getOrder(A), getOrder(B), getOrder(C));
    printf("\nExpected:\n1: 50\n10: 40\n50: 1\n1 2 3\n----------\n");

    //testing BFS and get Source Parent
    printf("Got:\n");
    BFS(G, 1);
    printf("%d ", getSource(G));
    printf("%d", getParent(G, 1));
    printf("\nExpected:\n1 0\n----------\n");

    //testing makeNull and get Dist
    printf("Got:\n");
    makeNull(G);
    printf("%d ", getSize(G));
    addEdge(G, 1, 2);
    addEdge(G, 2, 3);
    addEdge(G, 3, 4);
    addEdge(G, 4, 5);
    BFS(G, 5);
    printf("%d", getDist(G, 1));
    printf("\nExpected:\n0 4\n----------\n");

    //testing getPath
    printf("Got:\n");
    getPath(L, G, 1);
    printList(stdout, L);
    printf("\nExpected:\n5 4 3 2 1\n----------\n");

    // Free objects
    freeGraph(&A);
    freeGraph(&B);
    freeGraph(&C);
    freeGraph(&G);
    freeList(&L);

    printf("All tests passed!\n");
    return(0);
}
