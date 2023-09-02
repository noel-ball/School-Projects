//-----------------------------------------------------------------------------
// Noel Ball njball pa4
//-----------------------------------------------------------------------------

#pragma once
#include <stdbool.h>

// Definitions ---------------------------------------------------
typedef void* ListElement;
typedef struct ListObj* List;

// Constructors-Destructors ---------------------------------------------------
List newList(void);
void freeList(List* pL);

// Access functions -----------------------------------------------------------
int length(List L);
int index(List L);
ListElement front(List L);
ListElement back(List L);
ListElement get(List L);

// Manipulation procedures ----------------------------------------------------
void clear(List L);
void set(List L, ListElement x);
void moveFront(List L);
void moveBack(List L);
void movePrev(List L);
void moveNext(List L);
void prepend(List L, ListElement x);
void append(List L, ListElement x);
void insertBefore(List L, ListElement x);
void insertAfter(List L, ListElement x);
void deleteFront(List L);
void deleteBack(List L);
void delete(List L);
