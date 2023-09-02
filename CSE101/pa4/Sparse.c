//-----------------------------------------------------------------------------
// Noel Ball njball pa4
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    FILE *infile, *outfile;

    if (argc != 3) {
        printf("Sparce Error: needs exactly two arguments <input> <output>\n");
        return 1;
    }

    char *inname = argv[1];
    char *outname = argv[2];

    infile = fopen(inname, "r");
    outfile = fopen(outname, "w");

    if (infile == NULL || outfile == NULL) {
        printf("Sparce Error: files could not be opened\n");
        return 1;
    }

    int s, a, b, row, col;
    double val;
    fscanf(infile, "%d %d %d", &s, &a, &b);
    Matrix A = newMatrix(s);
    Matrix B = newMatrix(s);


    for( int i = 0; i < a; i+=1 ){
        fscanf(infile, "%d %d %lf", &row, &col, &val);
        changeEntry(A, row, col, val);
    }

    for( int i = 0; i < b; i+=1 ){
        fscanf(infile, "%d %d %lf", &row, &col, &val);
        changeEntry(B, row, col, val);
    }

    fprintf(outfile, "A has %d non-zero entries:\n", NNZ(A));
    printMatrix(outfile, A);

    fprintf(outfile, "\nB has %d non-zero entries:\n", NNZ(B));
    printMatrix(outfile, B);

    Matrix Ax = scalarMult(1.5, A);
    fprintf(outfile, "\n(1.5)*A =\n");
    printMatrix(outfile, Ax);

    Matrix S = sum(A, B);
    fprintf(outfile, "\nA+B =\n");
    printMatrix(outfile, S);

    S = sum(A, A);
    fprintf(outfile, "\nA+A =\n");
    printMatrix(outfile, S);

    Matrix D = diff(B, A);
    fprintf(outfile, "\nB-A =\n");
    printMatrix(outfile, D);

    D = diff(A, A);
    fprintf(outfile, "\nA-A =\n");
    printMatrix(outfile, D);

    Matrix T = transpose(A);
    fprintf(outfile, "\nTranspose(A) =\n");
    printMatrix(outfile, T);

    Matrix P = product(A, B);
    fprintf(outfile, "\nA*B =\n");
    printMatrix(outfile, P);

    P = product(B, B);
    fprintf(outfile, "\nB*B =\n");
    printMatrix(outfile, P);

    fprintf(outfile, "\n");

    freeMatrix(&A);
    freeMatrix(&B);
    freeMatrix(&Ax);
    freeMatrix(&S);
    freeMatrix(&D);
    freeMatrix(&T);
    freeMatrix(&P);

    fclose(infile);
    fclose(outfile);
    return 0;
}
