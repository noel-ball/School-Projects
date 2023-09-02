//-----------------------------------------------------------------------------
// Noel Ball njball pa4
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <math.h>
#include "Matrix.h"
#include "List.h"

// Definitions ---------------------------------------------------

typedef struct EntryObj* Entry;

typedef struct EntryObj {
    int col;
    double val;
} EntryObj;

typedef struct MatrixObj {
    int size;
    int nnz;
    List* rows;
} MatrixObj;

// Constructors-Destructors ---------------------------------------------------

// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n){
    Matrix M = malloc(sizeof(MatrixObj));
    M->size = n;
    M->nnz = 0;
    M->rows = malloc((n + 1) * sizeof(List));
    for( int i = 1; i <= n; i+=1 ){
        M->rows[i] = newList();
    }

    return M;
}

// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix* pM){
    if( pM != NULL && *pM != NULL ){
        for( int i = 1; i <= (*pM)->size; i+=1 ){
            freeList(&((*pM)->rows[i]));
        }
        free((*pM)->rows);
        free(*pM);
        *pM = NULL;
    }
}

Entry newEntry(int col, double val){
    Entry newEntry = (Entry) malloc(sizeof(EntryObj));
    if( newEntry != NULL ){
        newEntry->col = col;
        newEntry->val = val;
    }

    return newEntry;
}

void freeEntry(Entry* entryPtr){
    if( entryPtr != NULL && *entryPtr != NULL ){
        free(*entryPtr);
        *entryPtr = NULL;
    }
}

// Access functions -----------------------------------------------------------

// Return the size of square Matrix M.
int size(Matrix M){
    return(M->size);
}

// Return the number of non-zero elements in M.
int NNZ(Matrix M){
    return(M->nnz);
}

// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B){
    if( A == NULL || B == NULL || (size(A) != size(B)) ){
        return 0;
    }
    if( A == B ){
        return 1;
    }

    for( int i = 1; i <= size(A); i+=1 ){
        moveFront(A->rows[i]);
        moveFront(B->rows[i]);
        while( index(A->rows[i]) != -1 && index(B->rows[i]) != -1 ){
            if( fabs(((Entry) get(A->rows[i]))->val - ((Entry) get(B->rows[i]))->val) > 0.0001 ){
                return 0;
            }
            moveNext(A->rows[i]);
            moveNext(B->rows[i]);
        }
        if( index(A->rows[i]) != index(B->rows[i]) && length(A->rows[i]) != 0 ){
            return 0;
        }
    }

    return 1;
}

// Manipulation procedures ----------------------------------------------------

