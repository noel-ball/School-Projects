//-----------------------------------------------------------------------------
// Noel Ball njball pa6
// Arithmetic.cpp
// Executable client module for BigInteger ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include<fstream>
#include"BigInteger.h"
using namespace std;

int main( int argc, char ** argv){
    // Handling file inputs
    if( argc != 3 ){
        throw invalid_argument("Arithmetic Error: Needs exactly two arguments <input> <output>");
    }
    fstream infile (argv[1], fstream::in);
    fstream outfile (argv[2], fstream::out);
    // Parsing infile for BigIntegers
    string sA;
    string ignore;
    string sB;
    std::getline(infile, sA);
    std::getline(infile, ignore);
    std::getline(infile, sB);
    // Creating BigIntegers
    BigInteger A = BigInteger(sA);
    BigInteger B = BigInteger(sB);


    // BigInteger scalers
    BigInteger Two = BigInteger(2);
    BigInteger Three = BigInteger(3);
    BigInteger Nine = BigInteger(9);
    BigInteger SixT = BigInteger(16);

    // Add/sub calculations
    outfile << A << endl << endl;
    outfile << B << endl << endl;

    BigInteger add = A+B;
    outfile << add << endl << endl;

    BigInteger sub = A-B;
    outfile << sub << endl << endl;

    sub = A-A;
    outfile << sub << endl << endl;

    // Mul calculations
    BigInteger x = (Three*A);
    BigInteger y = (Two*B);
    BigInteger z = x-y;
    outfile << z << endl << endl;

    BigInteger mul = A*B;
    outfile << mul << endl << endl;

    mul = A*A;
    outfile << mul << endl << endl;

    mul = B*B;
    outfile << mul << endl << endl;

    x = A;
    for( int i=0; i < 3; i+=1 ){ x *= A; }
    x *= Nine;
    y = B;
    for( int i=0; i < 4; i+=1 ){ y *= B; }
    y *= SixT;
    z = x + y;
    outfile << z << endl << endl;

}