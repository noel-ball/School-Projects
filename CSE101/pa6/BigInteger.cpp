//-----------------------------------------------------------------------------
// Noel Ball njball pa6
// BigInteger.cpp
// Implementation file for BigInteger ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include"BigInteger.h"
using namespace std;

// Global Constants -----------------------------------------------------------
const int power = 9;
const long base = 1000000000;

// Class Constructors & Destructors -------------------------------------------

// Constructor that creates a new BigInteger in the zero state:
BigInteger::BigInteger(){
    // Sign 0 is empty, and digits remains empty
    signum = 0;
}

// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x){
    // Setting sign of x and finding absolute value
    if( x>0 ){
        signum = 1;
    } else if( x<0 ){
        signum = -1;
        x *= 1;
    } else {
        signum = 0;
    }
    // Finding all digits of normalized x
    long lDig;
    // If single digit
    if( x < base ){
        digits.insertAfter(x);
        x = 0;
    }
    while( x != 0 ){
        // Since > 1 digit, mod by base to find last digit
        lDig = x % base;
        digits.insertAfter(lDig);
        // Remove last digit to find next
        x = x / lDig;
    }
}

// Constructor that creates a new BigInteger from the string s.
BigInteger::BigInteger(std::string s){
    if( s.empty() ){
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    // Setting sign of x and removing indicator from string
    if( s[0]=='+' ){
        signum = 1;
        s.erase(0,1);
    } else if( s[0]=='-' ){
        signum = -1;
        s.erase(0,1);
    } else signum = 1;
    // Removing front zero spacing
    while( s[0] == '0' ){ s.erase(0, 1); }

    // Parsing string for all substrings of normalized digits
    while( s.length() >= power ){
        string sDig;
        long lDig;
        // Taking normalized digit as substring of s
        sDig = s.substr(s.length()-power, power);
        // Removing substring digit zero spacing
        while( sDig[0] == '0' ){ sDig.erase(0,1); }
        // Converting substring to long
        if( sDig.empty()!=1 ){
            lDig = std::stol(sDig, 0, 10);
            digits.insertAfter(lDig);
        } else { digits.insertAfter(0); }
        // Remove last digit to find next
        s.erase(s.length()-power, power);
    }
    if( s.length() > 0 ){
        // Converting the rest of s to long
        long fDig;
        fDig = std::stol(s, 0, 10);
        digits.insertAfter(fDig);
    }
    digits.moveFront();
    if( digits.length()==1 && digits.peekNext()==0 ){ signum = 0; }
}

// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
    signum = N.signum;
    digits = N.digits;
}

// Access functions --------------------------------------------------------

// Returns -1, 1 or 0 according to the sign of BigInteger
int BigInteger::sign() const{
    return signum;
}

// Returns -1, 1 or 0 according to BigInteger <, >, or = to N
int BigInteger::compare(const BigInteger& N) const{
    BigInteger A = *this;
    BigInteger B = N;

    if( A.signum == B.signum && A.digits == B.digits ) return 0;
    else if( A.signum > B.signum ) return 1;
    else if( A.signum < B.signum ) return -1;
    else if( A.digits.length() > B.digits.length() ) return signum;
    else if( A.digits.length() < B.digits.length() ) return -signum;

    // Sign and digit length are shared
    A.digits.moveFront();
    B.digits.moveFront();
    while( A.digits.length() > 0 && B.digits.length() > 0 ){
        if( A.digits.peekNext() != B.digits.peekNext() ){
            long lDigA = A.digits.peekNext();
            long lDigB = B.digits.peekNext();
            if( (lDigA > lDigB && signum == 1) || (lDigA < lDigB && signum == -1) ){
                return 1;
            } else if( (lDigA < lDigB && A.signum == 1) || (lDigA > lDigB && A.signum == -1) ){
                return -1;
            }
        }
        A.digits.eraseAfter();
        B.digits.eraseAfter();
    }

    // Every digit is shared
    return 0;
}

// Manipulation procedures -------------------------------------------------

// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
    signum = 0;
    digits.clear();
}

// Reverses sign if not zero
void BigInteger::negate(){
    if( signum!=0 ) signum *= -1;
}

// BigInteger Arithmetic operations ----------------------------------------

