//-----------------------------------------------------------------------------
// Noel Ball njball pa1
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdbool.h>
#include "List.h"

// structs --------------------------------------------------------------------

// private Node type
typedef struct NodeObj* Node;

// private NodeObj type
typedef struct NodeObj{
    ListElement data;
    Node next;
    Node prev;
} NodeObj;

// private ListObj type
typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;


// Constructors-Destructors ---------------------------------------------------

// Returns reference to new Node object. Initializes next and data fields.
Node newNode(ListElement data){
    Node N = malloc(sizeof(NodeObj));
    assert( N!=NULL );
    N->data = data;
    N->next = N->prev = NULL;
    return(N);
}

// Frees heap memory pointed to by *pN, sets *pN to NULL.
void freeNode(Node* pN){
    if( pN!=NULL && *pN!=NULL ){
        free(*pN);
        *pN = NULL;
    }
}

// Returns reference to new empty List object.
List newList(void){
    List L;
    L = malloc(sizeof(ListObj));
    assert( L!=NULL );
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return(L);
}

// Frees all heap memory associated with List *pL, and sets *pL to NULL.
void freeList(List* pL){
    if ( pL==NULL || *pL==NULL ){
        fprintf(stderr, "List Error: calling freeList() on NULL List reference\n");
    }
    clear(*pL);
    free(*pL);
    *pL = NULL;
}


// Access functions -----------------------------------------------------------

// Returns the number of elements in L.
int length(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling Length() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    return( L->length );
}

// Returns index of cursor element if defined, -1 otherwise.
int index(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling index() on NULL list reference");
        exit(EXIT_FAILURE);
    }

    return( L->index );
}

// Returns front element of L. Pre: length()>0
ListElement front(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling front() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling front() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    return( L->front->data );
}

// Returns back element of L. Pre: length()>0
ListElement back(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling back() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling back() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    return( L->back->data );
}

