//-----------------------------------------------------------------------------
// Noel Ball njball pa7
// Order.cpp
// Client file for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include"Dictionary.h"

using namespace std;

int main( int argc, char** argv ){
    if( argc!=3 ){
        throw invalid_argument("Order: Requires exactly two arguments: <input> and <output> files");
        return 1;
    }

    fstream infile(argv[1], fstream::in);
    fstream outfile(argv[2], fstream::out);

    // Parsing infile
    Dictionary In;
    keyType key;
    valType val = 1;
    while( std::getline(infile, key) ){
        In.setValue(key, val);
        val += 1;
    }

    // Printing data to outfile
    outfile << In.to_string() << endl << In.pre_string() << endl;
    // Closing files
    infile.close();
    outfile.close();
    return 0;
}