// Returns a BigInteger representing the sum of this and N
BigInteger BigInteger::add(const BigInteger& N ) const{
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger EQ; // this + N
    BigInteger Large;

    // Establishing A and B are not opposite signs, setting eq sign
    if( A.sign() != B.sign() && A.sign()!=0 && B.sign()!=0 ){
        // Subtracting in the correct order will return the correct answer
        B.negate();
        return A.sub(B);
    }
    if( A.sign() != 0 ){ EQ.signum = A.sign(); }
    else if( B.sign() != 0 ){ EQ.signum = B.sign(); }

    // All normalized digits add together
    long lDigA;
    long lDigB;
    long lDig_EQ;
    long Rem = 0; // Remainder
    A.digits.moveBack();
    B.digits.moveBack();
    while( A.digits.length() > 0 || B.digits.length() > 0 ){
        // Adds spacing if needed
        if( A.digits.position() > 0 ){
            lDigA = A.digits.peekPrev();
        } else { lDigA = 0; }
        if( B.digits.position() > 0 ){
            lDigB = B.digits.peekPrev();
        } else { lDigB = 0; }

        // Calculating the digit
        lDig_EQ = (lDigA + lDigB) + Rem;
        Rem = 0; // Remainder used
        while( lDig_EQ >= base ){
            lDig_EQ -= base; // Normalized
            Rem += 1; // Next remainder
        }
        EQ.digits.insertAfter(lDig_EQ);
        if( A.digits.position() > 0 ){ A.digits.eraseBefore(); }
        if( B.digits.position() > 0 ){ B.digits.eraseBefore(); }
    }
    // Adding final remainder if any
    if( Rem > 0 ){
        EQ.digits.insertAfter(Rem);
    }

    return EQ;
}

// Returns a BigInteger representing the difference of this and N
BigInteger BigInteger::sub(const BigInteger& N ) const{
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger EQ; // this - N
    BigInteger Large;

    // Establishing A and B are not opposite signs, setting eq sign
    if( A.sign() != B.sign() && A.sign()!=0 && B.sign()!=0 ){
        // Adding in the correct order will return the correct answer
        B.negate();
        return A.add(B);
    }
    // Setting equals signum
    if( A > B ) EQ.signum = 1;
    else if( A < B ){
        EQ.signum = -1;
    } else if( A == B ){ return EQ; } // Subtracting a value from itself will result in 0

    // All normalized digits sub together
    long lDigA;
    long lDigB;
    long lDig_EQ;
    long Rem = 0; // Remainder
    A.digits.moveBack();
    B.digits.moveBack();
    while( A.digits.length() > 0 || B.digits.length() > 0 ){
        // Adds spacing if needed
        if( A.digits.position() > 0 ){
            lDigA = A.digits.peekPrev();
        } else { lDigA = 0; }
        if( B.digits.position() > 0 ){
            lDigB = B.digits.peekPrev();
        } else { lDigB = 0; }

        // Calculating digit
        lDig_EQ = (lDigA - lDigB) - Rem;
        Rem = 0; // Remainder used
        if( A.sign()==-1 && B.sign()==-1 ){
            lDig_EQ *= -1;
        }
        if( EQ.signum == 1 && lDig_EQ < 0 && ( A.sign()!=-1 || B.sign()!=-1 )){
            lDig_EQ += base; // Normalized
            Rem = 1; // Next remainder
        } else if( EQ.signum == -1 && lDig_EQ > 0 ){
            lDig_EQ -= base; // Normalized
            Rem = -1; // Next remainder
        }
        // Representing every digit as positive, signum will indicate sign
        if( EQ.signum == -1 && lDig_EQ < 0 ){
            lDig_EQ *= -1;
        }
        EQ.digits.insertAfter(lDig_EQ);
        if( A.digits.position() > 0 ){ A.digits.eraseBefore(); }
        if( B.digits.position() > 0 ){ B.digits.eraseBefore(); }
    }
    // Adding final remainder if any
    if( Rem > 0 ){
        EQ.digits.insertAfter(Rem);
    }

    return EQ;
}

