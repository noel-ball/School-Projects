//-----------------------------------------------------------------------------
// Noel Ball njball pa7
// Dictionary.cpp
// Implementation file for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include"Dictionary.h"
using namespace std;

// Private Constructor --------------------------------------------------------

// Node constructor
Dictionary::Node::Node(keyType k, valType v){
    key = k;
    val = v;
    parent = nullptr;
    left   = nullptr;
    right  = nullptr;
}

// Class Constructors & Destructors ----------------------------------------

// Creates new Dictionary in the empty state.
Dictionary::Dictionary(){
    // Nil set to be empty state, start root and current as empty state
    nil = new Node("", 0);
    nil->parent = nil->left = nil->right = nil;
    root = current = nil;
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
    // Copying fields
    nil       = D.nil;
    root      = D.root;
    current   = D.current;
    num_pairs = D.num_pairs;
    // Copying nodes
    preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary(){
    // Deleting nodes
    postOrderDelete(root);
    // Deleting nil memory
    delete nil;
}

// Helper Functions (Optional) ---------------------------------------------

// Appends a string representation of the tree rooted at R to string s.
void Dictionary::inOrderString(std::string& s, Node* R) const{
    // Recursively looks left, current, right
    if( R->key=="" ) return;
    inOrderString(s, R->left );
    s += R->key + " : " + std::to_string(R->val) + "\n";
    inOrderString(s, R->right);
}

// Appends a string representation of pre-order tree walk keys only, separated by "\n".
void Dictionary::preOrderString(std::string& s, Node* R) const{
    // Recursively searches current, left, right
    if( R->key=="" ) return;
    s += R->key + "\n";
    preOrderString(s, R->left );
    preOrderString(s, R->right);
}


// Recursively inserts a deep copy of the subtree rooted at R, terminating at N.
void Dictionary::preOrderCopy(Node* R, Node* N){
    // Recursively searches current, left, right
    if( R->key=="" )     return;
    if( R->key==N->key ) return;
    setValue(R->key, R->val);
    preOrderCopy(R->left,  N);
    preOrderCopy(R->right, N);
}

// Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R){
    // Recursively searches right, left, current
    if( R->key=="" ) return;
    postOrderDelete(R->right);
    postOrderDelete(R->left );
    delete R;
}

// Searches the subtree rooted at R for a Node with key==k.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const{
    // Recursively searches for R->key == k
    if( R->key=="" || R->key==k ) return R;
    if( R->key > k ) return search(R->left, k);
    return search(R->right, k);
}

// Returns a pointer to the leftmost Node in that subtree.
Dictionary::Node* Dictionary::findMin(Node* R){
    // Recursively searches for R->left until last
    if( R==nil ) return nil;
    if( R->left==nil ) return R;
    return findMin(R->left);
}

// Returns a pointer to the rightmost Node in that subtree.
Dictionary::Node* Dictionary::findMax(Node* R){
    // Recursively searches for R->right until last
    if( R==nil ) return nil;
    if( R->right==nil ) return R;
    return findMax(R->right);
}

// Returns a pointer to the node after N in an in-order tree walk.
Dictionary::Node* Dictionary::findNext(Node* N){
    if( N==nil ) return nil;
    // Smallest node after it
    if( N->right!=nil ) return findMin(N->right);
    // Next node could be higher
    Node* pN = N->parent;
    while( pN->right==N && pN!=nil ){
        // Node is still higher
        pN = pN->parent;
        N  = N->parent;
    }
    return pN;
}

// Returns a pointer to the node before N in an in-order tree walk.
Dictionary::Node* Dictionary::findPrev(Node* N){
    if( N==nil ) return nil;
    // Largest node before it
    if( N->left!=nil ) return findMax(N->left);
    // Next node could be higher
    Node *pN = N->parent;
    while( pN->left==N && pN!=nil ){
        // Node is still higher
        pN = pN->parent;
        N  = N->parent;
    }
    return pN;
}

// Access functions --------------------------------------------------------

// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// Returns true if there exists a pair such that key==k, and returns false otherwise.
bool Dictionary::contains(keyType k) const{
    Node* key = search(root, k);
    if( key!=nil ) return 1;
    return 0;
}

// Returns a reference to the value corresponding to key k.
valType& Dictionary::getValue(keyType k) const{
    Node* key = search(root, k);
    if( key==nil ) throw std::invalid_argument("Dictionary: getValue(): does not contain(k)");
    return key->val;
}

// Returns true if the current iterator is defined, and returns false otherwise.
bool Dictionary::hasCurrent() const{
    if( current!=nil ) return 1;
    return 0;
}

// Returns the current key.
keyType Dictionary::currentKey() const{
    if( current!=nil ) return current->key;
    throw std::invalid_argument("Dictionary: currentKey(): current undefined");
}

