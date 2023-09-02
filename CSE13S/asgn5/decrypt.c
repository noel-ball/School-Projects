#include "ss.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gmp.h>

#define OPTIONS "hvi:o:n:"

int main(int argc, char **argv) {
    //Sets synopsis, stats, default variables, and pointers
    char Help[] = "SYNOPSIS\n"
                  "   Decrypts data using SS decryption.\n"
                  "   Decrypted data is encrypted by the encrypt program.\n"
                  "\n"
                  "USAGE\n"
                  "   ./decrypt [OPTIONS]\n"
                  "\n"
                  "OPTIONS\n"
                  "   -h              Display program help and usage.\n"
                  "   -v              Display verbose program output.\n"
                  "   -i infile       Input file of data to decrypt (default: stdin).\n"
                  "   -o outfile      Output file for decrypted data (default: stdout).\n"
                  "   -n pvfile       Private key file (default: ss.priv).\n";

    int verbose = 0, opt = 0;
    char pvf[64] = { 0 }, inf[64] = { 0 }, otf[64] = { 0 };

    //command line parsing inputs
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        case 'v': verbose = 1; break;
        case 'i': (void) strcpy(inf, optarg); break;
        case 'o': (void) strcpy(otf, optarg); break;
        case 'n': (void) strcpy(pvf, optarg); break;
        case 'h':
            //prints Help if -h or nothing is input
            printf("%s\n", Help);
            return 1;
        }
    }

    FILE *pvfile, *infile, *outfile;
    infile = stdin;
    outfile = stdout;
    //opening files
    if (pvf[0] != 0) {
        pvfile = fopen(pvf, "r");
    } else {
        pvfile = fopen("ss.priv", "r");
    }
    if (pvfile == NULL) {
        fprintf(stderr, "Private File couldn't be opened\n");
    }
    //opening infile
    if (inf[0] != 0) {
        infile = fopen(inf, "r");
    }
    if (infile == NULL) {
        fprintf(stderr, "In File couldn't be opened\n");
    }
    //opening outfile
    if (otf[0] != 0) {
        outfile = fopen(otf, "w");
    }
    if (outfile == NULL) {
        fprintf(stderr, "Out File couldn't be opened\n");
    }
    //reading private key
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);
    ss_read_priv(pq, d, pvfile);
    //printing verbose output
    if (verbose) {
        gmp_fprintf(stdout, "pq (%i bits) = %Zd\n", (int) mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stdout, "d (%i bits) = %Zd\n", (int) mpz_sizeinbase(d, 2), d);
    }
    //decrypting file
    ss_decrypt_file(infile, outfile, d, pq);
    //clearing
    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(pq, d, NULL);
}
