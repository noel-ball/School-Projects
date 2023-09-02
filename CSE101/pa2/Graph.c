//------------------------------------------------------------------------------
// Noel Ball njball pa2
//------------------------------------------------------------------------------

#include "Graph.h"
#include <stdlib.h>

// Definitions ---------------------------------------------------
#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct GraphObj {
    List *adj;
    int *color;
    int *parent;
    int *distance;
    int order;
    int size;
    int source;
} GraphObj;

// Constructors-Destructors ---------------------------------------------------

//allocates graph, sets fields
Graph newGraph(int n){
    Graph G = malloc(sizeof(GraphObj));
    G->adj = calloc(n+1, sizeof(List));
    G->color = calloc(n+1, sizeof(int));
    G->parent = calloc(n+1, sizeof(int));
    G->distance = calloc(n+1, sizeof(int));
    G->order = n;
    G->size = 0;
    G->source = NIL;
    for( int i = 1; i <= n; i+=1 ){
        G->adj[i] = newList();
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->distance[i] = INF;
    }
    return(G);
}

//frees graph fields and pointer
void freeGraph(Graph* pG){
    if( pG!=NULL && *pG!=NULL ){
        Graph G = *pG;
        for ( int i = 1; i <= getOrder(G); i+=1 ){
            freeList(&(G->adj[i]));
        }
        free(G->adj);
        free(G->color);
        free(G->parent);
        free(G->distance);
        free(*pG);
        *pG = NULL;
    }
}

// Access functions -----------------------------------------------------------

int getOrder(Graph G){
    return G->order;
}

int getSize(Graph G){
    return G->size;
}

int getSource(Graph G){
    return G->source;
}

int getParent(Graph G, int u){
    return G->parent[u];
}

int getDist(Graph G, int u){
    return G->distance[u];
}

void getPath(List L, Graph G, int u){
    if( getSource(G)==NIL ){
        fprintf(stderr, "Graph Error: calling getPath() with no source\n");
        exit(EXIT_FAILURE);
    }
    if( u < 1 || u > getOrder(G) ){
        fprintf(stderr, "Graph Error: calling getPath() out of bounds\n");
        exit(EXIT_FAILURE);
    }
    //finds shortest path to u from source
    if( u == getSource(G) ){
        append(L, getSource(G));
    } else if( getParent(G, u)==NIL ){
        append(L, NIL);
    } else {
        getPath(L, G, getParent(G, u));
        append(L, u);
    }
}

// Manipulation procedures ----------------------------------------------------

void makeNull(Graph G){
    //clears all adjacent vertices
    for( int i = 1; i <= getOrder(G); i+=1 ){
        clear(G->adj[i]);
    }
    G->size = 0;
}

void addEdge(Graph G, int u, int v){
    if( u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G) ){
        fprintf(stderr, "Graph Error: calling addEdge() out of bounds\n");
        exit(EXIT_FAILURE);
    }
    //adds arcs both ways between u and v, only counts as 1 size
    addArc(G, u, v);
    addArc(G, v, u);
    G->size -= 1;
}

void addArc(Graph G, int u, int v){
    if( u < 1 || u > getOrder(G) || v < 1 || v > getOrder(G) ){
        fprintf(stderr, "Graph Error: calling addArc() out of bounds\n");
        exit(EXIT_FAILURE);
    }
    //compares v to u's adjacent vertices to append correctly, adds 1 size
    if( length(G->adj[u]) == 0 ){
        append(G->adj[u], v);
    } else {
        moveFront(G->adj[u]);
        while( index(G->adj[u]) != -1 && v > get(G->adj[u]) ){
            moveNext(G->adj[u]);
        }
        if( index(G->adj[u]) == -1 ) {
            append(G->adj[u], v);
        } else if( get(G->adj[u]) == v ){
            return;
        } else {
            insertBefore(G->adj[u], v);
        }
    }
    G->size += 1;
}

void BFS(Graph G, int s){
    //performs Breadth First Search from source vertex s
    for( int k = 1; k <= getOrder(G); k+=1 ){
        G->color[k] = WHITE;
        G->parent[k] = NIL;
        G->distance[k] = INF;
    }
    G->source = s;
    G->color[s] = GRAY;
    G->distance[s] = 0;
    G->parent[s] = NIL;
    List nL = newList();
    append(nL, s);
    while( length(nL)!=0 ){
        int f = front(nL);
        deleteFront(nL);
        List adj = G->adj[f];
        for( moveFront(adj); index(adj) != -1; moveNext(adj) ){
            int a = get(adj);
            if (G->color[a] == WHITE) {
                G->color[a] = GRAY;
                G->distance[a] = G->distance[f] + 1;
                G->parent[a] = f;
                append(nL, a);
            }
        }
        G->color[f] = BLACK;
    }
    freeList(&nL);
}

// Other operations -----------------------------------------------------------

void printGraph(FILE* out, Graph G) {
    //iterates through adjacency to print every list in graph
    for (int i = 1; i <= getOrder(G); i++) {
        if( length(G->adj[i]) != 0 ){
            fprintf(out, "%d: ", i);
            printList(out, G->adj[i]);
            fprintf(out, "\n");
        }
    }
}