// Returns a reference to the current value.
valType& Dictionary::currentVal() const{
    if( current!=nil ) return current->val;
    throw std::invalid_argument("Dictionary: currentVal(): current undefined");
}

// Manipulation procedures -------------------------------------------------

// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    if( root!=nil ) postOrderDelete(root);
    root = current = nil;
    num_pairs = 0;
}

// Overwrites key==k with the corresponding value v, otherwise inserts as new pair.
void Dictionary::setValue(keyType k, valType v){
    Node* key = search(root, k);
    if( key!=nil ) key->val = v;
    else {
        // Inserting new pair
        Node *Insert = new Node(k, v);
        Insert->left = Insert->right = nil;
        Node *Search = root;
        Node *pN = nil;
        while (Search != nil) {
            pN = Search;
            if (Insert->key < Search->key) Search = Search->left;
            else Search = Search->right;
        }
        Insert->parent = pN;

        // If parent is nil, new root
        if( Insert->parent == nil ) root = Insert;

        // Inserted node is left child
        if( Insert->parent->key > Insert->key ) Insert->parent->left = Insert;

        // Inserted node is right child
        if( Insert->parent->key < Insert->key ) Insert->parent->right = Insert;

        num_pairs += 1;
    }
}

// Deletes the pair for which key==k. If that pair is current, it becomes undefined.
void Dictionary::remove(keyType k){
    Node* key = search(root, k);
    if( key==nil ) throw std::invalid_argument("Dictionary: remove(): does not contain(k)");
    Node* Replace;

    // Deleted node has both children
    if( key->left!=nil && key->right!=nil ){
        Replace = findMin(key->right);
        if( Replace->right!=nil ){
            // Replace will be moved to removed keys position
            Replace->right->parent = Replace->parent;
        }
        if( Replace->parent->left==Replace ){
            // Replace is left child, right moves up
            Replace->parent->left = Replace->right;
        } else {
            // Replace is right child, left moves up
            Replace->parent->right = Replace->right;
        }
        // Replace takes removed keys fields
        Replace->parent = key->parent;
        Replace->left   = key->left;
        Replace->right  = key->right;

    // Deleted node only has one child, it moves up
    } else if( key->left!=nil ){
        if( key==root ) root = key->left;
        else {
            key->left->parent = key->parent;
            if( key->parent->left==key ) key->parent->left = key->left;
            else key->parent->right = key->left;
        }

    } else if( key->right!=nil ){
        if( key==root ) root = key->right;
        else {
            key->right->parent = key->parent;
            if( key->parent->left==key ) key->parent->left = key->right;
            else key->parent->right = key->right;
        }

    } else {
        // Deleted node has no children
        if( key==root ) root = nil;
        else{
            if( key->parent->left==key ) key->parent->left = nil;
            else key->parent->right = nil;
        }
    }

    if( current==key ) current = nil;
    delete key;
    num_pairs -= 1;
}

// If non-empty, places current iterator at the first (key, value) pair, otherwise does nothing.
void Dictionary::begin(){
    Node* key = findMin(root);
    if( key!=nil ) current = key;
}

// If non-empty, places current iterator at the last (key, value) pair, otherwise does nothing.
void Dictionary::end(){
    Node* key = findMax(root);
    if( key!=nil ) current = key;
}

// Advances current to the next pair. If at the last pair, makes current undefined.
void Dictionary::next(){
    if( current==nil ) throw std::invalid_argument("Dictionary: next(): current undefined");
    current = findNext(current);
}

// Moves current to the previous pair. If at the first pair, makes current undefined.
void Dictionary::prev(){
    if( current==nil ) throw std::invalid_argument("Dictionary: prev(): current undefined");
    current = findPrev(current);
}

// Other Functions ---------------------------------------------------------

// Returns a string representation of consecutive key : value pairs in order by <.
std::string Dictionary::to_string() const{
    std::string sDict = "";
    inOrderString(sDict, root);
    return sDict;
}

// Returns a string consisting of all keys separated by newline "\n" characters in pre-order.
std::string Dictionary::pre_string() const{
    std::string sDict = "";
    preOrderString(sDict, root);
    return sDict;
}

// Returns true if and only if this Dictionary contains the same pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
    if( size() != D.size() ) return 0;
    return to_string() == D.to_string();
}

// Overloaded Operators ----------------------------------------------------

// Inserts string representation of Dictionary D into stream.
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    return stream << D.to_string();
}

// Returns true if and only if Dictionary A equals Dictionary B.
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// Overwrites this Dictionary state with state of D, and returns a reference.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    // Copying fields
    nil       = D.nil;
    root      = D.root;
    current   = D.current;
    num_pairs = D.num_pairs;
    // Copying nodes
    preOrderCopy(D.root, D.nil);
    return *this;
}