// Returns a BigInteger representing the product of this and N
BigInteger BigInteger::mult(const BigInteger& N ) const{
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger EQ; // this * N
    BigInteger lRow; // Last row computed
    lRow.signum = 1; // Needs to be considered positive to add computation to total

    if( A.sign()==0 || B.sign()==0 ){ return EQ; }

    // Each normalized digit of B multiplied by all of A, plus 0's spacing
    long lDigA;
    long lDigB;
    long lDig_lR;
    long Space = 0; // How many spaces of base
    long Rem = 0; // Remainder
    B.digits.moveBack();
    while( B.digits.length() > 0 ){
        // Adds spacing to row calculation
        for( int s = 0; s < Space; s += 1 ){
            lRow.digits.insertAfter(0);
        }
        // Iterates A with current B element for row
        A.digits.moveBack();
        while( A.digits.position() > 0 ){
            // Calculating row
            lDigA = A.digits.peekPrev();
            lDigB = B.digits.peekPrev();
            lDig_lR = (lDigB * lDigA) + Rem;
            Rem = lDig_lR / base; // Next remainder
            lDig_lR = lDig_lR % base; // Normalized
            lRow.digits.insertAfter(lDig_lR);
            A.digits.movePrev();
        }
        EQ += lRow;
        // Adding final remainder if any
        if( Rem > 0 ){
            EQ.digits.moveFront();
            EQ.digits.insertAfter(Rem);
            Rem = 0;
        }
        lRow.digits.clear();
        Space += 1;
        B.digits.eraseBefore();
    }
    // Set EQ sign
    if( A.sign() != B.sign() ){ EQ.signum = -1; }
    if( A.sign() == B.sign() ){ EQ.signum = 1; }

    return EQ;
}

// Other Functions ---------------------------------------------------------

std::string BigInteger::to_string(){
    string sBigInt;
    long Dig;
    if( signum == 0 ){ return "0"; }
    if( signum == -1 ){ sBigInt += "-"; }

    // Looking at every digit of this to append to string
    for( digits.moveFront(); digits.position() < digits.length(); digits.moveNext() ){
        Dig = digits.peekNext();
        if( (Dig/(base/10)) < 1 && digits.position()!=0 ) {
            int DigLen = 0;
            long tDig = Dig; // Temp digit
            while( tDig > 0 ){
                // Counts length of digit to determine zero space
                tDig = tDig / 10;
                DigLen += 1;
            }
            for( int zeros = 0; zeros < (power - DigLen); zeros += 1 ){ sBigInt += "0"; }
            if( DigLen == 0 ){ sBigInt.erase(sBigInt.length()-1,1); }
        }
        sBigInt += std::to_string(Dig);
    }
    // Removing front zero spacing
    while( sBigInt[0] == '0' ){ sBigInt.erase(0, 1); }

    return sBigInt;
}

// Overriden Operators -----------------------------------------------------

// Inserts string representation of N into stream
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream << N.to_string();
}

// Returns true if and only if A equals B
bool operator==( const BigInteger& A, const BigInteger& B ){
    if( A.compare(B) == 0 ){
        return 1;
    } else return 0;
}

// Returns true if and only if A is less than B
bool operator<( const BigInteger& A, const BigInteger& B ){
    if( A.compare(B) == -1 ){
        return 1;
    } else return 0;
}

// Returns true if and only if A is less than or equal to B
bool operator<=( const BigInteger& A, const BigInteger& B ){
    if( A.compare(B) == 0 || A.compare(B) == -1 ){
        return 1;
    } else return 0;
}

// Returns true if and only if A is greater than B
bool operator>( const BigInteger& A, const BigInteger& B ){
    if( A.compare(B) == 1 ){
        return 1;
    } else return 0;
}

// Returns true if and only if A is greater than or equal to B
bool operator>=( const BigInteger& A, const BigInteger& B ){
    if( A.compare(B) == 0 || A.compare(B) == 1 ){
        return 1;
    } else return 0;
}

// Returns the sum A+B
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

// Overwrites A with the sum A+B.
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A.add(B);
    return A;
}

// Returns the difference A-B
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

// Overwrites A with the difference A-B.
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A.sub(B);
    return A;
}

// Returns the product A*B
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}

// Overwrites A with the product A*B
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A.mult(B);
    return A;
}
