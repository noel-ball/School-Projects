//------------------------------------------------------------------------------
// Noel Ball njball pa3
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
    int *discover;
    int *finish;
    int order;
    int size;
} GraphObj;

// Constructors-Destructors ---------------------------------------------------

//allocates graph, sets fields
Graph newGraph(int n){
    Graph G = malloc(sizeof(GraphObj));
    G->adj = calloc(n+1, sizeof(List));
    G->color = calloc(n+1, sizeof(int));
    G->parent = calloc(n+1, sizeof(int));
    G->discover = calloc(n + 1, sizeof(int));
    G->finish = calloc(n + 1, sizeof(int));
    G->order = n;
    G->size = 0;
    for( int i = 1; i <= n; i+=1 ){
        G->adj[i] = newList();
        G->color[i] = WHITE;
        G->parent[i] = NIL;
        G->discover[i] = UNDEF;
        G->finish[i] = UNDEF;
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
        free(G->discover);
        free(G->finish);
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

int getParent(Graph G, int u){
    return G->parent[u];
}

int getDiscover(Graph G, int u){
    if( u < 1 || u > getOrder(G) ){
        fprintf(stderr, "Graph Error: calling getDiscover() out of range\n");
        exit(EXIT_FAILURE);
    }
    return G->discover[u];
}

int getFinish(Graph G, int u){
    if( u < 1 || u > getOrder(G) ){
        fprintf(stderr, "Graph Error: calling getFinish() out of range\n");
        exit(EXIT_FAILURE);
    }
    return G->finish[u];
}

// Manipulation procedures ----------------------------------------------------

void makeNull(Graph G){
    //clears all adjacent vertices
    for( int i = 1; i <= getOrder(G); i+=1 ){
        clear(G->adj[i]);
        G->color[i] = WHITE;
        G->parent[i] = NIL;
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

void Visit(Graph G, List L, int s, int *time){
    int adj;
    //discovered node
    G->color[s] = GRAY;
    (*time) += 1;
    G->discover[s] = *time;

    //searches deeper through adjacent nodes
    List Adj = G->adj[s];
    moveFront(Adj);
    for( int i = 0; index(Adj) != -1; i += 1 ){
        adj = get(Adj);
        if( G->color[adj] == WHITE ){
            G->parent[adj] = s;
            Visit(G, L, adj, time);
        }
        moveNext(Adj);
    }

    //all nodes discovered
    prepend(L, s);
    G->color[s] = BLACK;
    (*time) += 1;
    G->finish[s] = *time;
}

void DFS(Graph G, List S){
    if( G==NULL ){
        fprintf(stderr, "Graph Error: calling DFS() on NULL Graph\n");
        exit(EXIT_FAILURE);
    }
    if( S==NULL ){
        fprintf(stderr, "Graph Error: calling DFS() on NULL List\n");
        exit(EXIT_FAILURE);
    }
    if( length(S) != getOrder(G) ){
        fprintf(stderr, "Graph Error: calling DFS() with a differently sized Graph and List\n");
        exit(EXIT_FAILURE);
    }
    //performs Depth First Search from source vertex s, determined by List S
    for( int i = 1; i <= length(S); i+=1 ){
        G->color[i] = WHITE;
        G->parent[i] = NIL;
    }
    int time = 0;
    List nL = newList();
    moveFront(S);
    while( index(S) != -1 ){
        int s = get(S);
        if( G->color[s] == WHITE ){
            Visit(G, nL, s, &time);
        }
        moveNext(S);
    }
    //arranges S as output
    clear(S);
    moveFront(nL);
    for( int i = 0; index(nL) != -1; i+=1 ){
        append(S, get(nL));
        moveNext(nL);
    }
    freeList(&nL);
}

// Other operations -----------------------------------------------------------

//computes Graph Transpose
Graph transpose(Graph G){
    Graph gT = newGraph(getOrder(G));
    for( int i = 1; i <= getOrder(G); i+=1 ){
        moveFront(G->adj[i]);
        for( int j = 0; index(G->adj[i]) != -1; j+=1 ){
            addArc(gT, get(G->adj[i]), i);
            moveNext(G->adj[i]);
        }
    }
    return(gT);
}

//creates a copy of Graph
Graph copyGraph(Graph G){
    Graph gC = newGraph(getOrder(G));
    for( int i = 1; i <= getOrder(G); i+=1 ){
        moveFront(G->adj[i]);
        for( int j = 1; index(G->adj[i]) != -1; j+=1 ){
            addArc(gC, get(G->adj[i]), i);
            moveNext(G->adj[i]);
        }
    }
    return(gC);
}

//prints Graph to outfile
void printGraph(FILE* out, Graph G){
    //iterates through adjacency to print every list in graph
    for( int i = 1; i <= getOrder(G); i+=1 ){
        fprintf(out, "%d:", i);
        printList(out, G->adj[i]);
        fprintf(out, "\n");
    }
}

