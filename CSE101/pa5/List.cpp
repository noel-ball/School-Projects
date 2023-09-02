//-----------------------------------------------------------------------------
// Noel Ball njball pa5
// List.cpp
// Implementation file for List ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include<stdexcept>
#include"List.h"
using namespace std;

// Private Constructor --------------------------------------------------------

// Node constructor
List::Node::Node(ListElement x){
    data = x;
    next = nullptr;
    prev = nullptr;
}


// Class Constructors & Destructors -------------------------------------------

// Creates a new List in the empty state.
List::List(){
    // Setting initial field values
    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = num_elements = 0;
}

// Copy Constructor.
List::List(const List& L){
    // Making an empty List
    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = num_elements = 0;

    // Loading elements of L into this List
    Node *Cursor = L.frontDummy->next;
    while( Cursor != L.backDummy ){
        this->insertAfter(Cursor->data);
        this->moveNext();
        Cursor = Cursor->next;
    }
}

// Destructor
List::~List(){
    // Emptying list
    this->clear();
    // Freeing memory
    delete this->frontDummy;
    delete this->backDummy;
}


// Access functions -----------------------------------------------------------

// Returns the length of this List
int List::length() const{
    return this->num_elements;
}

// Returns the front element in this List
ListElement List::front() const{
    if( length()==0 ){
        throw std::length_error("List: front(): empty List");
    }
    return frontDummy->next->data;
}

// Returns the back element in this List
ListElement List::back() const{
    if( length()==0 ){
        throw std::length_error("List: front(): empty List");
    }
    return backDummy->prev->data;
}

// Returns the position of the cursor in this List
int List::position() const{
    return pos_cursor;
}

// Returns the element after the cursor
ListElement List::peekNext() const{
    if( position() >= length() ){
        throw std::range_error("List: peekNext(): No element after cursor");
    }
    return afterCursor->data;
}

// Returns the element before the cursor
ListElement List::peekPrev() const{
    if( position() <= 0 ){
        throw std::range_error("List: peekPrev(): No element before cursor");
    }
    return beforeCursor->data;
}

// Manipulation procedures ----------------------------------------------------

// Deletes all elements in this List, setting it to the empty state.
void List::clear(){
    moveBack();
    while( length() > 0 ){
        eraseBefore();
    }
}