// Re-sets M to the zero Matrix state.
void makeZero(Matrix M){
    if( M == NULL ){
        fprintf(stderr, "Matrix Error: calling makeZero() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    for( int i = 1; i <= size(M); i+=1 ){
        clear(M->rows[i]);
    }

    M->nnz = 0;
}

// Changes the ith row, jth column of M to the value x.
void changeEntry(Matrix M, int i, int j, double x){
    if( M == NULL ){
        fprintf(stderr, "Matrix Error: calling changeEntry() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if( i < 1 || i > size(M) || j < 1 || j > size(M) ){
        fprintf(stderr, "Matrix Error: calling changeEntry() with out of bounds indices\n");
        exit(EXIT_FAILURE);
    }
    Entry E = newEntry(j, x);

    if( x != 0 ){
        if( length(M->rows[i]) == 0 ){
            append(M->rows[i], E);
            M->nnz+=1;
            return;
        }
        moveFront(M->rows[i]);
        Entry curr = get(M->rows[i]);

        while( index(M->rows[i]) < (length(M->rows[i])-1) && curr->col < j ){
            moveNext(M->rows[i]);
            curr = get(M->rows[i]);
        }
        if( index(M->rows[i]) == (length(M->rows[i])-1) && curr->col < j ){
            append(M->rows[i], E);
            M->nnz+=1;
            return;
        } else if( curr->col > j ){
            insertBefore(M->rows[i], E);
            M->nnz+=1;
            return;
        } else {
            curr->val = x;
        }
    }

    else if( length(M->rows[i]) != 0 ){
        moveFront(M->rows[i]);
        Entry curr = get(M->rows[i]);
        while( index(M->rows[i]) < (length(M->rows[i])-1) && curr->col < j ){
            moveNext(M->rows[i]);
            curr = get(M->rows[i]);
        }
        if( curr->col == j ){
            delete(M->rows[i]);
            M->nnz-=1;
        }
    }

    freeEntry(&E);
}

// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A){
    if( A == NULL ){
        fprintf(stderr, "Matrix Error: calling copy() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    int s = size(A);
    Matrix C = newMatrix(s);
    for( int i = 1; i <= s; i+=1 ){
        moveFront(A->rows[i]);
        while( index(A->rows[i]) != -1 ){
            append( C->rows[i], (Entry) get(A->rows[i]) );
            moveNext(A->rows[i]);
        }
    }

    C->nnz = A->nnz;
    return C;
}

// Returns a reference to a new Matrix object representing A^T
Matrix transpose(Matrix A){
    int s = size(A);
    Matrix T = newMatrix(s);

    for( int j = 1; j <= s; j+=1 ){
        List row = A->rows[j];
        moveFront(row);
        while( index(row) >= 0 ){
            int i = ((Entry) get(row))->col;
            double x = ((Entry) get(row))->val;
            changeEntry(T, i, j, x);
            moveNext(row);
        }
    }

    return T;
}

// Returns a reference to a new Matrix object representing A*x
Matrix scalarMult(double x, Matrix A){
    int s = size(A);
    Matrix Ax = newMatrix(s);

    for( int i = 1; i <= s; i+=1 ){
        List row = A->rows[i];
        moveFront(row);
        while( index(row) >= 0 ){
            int j = ((Entry) get(row))->col;
            double y = x * ((Entry) get(row))->val;
            changeEntry(Ax, i, j, y);
            moveNext(row);
        }
    }

    return Ax;
}

// Returns a reference to a new Matrix object representing A+B.
Matrix sum(Matrix A, Matrix B){
    if( size(A) != size(B) ){
        fprintf(stderr, "Matrix Error: calling sum() on matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    int s = size(A);
    Matrix S = newMatrix(s);

    if( A == B ){
        for( int i = 1; i <= s; i+=1 ){
            List rowA = A->rows[i];
            moveFront(rowA);
            while( index(rowA) >= 0 ){
                int jA = ((Entry) get(rowA))->col;
                double x = ((((Entry) get(rowA))->val) * 2);
                changeEntry(S, i, jA, x);
                moveNext(rowA);
            }
        }

    } else {
        for( int i = 1; i <= s; i+=1 ){
            List rowA = A->rows[i];
            List rowB = B->rows[i];
            moveFront(rowA);
            moveFront(rowB);

            while( index(rowA) >= 0 && index(rowB) >= 0 ){
                int jA = ((Entry) get(rowA))->col;
                int jB = ((Entry) get(rowB))->col;

                if( jA == jB ){
                    double x = ((Entry) get(rowA))->val + ((Entry) get(rowB))->val;
                    changeEntry(S, i, jA, x);
                    moveNext(rowA);
                    moveNext(rowB);
                } else if( jA < jB ){
                    double x = ((Entry) get(rowA))->val;
                    changeEntry(S, i, jA, x);
                    moveNext(rowA);
                } else {
                    double x = ((Entry) get(rowB))->val;
                    changeEntry(S, i, jB, x);
                    moveNext(rowB);
                }
            }

            while( index(rowA) >= 0 ){
                int jA = ((Entry) get(rowA))->col;
                double x = ((Entry) get(rowA))->val;
                changeEntry(S, i, jA, x);
                moveNext(rowA);
            }

            while( index(rowB) >= 0 ){
                int jB = ((Entry) get(rowB))->col;
                double x = ((Entry) get(rowB))->val;
                changeEntry(S, i, jB, x);
                moveNext(rowB);
            }
        }
    }

    return S;
}

// Returns a reference to a new Matrix object representing A-B.
Matrix diff(Matrix A, Matrix B){
    if( size(A) != size(B) ){
        printf("Matrix Error: calling diff() on matrices of different sizes.\n");
        exit(EXIT_FAILURE);
    }
    int s = size(A);
    Matrix D = newMatrix(s);

    if( A == B ){
        return D;

    } else {
        for( int i = 1; i <= s; i+=1 ){
            List rowA = A->rows[i];
            List rowB = B->rows[i];
            moveFront(rowA);
            moveFront(rowB);

            while( index(rowA) >= 0 && index(rowB) >= 0 ){
                int jA = ((Entry) get(rowA))->col;
                int jB = ((Entry) get(rowB))->col;
                if( jA == jB ){
                    double x = ((Entry) get(rowA))->val - ((Entry) get(rowB))->val;
                    changeEntry(D, i, jA, x);
                    moveNext(rowA);
                    moveNext(rowB);
                } else if( jA < jB ){
                    double x = ((Entry) get(rowA))->val;
                    changeEntry(D, i, jA, x);
                    moveNext(rowA);
                } else {
                    double x = ((Entry) get(rowB))->val;
                    changeEntry(D, i, jB, -x);
                    moveNext(rowB);
                }
            }

            while( index(rowA) >= 0 ){
                int jA = ((Entry) get(rowA))->col;
                double x = ((Entry) get(rowA))->val;
                changeEntry(D, i, jA, x);
                moveNext(rowA);
            }

            while( index(rowB) >= 0 ){
                int jB = ((Entry) get(rowB))->col;
                double x = ((Entry) get(rowB))->val;
                changeEntry(D, i, jB, -x);
                moveNext(rowB);
            }
        }
    }

    return D;
}

// Returns a reference to a new Matrix object representing A⋅B
double vectorDot(List P, List Q){
    double dot = 0.0;
    moveFront(P);
    moveFront(Q);
    while( (index(P) != -1) && (index(Q) != -1) ){
        Entry eP = (Entry) get(P);
        Entry eQ = (Entry) get(Q);
        if (eP->col < eQ->col) {
            moveNext(P);
        } else if (eP->col > eQ->col) {
            moveNext(Q);
        } else {
            dot += (eP->val) * (eQ->val);
            moveNext(P);
            moveNext(Q);
        }
    }

    return dot;
}

// Returns a reference to a new Matrix object representing A*B
Matrix product(Matrix A, Matrix B){
    int s = size(A);
    if( s != size(B) ){
        printf("Matrix Error: calling product() on matrices of different sizes.\n");
        exit(EXIT_FAILURE);
    }
    Matrix P = newMatrix(s);
    Matrix T = transpose(B);
    for( int i = 1; i <= s; i+=1 ){
        if( length(A->rows[i]) == 0 ){
            continue;
        }
        for( int j = 1; j <= s; j+=1 ){
            if( length(T->rows[j]) == 0 ){
                continue;
            }
            double dP = vectorDot(A->rows[i], T->rows[j]);
            if (dP != 0) {
                changeEntry(P, i, j, dP);
            }
        }
    }

    freeMatrix(&T);
    return P;
}

// Other functions ------------------------------------------------------------

// Prints a string representation of Matrix M to filestream out
void printMatrix(FILE* out, Matrix M) {
    if( M == NULL ){
        fprintf(stderr, "Matrix Error: calling printMatrix() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    for( int i = 1; i <= size(M); i+=1 ){
        if( length(M->rows[i]) > 0 ){
            fprintf(out, "%d: ", i);
            moveFront(M->rows[i]);
            while( index(M->rows[i]) != -1 ){
                Entry curr = get(M->rows[i]);
                fprintf(out, "(%d, %.1f) ", curr->col, curr->val);
                moveNext(M->rows[i]);
            }
            fprintf(out, "\n");
        }
    }
}
