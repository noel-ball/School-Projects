//-----------------------------------------------------------------------------
// Noel Ball njball pa5
// ListTest.cpp
// Client Testing Harness for List ADT
//-----------------------------------------------------------------------------

#include <iostream>
#include "List.h"

using namespace std;
int main(){
    List A;
    List B;
    List C;
    int count;
    int i = 1;
    int j = 10;
    int k = 100;

    while( count <= 5 ){
        A.insertBefore(count*i);
        B.insertBefore(count*j);
        C.insertBefore(count*k);
        count += 1;
    }
    count = 1;

    cout << "A: " << A << endl;
    cout << "B: " << B << endl;
    cout << "C: " << C << endl;
    cout << endl;

    A.moveFront();
    B.moveFront();
    B.moveNext();
    B.moveNext();
    C.moveBack();

    cout << "Cursor A: " << A.position() << endl;
    cout << "Cursor B: " << B.position() << endl;
    cout << "Cursor C: " << C.position() << endl;
    cout << endl;

    A.moveFront();
    cout << A.findNext(3) << endl;
    B.moveBack();
    cout << B.findPrev(30) << endl;
    List D = A;
    List E = B;
    List F = A;
    cout << "D == E: " << ( D==E?"True":"False" ) << endl;
    cout << "D == F: " << ( D==F?"True":"False" ) << endl;
    A.cleanup();
    B.cleanup();
    C.cleanup();
    D.cleanup();
    E.cleanup();
    F.cleanup();
    cout << "All Cleaned!" << endl;
    cout << endl;
    cout << "Testing Harness Finished!" << endl;

    return 0;
}