// Moves cursor to position 0 in this List.
void List::moveFront(){
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

// Moves cursor to position length() in this List.
void List::moveBack(){
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    pos_cursor = length();
}

// Advances cursor to next higher position. Returns the List element that was passed
ListElement List::moveNext(){
    if( position() >= length() ){
        throw std::range_error("List: moveNext(): No element after cursor");
    }
    beforeCursor = afterCursor;
    afterCursor = beforeCursor->next;
    pos_cursor += 1;

    return beforeCursor->data;
}

// Advances cursor to next lower position. Returns the List element that was passed
ListElement List::movePrev(){
    if( position() <= 0 ){
        throw std::range_error("List: movePrev(): No element before cursor");
    }
    afterCursor = beforeCursor;
    beforeCursor = afterCursor->prev;
    pos_cursor -= 1;

    return afterCursor->data;
}

// Inserts x after cursor
void List::insertAfter(ListElement x){
    // Creates insert
    Node *insert = new Node(x);
    // Sets node pointers
    beforeCursor->next = insert;
    afterCursor->prev = insert;
    insert->prev = beforeCursor;
    insert->next = afterCursor;
    afterCursor = insert;
    num_elements += 1;
}

// Inserts x before cursor.
void List::insertBefore(ListElement x) {
    // Creates insert
    Node *insert = new Node(x);
    // Sets node pointers
    beforeCursor->next = insert;
    afterCursor->prev = insert;
    insert->prev = beforeCursor;
    insert->next = afterCursor;
    beforeCursor = insert;
    pos_cursor += 1;
    num_elements += 1;
}

// Overwrites the List element after the cursor with x.
void List::setAfter(ListElement x){
    if( position() >= length() ){
        throw std::range_error("List: setAfter(): No element after cursor");
    }
    afterCursor->data = x;
}

// Overwrites the List element before the cursor with x.
void List::setBefore(ListElement x){
    if( position() <= 0 ){
        throw std::range_error("List: setBefore(): No element before cursor");
    }
    beforeCursor->data = x;
}

// Deletes element after cursor.
void List::eraseAfter(){
    if( position() >= length() ){
        throw std::range_error("List: eraseAfter(): No element after cursor");
    }
    // Sets node pointers
    afterCursor->next->prev = beforeCursor;
    beforeCursor->next = afterCursor->next;
    // Replaces afterCursor
    delete afterCursor;
    afterCursor = beforeCursor->next;
    num_elements -= 1;
}

// Deletes element before cursor.
void List::eraseBefore(){
    if( position() <= 0 ){
        throw std::range_error("List: eraseBefore(): No element before cursor");
    }
    // Sets node pointers
    beforeCursor->prev->next = afterCursor;
    afterCursor->prev = beforeCursor->prev;
    // Replaces beforeCursor
    delete beforeCursor;
    beforeCursor = afterCursor->prev;
    // Cursor falls back because element deleted before initial cursor
    pos_cursor -= 1;
    num_elements -= 1;
}

// Other Functions ------------------------------------------------------------

// Performs linear search after cursor for first occurrence of x
// Returns cursor position after x when found, -1 if not
int List::findNext(ListElement x){
    while( position() < length() ){
        if( afterCursor->data != x ){
            // X not found yet
            moveNext();
        } else {
            // X found
            moveNext();
            return position();
        }
    }
    // X not found
    return -1;
}

// Performs linear search before cursor for first occurrence of x
// Returns cursor position before x when found, -1 if not
int List::findPrev(ListElement x){
    while( position() > 0 ){
        if( beforeCursor->data != x ){
            // X not found yet
            movePrev();
        } else {
            // X found
            movePrev();
            return position();
        }
    }
    // X not found
    return -1;
}

// Removes any repeated elements in this List
void List::cleanup(){
    // Saving initial cursor position and creating a new "cleaned" List
    int CursorInit = position();
    List Clean;
    // Checking unique element added to Clean List against elements of this List
    while( length() > Clean.length() ){
        // Setting cursor of this List to iterate after unique elements
        for( moveFront(); position() < Clean.length(); moveNext() );
        // Adding newest unique element
        Clean.moveBack();
        Clean.insertBefore(peekNext());
        // Iterating rest of this List to check for copies
        moveNext();
        while( position() < length() ){
            if( peekNext() != Clean.peekPrev() ){
                // Copy not found yet
                moveNext();
            } else {
                // Copy found and erased
                eraseAfter();
                if( position() < CursorInit ){
                    // Element deleted before cursor moves initial Cursor back
                    CursorInit -= 1;
                }
            }
        }
    }
    // This List matches Clean List, refreshing cursor data
    moveFront();
    while( position() < CursorInit ){
        moveNext();
    }
}

// Returns a new List consisting of the elements of this List + List L
List List::concat(const List& L) const{
    // Creating Concatenated List and Cursor Node
    List Concat;
    Concat.moveBack();
    Node *Cursor = frontDummy->next;
    // Iterating through this List for first set of elements
    while( Cursor != backDummy ){
        Concat.insertBefore(Cursor->data);
        Cursor = Cursor->next;
    }
    // Iterating through List L for next set of elements
    Cursor = L.frontDummy->next;
    while( Cursor != L.backDummy ){
        Concat.insertBefore(Cursor->data);
        Cursor = Cursor->next;
    }
    Concat.moveFront();

    return Concat;
}

// Returns a string of this List consisting of a comma separated elements in ()
std::string List::to_string() const{
    if( length()==0 ){
        return "";
    }
    // Creating string of List and Cursor Node
    std::string strL = "(";
    Node *Cursor;
    // Iterating through this List to add elements to string
    for( Cursor = frontDummy->next; Cursor != backDummy; Cursor = Cursor->next ){
        if( Cursor != backDummy->prev ){
            strL += std::to_string(Cursor->data) + ", ";
        } else {
            // Last element
            strL += std::to_string(Cursor->data) + ")";
        }
    }

    return strL;
}

bool List::equals(const List& R) const{
    // Creating Cursor Node
    Node *Cursor = frontDummy->next;
    Node *rCursor = R.frontDummy->next;
    // Iterating through this List to compare with List R
    while( Cursor != backDummy && rCursor != R.backDummy ){
        if( Cursor->data != rCursor->data ){
            return false;
        }
        Cursor = Cursor->next;
        rCursor = rCursor->next;
    }

    if( Cursor == backDummy && rCursor == R.backDummy ){
        return true;
    } else { return false; }
}

// Overriding Operators --------------------------------------------------------

// Inserts string representation of Q into stream
std::ostream& operator<<( std::ostream& stream, const List& L ){
    return stream << L.List::to_string();
}

// Returns true if and only if A is the same integer sequence as B
bool operator==( const List& A, const List& B ){
    return A.List::equals(B);
}

// Overwrites the state of this List with state of L
List& List::operator=( const List& L ) {
    if( this != &L ){ // Not self assignment
        // Make a copy of L
        List Swap = L;

        // Then swap the copy's fields with fields of this
        std::swap(frontDummy, Swap.frontDummy);
        std::swap(backDummy, Swap.backDummy);
        std::swap(beforeCursor, Swap.beforeCursor);
        std::swap(afterCursor, Swap.afterCursor);
        std::swap(pos_cursor, Swap.pos_cursor);
        std::swap(num_elements, Swap.num_elements);
    }

    // Return this with the new data installed
    return *this;
}