// Returns cursor element of L. Pre: length()>0, index()>=0
ListElement get(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling get() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling get() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == -1 ){
        fprintf(stderr, "List Error: calling get() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    return( L->cursor->data );
}

// Returns true if Lists A and B are in same state, and returns false otherwise.
bool equals(List A, List B){
    if( A==NULL || B==NULL ){
        fprintf(stderr, "List Error: calling equals() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    bool eq;
    Node N, M;

    eq = ( A->length == B->length );
    N = A->front;
    M = B->front;
    while( eq && N!=NULL ){
        eq = ( N->data==M->data );
        N = N->next;
        M = M->next;
    }
    return eq;
}

// Manipulation procedures ----------------------------------------------------

// Resets L to its original empty state by deleting the newest elements until there are none
void clear(List L){
    moveFront(L);
    while( L->length > 0 ){
        deleteFront(L);
    }
}

// Overwrites the cursor element’s data with x. Pre: length()>0, index()>=0
void set(List L, ListElement x){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling set() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling set() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == -1 ){
        fprintf(stderr, "List Error: calling set() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    //sets cursor element to x
    L->cursor->data = x;
}

// If L is non-empty, sets cursor under the front element, otherwise does nothing.
void moveFront(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling moveFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length == 0 ){ return; }
    L->cursor = L->front;
    L->index = 0;
}

// If L is non-empty, sets cursor under the back element, otherwise does nothing.
void moveBack(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if ( L->length == 0 ){ return; }
    L->cursor = L->back;
    L->index = ( L->length-1 );
}

// If cursor is defined and not at front, move cursor one step toward the front of L;
// if cursor is defined and at front, cursor becomes undefined; if cursor is undefined do nothing
void movePrev(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling movePrev() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->cursor == NULL ){ return; }
    if( L->index <= 0 ){
        L->cursor = NULL;
        L->index = -1;
        return;
    }
    L->cursor = L->cursor->prev;
    L->index -= 1;
}

// If cursor is defined and not at back, move cursor one step toward the back of L;
// if cursor is defined and at back, cursor becomes undefined; if cursor is undefined do nothing
void moveNext(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling moveNext() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->cursor == NULL ){ return; }
    if( L->index >= (L->length-1) ){
        L->cursor = NULL;
        L->index = -1;
        return;
    }
    L->cursor = L->cursor->next;
    L->index += 1;
}

// Insert new element into L. If L is non-empty, insertion takes place before front element.
void prepend(List L, ListElement x){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling prepend() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if( L->length != 0 ){
        Node pre_N = newNode(x);
        L->front->prev = pre_N; //puts prepend previous to the front
        pre_N->next = L->front; //and front next to the prepend
        L->front = pre_N; //prepend is new front
        L->length += 1;
        if ( L->index >= 0 ){
            L->index += 1;
        }
        return;
    }
    Node pre_N = newNode(x); //prepend in empty list
    L->front = L->back = pre_N; //prepend is front and back
    L->length += 1;
}

// Insert new element into L. If L is non-empty, insertion takes place after back element.
void append(List L, ListElement x) {
    if( L==NULL ){
        fprintf(stderr, "List Error: calling append() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    if (L->length != 0) {
        Node app_N = newNode(x);
        L->back->next = app_N; //puts append next to the back
        app_N->prev = L->back; //and back previous to the append
        L->back = app_N; //append is new back
        L->length += 1;
        return;
    }
    Node app_N = newNode(x); //append in empty list
    L->front = L->back = app_N; //append is front and back
    L->length += 1;
}

// Insert new element before cursor. Pre: length()>0, index()>=0
void insertBefore(List L, int x){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling insertBefore() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling insertBefore() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == -1 ){
        fprintf(stderr, "List Error: calling insertBefore() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if( L->index == 0 ){
        prepend(L, x); //to ensure front is set correctly when inserting before first element
        return;
    }

    Node iB_N = newNode(x);
    L->cursor->prev->next = iB_N; //puts insert next to the node previous to cursor
    iB_N->prev = L->cursor->prev; //previous node to insert is node previous to cursor
    L->cursor->prev = iB_N; //previous to cursor is now insert
    iB_N->next = L->cursor; //node next to insert is now cursor
    L->length += 1;
    L->index += 1; //cursor moves forward because insert is previous to cursor
}

// Insert new element after cursor. Pre: length()>0, index()>=0
void insertAfter(List L, int x){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling insertAfter() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling insertAfter() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == -1 ){
        fprintf(stderr, "List Error: calling insertAfter() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    if( L->index == (L->length-1) ){
        append(L, x); //to ensure back is set correctly when inserting after last element
        return;
    }

    Node iA_N = newNode(x);
    L->cursor->next->prev = iA_N; //puts insert prev to the node next to cursor
    iA_N->next = L->cursor->next; //next node to insert is node next to cursor
    L->cursor->next = iA_N; //next to cursor is now insert
    iA_N->prev = L->cursor; //node previous to insert is now cursor
    L->length += 1;
}

// Delete the front element. Pre: length()>0
void deleteFront(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling deleteFront() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling deleteFront() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == 0 ){
        //cursor is undefined if deleted
        L->cursor = NULL;
    }
    if ( L->index != -1 ){
        L->index -= 1; //corrects cursor position unless undefined
    }
    if( L->length > 1 ){
        Node nF_N = L->front->next; //new front node is next to front
        freeNode(&L->front);
        L->front = nF_N; //new front
        L->front->prev = NULL;
        L->length -= 1;
    } else {
        freeNode(&L->front);
        L->front = L->back = NULL;
        L->length = 0;
    }
}

// Delete the back element. Pre: length()>0
void deleteBack(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling deleteBack() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling deleteBack() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    if( L->index == (L->length-1) ){
        //cursor is undefined if deleted
        L->index = -1;
        L->cursor = NULL;
    }

    if( L->length > 1 ){
        Node nB_N = L->back->prev; //new back node is prev to back
        freeNode(&L->back);
        L->back = nB_N; //new back
        L->back->next = NULL;
        L->length -= 1;
    } else {
        freeNode(&L->back);
        L->front = L->back = NULL;
        L->length = 0;
    }

}

// Delete cursor element, making cursor undefined. Pre: length()>0, index()>=0
void delete(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling delete() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling delete() on an empty List\n");
        exit(EXIT_FAILURE);
    }
    if( L->index == -1 ){
        fprintf(stderr, "List Error: calling delete() with undefined cursor\n");
        exit(EXIT_FAILURE);
    }

    //deleting front or back
    if( L->index == 0 ){
        deleteFront(L);
        return;
    }
    if( L->index == (L->length-1) ){
        deleteBack(L);
        return;
    }

    //next to prev of cursor becomes next to cursor, and vice versa
    L->cursor->prev->next = L->cursor->next;
    L->cursor->next->prev = L->cursor->prev;
    freeNode(&L->cursor);
    //making cursor undefined when deleted
    L->length -= 1;
    L->index = -1;
}

// Other Functions ------------------------------------------------------------

void printList(FILE* out, List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling printList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }
    if( L->length == 0 ){
        fprintf(stderr, "List Error: calling printList() on an empty List\n");
        exit(EXIT_FAILURE);
    }

    //saving index of cursor, and setting to front
    int s_index = L->index;
    moveFront(L);
    //iterating over elements with cursor to print to out
    while( L->index != -1 ){
        fprintf(out, "%i ", L->cursor->data);
        moveNext(L);
    }
    //restoring original cursor
    L->index = s_index;
}

List copyList(List L){
    if( L==NULL ){
        fprintf(stderr, "List Error: calling copyList() on NULL List reference\n");
        exit(EXIT_FAILURE);
    }

    //saving index of cursor, and setting to front
    int s_index = L->index;
    moveFront(L);
    //creating new list, and iterating over elements with cursor to copy over
    List C = newList();
    while( L->index != -1 ){
        append(C, L->cursor->data);
        moveNext(L);
    }

    //restoring original cursor
    L->index = s_index;
    C->index = s_index;
    return C;
}
