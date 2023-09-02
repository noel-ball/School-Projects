//-----------------------------------------------------------------------------
// Noel Ball njball pa8
// Dictionary.cpp
// Implementation file for the Dictionary ADT
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>
#include"Dictionary.h"
#define RED 0
#define BLACK 1
using namespace std;

// Private Constructor --------------------------------------------------------

// Node constructor
Dictionary::Node::Node(keyType k, valType v){
    key = k;
    val = v;
    parent = nullptr;
    left   = nullptr;
    right  = nullptr;
    color  = RED;
}

// Class Constructors & Destructors ----------------------------------------

// Creates new Dictionary in the empty state.
Dictionary::Dictionary(){
    // Nil set to be empty state, start root and current as empty state
    nil = new Node("", 0);
    nil->parent = nil->left = nil->right = nil;
    nil->color = BLACK;
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
    if( R->color == RED ) s += R->key + " (RED)\n";
    else s += R->key + "\n";
    preOrderString(s, R->left );
    preOrderString(s, R->right);
}

// Inserts a copy of the Node *M into this Dictionary. Used by preOrderCopy().
void Dictionary::BST_insert(Node* M){
    Node *Search = root;
    Node *pN = nil;
    while (Search != nil) {
        pN = Search;
        if (M->key < Search->key) Search = Search->left;
        else Search = Search->right;
    }
    M->parent = pN;

    // If parent is nil, new root
    if( M->parent == nil ) root = M;

    // Inserted node is left child
    if( M->parent->key > M->key ) M->parent->left = M;

    // Inserted node is right child
    if( M->parent->key < M->key ) M->parent->right = M;

    M->left = M->right = nil;
    RB_InsertFixUp(M);
}

