//-----------------------------------------------------------------------------
// Noel Ball njball pa7
// DictionaryClient.cpp
// A test client for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include"Dictionary.h"

using namespace std;

int main(){

    string s;
    int x;
    string S[] =   {
            "coming",
            "up",
            "with",
            "some",
            "new",
            "values",
            "for",
            "string",
            "S",
            "to",
            "use",
            "im",
            "looking",
            "for",
            "around",
            "ten",
            "more",
            "or",
            "so",
            "testing",
            "larger",
            "afsoiuyebagjkhagvoieubr",
            "qewfuhasdklfjbasdfuiyasedofiyugeafoiuagsdvjkhbasdfiouaygef",
            "aowieubfajksdgfaiouyegfa",
            "afeaiougfoauiyvbdavoiudaioeugfioausgvoiuagsd",
            "oof"
    };


    Dictionary A;
    Dictionary B;

    cout << endl;

    // insert some pairs into A
    for(int i=0; i<26; i++){
        A.setValue(S[i], i+1);
    }

    // call operator=()
    B = A;

    cout << "A.size() = " << A.size() << endl  << A << endl;
    cout << "B.size() = " << B.size() << endl  << B << endl;

    B.setValue("attempting", 101);
    B.setValue("to", 102);
    B.setValue("set", 103);
    B.setValue("some", 104);
    B.setValue("values", 105);

    // call copy constructor
    Dictionary C = B;

    cout << "B.size() = " << B.size() << endl  << B << endl;
    cout << "C.size() = " << C.size() << endl  << C << endl;

    // check operator==()
    cout << "A==B is " << (A==B?"true":"false") << endl;
    cout << "B==C is " << (B==C?"true":"false") << endl;
    cout << "C==A is " << (C==A?"true":"false") << endl << endl;

    // perform alterations on A
    cout << A.getValue("S") << endl;
    A.getValue("S") *= 10;
    cout << A.getValue("S") << endl << endl;

    // check state of A
    cout << "A.size() = " << A.size() << endl  << A << endl;
    cout << A.pre_string() << endl;

    // remove some pairs from A
    for(int i=0; i<13; i++){
        cout << "removing " << S[i] << " from Dictionary A" << endl;
        A.remove(S[i]);
    }

    cout << "after remove loop" << endl;

    // check state of A
    cout << "A.size() = " << A.size() << endl  << A << endl;
    cout << A.pre_string() << endl;

    // do forward iteration on A
    for(A.begin(); A.hasCurrent(); A.next()){
        s = A.currentKey();
        x = A.currentVal();
        cout << "("+s+", " << x << ") ";
    }
    cout << endl << endl;

    // do reverse iteration on A
    for(A.end(); A.hasCurrent(); A.prev()){
        s = A.currentKey();
        x = A.currentVal();
        cout << "("+s+", " << x << ") ";
    }
    cout << endl << endl;

    return( EXIT_SUCCESS );
}