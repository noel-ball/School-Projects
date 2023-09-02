//-----------------------------------------------------------------------------
// Noel Ball njball pa1
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <assert.h>
#include "List.h"

int main(int argc, char* argv[]){

    List A = newList();
    List B = newList();
    List C = newList();
    List D = NULL;

    //testing append, prepend, and printList
    for( int i = 1; i <= 10; i+=2 ) {
        append(A,i);
        prepend(B,i);
    }
    for( int i = 0; i <= 2; i+=1 ) {
        append(C, i);
    }
    for( int i = -1; i >= -2; i-=1 ){
        prepend(C, i);
    }

    printf("Got:\n");
    printList(stdout,A);
    printf("\n");
    printList(stdout,B);
    printf("\n");
    printList(stdout,C);
    printf("\n");
    printf("\nExpected: \n1 3 5 7 9\n9 7 5 3 1\n-2 -1 0 1 2\n----------\n");

    //testing indexes and get()
    printf("Got:\n");
    moveFront(A);
    while( index(A) >= 0 ){
        printf("%d ", get(A));
        moveNext(A);
    }
    printf("\n");

    moveBack(B);
    while( index(B) >= 0 ){
        printf("%d ", get(B));
        movePrev(B);
    }
    printf("\n");
    printf("\nExpected: \n1 3 5 7 9\n1 3 5 7 9\n----------\n");

    //testing out of bounds
    moveFront(C);
    movePrev(C);
    assert(index(C) == -1);
    moveBack(C);
    moveNext(C);
    assert(index(C) == -1);
    printf("Passed asserts testing bounds!\n----------\n");

    //testing copyList() and equals()
    printf("Got:\n");
    D = copyList(C);
    printf("%s\n", equals(A,B)?"true":"false");
    printf("%s\n", equals(C,D)?"true":"false");
    freeList(&A);
    A = copyList(C);
    printf("%s\n", equals(D,A)?"true":"false");
    freeList(&A);
    A = copyList(B);
    printf("\nExpected: \nfalse\ntrue\ntrue\n----------\n");

    //testing insert before/after
    moveFront(A);
    insertAfter(A, 2);
    int k=1;
    while( k<=5 ){
        moveNext(A);
        k+=1;
    }
    insertBefore(A, 8);

    moveFront(B);
    moveNext(B);
    insertAfter(B, 4);
    k=1;
    while( k<=4 ){
        moveNext(B);
        k+=1;
    }
    insertBefore(B, 6);
    printf("Got:\n");
    printList(stdout, A);
    printf("\n");
    printList(stdout, B);
    printf("\n");
    printf("\nExpected: \n9 2 7 5 3 8 1\n9 7 4 5 3 6 1\n----------\n");

    //testing delete, and delete front/back
    moveFront(D);
    moveNext(D);
    moveNext(D);
    delete(D);
    printf("Got:\n");
    printList(stdout,D);
    printf("\n");
    deleteFront(A);
    deleteBack(B);
    printList(stdout, A);
    printf("\n");
    printList(stdout, B);
    printf("\n");
    printf("\nExpected: \n-2 -1 1 2\n2 3 5 7 8 9\n1 3 4 5 6 7\n----------\n");

    //testing clear
    printf("Got:\n");
    printf("%d ", length(C));
    clear(C);
    printf("%d\n", length(C));
    printf("\nExpected: \n5 0\n----------\n");

    //testing frees
    freeList(&A);
    freeList(&B);
    freeList(&C);
    freeList(&D);
    printf("Freed all the lists!\n");

    return(0);
}