// Recursively inserts a deep copy of the subtree rooted at R, terminating at N.
void Dictionary::preOrderCopy(Node* R, Node* N){
    // Recursively searches current, left, right
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
    if( R==nil || R->key==k ) return R;
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

// RBT Helper Functions (Optional) -----------------------------------------

void Dictionary::LeftRotate(Dictionary::Node *N){
    // Creating space for node to rotate
    Node* lR = N->right;
    N->right = lR->left;
    if( lR->left!=nil ) lR->left->parent = N; // Child of N
    lR->parent = N->parent;
    if( N->parent==nil ) root = lR; // Rotate takes root
    else if( N->parent->left  == N ) N->parent->left = lR; // Rotate becomes left child
    else N->parent->right = lR; // Rotate becomes right child
    lR->left = N; // Node rotated left
    N->parent = lR;
}

void Dictionary::RightRotate(Dictionary::Node *N){
    // Creating space for node to rotate
    Node* rR = N->left;
    N->left = rR->right;
    if( rR->right!=nil ) rR->right->parent = N; // Child of N
    rR->parent = N->parent;
    if( N->parent==nil ) root = rR; // Rotate takes root
    else if( N->parent->right  == N ) N->parent->right = rR; // Rotate becomes left child
    else N->parent->left = rR; // Rotate becomes right child
    rR->right = N; // Node rotated left
    N->parent = rR;
}

void Dictionary::RB_InsertFixUp(Dictionary::Node *N){
    while( N->parent->color == RED ){
        if( N->parent == N->parent->parent->left ) {
            Node *S = N->parent->parent->right;
            if( S->color == RED ){
                // Case 1
                N->parent->color = BLACK;
                S->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if( N == N->parent->right ){
                    // Case 2
                    N = N->parent;
                    LeftRotate(N);
                }
                // Case 3
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                RightRotate(N->parent->parent);
            }
        } else {
            // Case 4
            Node* S = N->parent->parent->left;
            if( S->color == RED ){
                N->parent->color = BLACK;
                S->color = BLACK;
                N->parent->parent->color = RED;
                N = N->parent->parent;
            } else {
                if( N == N->parent->left ){
                    // Case 5
                    N = N->parent;
                    RightRotate(N);
                }
                // Case 6
                N->parent->color = BLACK;
                N->parent->parent->color = RED;
                LeftRotate(N->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void Dictionary::RB_Transplant(Dictionary::Node *u, Dictionary::Node *v){
    if( u->parent==nil ) root = v; // V takes root
    else if( u->parent->left == u ) u->parent->left = v; // V is left child
    else u->parent->right = v; // V is right child
    v->parent = u->parent;
}

void Dictionary::RB_DeleteFixUp(Dictionary::Node *N){
    while( N!=root && N->color == BLACK ){
        if( N->parent->left == N ){
            Node* S = N->parent->right;

            if( S->color == RED ){
                N->parent->color = RED;
                S->color = BLACK;
                LeftRotate(N->parent);
                S = N->parent->right;
            }

            if( S->left->color == BLACK && S->right->color == BLACK ){
                S->color = RED;
                N = N->parent;
            } else {
                if( S->right->color == BLACK ){
                    S->left->color = BLACK;
                    S->color = RED;
                    RightRotate(S);
                    S = N->parent->right;
                }
                S->color = N->parent->color;
                N->parent->color = BLACK;
                S->right->color = BLACK;
                LeftRotate(N->parent);
                N = root; // Terminate the loop
            }
        } else {
            Node* S = N->parent->left;

            if( S->color == RED ){
                N->parent->color = RED;
                S->color = BLACK;
                RightRotate(N->parent);
                S = N->parent->left;
            }

            if( S->right->color == BLACK && S->left->color == BLACK ){
                S->color = RED;
                N = N->parent;
            } else {
                if( S->left->color == BLACK ){
                    S->right->color = BLACK;
                    S->color = RED;
                    LeftRotate(S);
                    S = N->parent->left;
                }
                S->color = N->parent->color;
                N->parent->color = BLACK;
                S->left->color = BLACK;
                RightRotate(N->parent);
                N = root; // Terminate the loop
            }
        }
    }
    N->color = BLACK;
}

void Dictionary::RB_Delete(Dictionary::Node *N){
    Node* dN = N; // Node to delete
    Node* fN; // Node to fix
    int C = dN->color; // Color to delete

    // Only right child
    if( N->left==nil ){
        fN = N->right;
        RB_Transplant(N, N->right);

    // Only left child
    } else if( N->right==nil ){
        fN = N->left;
        RB_Transplant(N, N->left);

    // Both or neither children
    } else {
        dN = findMin(N->right); // Successor child Node
        fN = dN->right;
        C = dN->color;
        if( dN->parent==N ) fN->parent = dN;
        else {
            RB_Transplant(dN, dN->right);
            dN->right = N->right;
            dN->right->parent = dN;
        }

        RB_Transplant(N, dN);
        dN->left = N->left;
        dN->left->parent = dN;
        dN->color = N->color;
    }

    delete N;
    if( C==BLACK ) RB_DeleteFixUp(fN);
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
    current = nil;
    num_pairs = 0;
    nil->color = BLACK;
}

// Overwrites key==k with the corresponding value v, otherwise inserts as new pair.
void Dictionary::setValue(keyType k, valType v){
    Node* key = search(root, k);
    if( key!=nil ) key->val = v;
    else {
        // Inserting new pair
        Node *Insert = new Node(k, v);
        BST_insert(Insert);
        num_pairs += 1;
    }
}

// Deletes the pair for which key==k. If that pair is current, it becomes undefined.
void Dictionary::remove(keyType k){
    Node* key = search(root, k);
    if( key==nil ) throw std::invalid_argument("Dictionary: remove(): does not contain(k)");
    if( key==current ) current = nil;
    RB_Delete(key);
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
    clear();
    // Copying current
    current = D.current;
    // Copying nodes
    preOrderCopy(D.root, D.nil);
    return *this;
}
