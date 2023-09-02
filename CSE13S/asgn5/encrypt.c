#include "ss.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <gmp.h>

#define OPTIONS "hvi:o:n:"

int main(int argc, char **argv) {
    //Sets synopsis, stats, default variables, and pointers
    char Help[] = "SYNOPSIS\n"
                  "   Encrypts data using SS encryption.\n"
                  "   Encrypted data is decrypted by the decrypt program.\n"
                  "\n"
                  "USAGE\n"
                  "   ./encrypt [OPTIONS]\n"
                  "\n"
                  "OPTIONS\n"
                  "   -h              Display program help and usage.\n"
                  "   -v              Display verbose program output.\n"
                  "   -i infile       Input file of data to encrypt (default: stdin).\n"
                  "   -o outfile      Output file for encrypted data (default: stdout).\n"
                  "   -n pbfile       Public key file (default: ss.pub).\n";

    int verbose = 0, opt = 0;
    char pbf[64] = { 0 }, inf[64] = { 0 }, otf[64] = { 0 }, un[64];

    //command line parsing inputs
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        case 'v': verbose = 1; break;
        case 'i': (void) strcpy(inf, optarg); break;
        case 'o': (void) strcpy(otf, optarg); break;
        case 'n': (void) strcpy(pbf, optarg); break;
        case 'h':
            //prints Help if -h
            printf("%s\n", Help);
            return 1;
        }
    }
    FILE *pbfile, *infile, *outfile;
    infile = stdin;
    outfile = stdout;
    //opening public file
    if (pbf[0] != 0) {
        pbfile = fopen(pbf, "r");
    } else {
        pbfile = fopen("ss.pub", "r");
    }
    if (pbfile == NULL) {
        fprintf(stderr, "Public File couldn't be opened\n");
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
    //reading public key
    mpz_t n;
    mpz_init(n);
    ss_read_pub(n, un, pbfile);
    //printing verbose output
    if (verbose) {
        gmp_fprintf(stdout, "user = %s\n", un);
        gmp_fprintf(stdout, "n (%i bits) = %Zd\n", (int) mpz_sizeinbase(n, 2), n);
    }
    //encrypting file
    ss_encrypt_file(infile, outfile, n);
    //clearing
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clear(n);
}
