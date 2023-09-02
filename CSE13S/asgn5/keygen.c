#include "ss.h"
#include "randstate.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <gmp.h>

#define OPTIONS "hvb:i:n:d:s"

int main(int argc, char **argv) {
    //Sets synopsis, stats, default variables, and pointers
    char Help[] = ("SYNOPSIS\n"
                   "    Generates an SS public/private key pair.\n"
                   "\n"
                   "USAGE\n"
                   "    ./keygen [OPTIONS]"
                   "\n"
                   "OPTIONS\n"
                   "    -h              Display program help and usage.\n"
                   "    -v              Display verbose program output.\n"
                   "    -b bits         Minimum bits needed for public key n (default: 256).\n"
                   "    -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n"
                   "    -n pbfile       Public key file (default: ss.pub).\n"
                   "    -d pvfile       Private key file (default: ss.priv).\n"
                   "    -s seed         Random seed for testing.\n");

    int verbose = 0, bits = 256, iters = 50, seed = time(NULL), opt = 0;
    char pbf[64] = { 0 }, pvf[64] = { 0 }, *un;

    //command line parsing inputs
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        //activates verbose output
        case 'v': verbose = 1; break;
        case 'b':
            //minimum bits
            bits = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'i':
            //iterations
            iters = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'n':
            //public file
            (void) strcpy(pbf, optarg);
            break;
        case 'd':
            //private file
            (void) strcpy(pvf, optarg);
            break;
        case 's':
            //seed
            seed = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'h':
            //prints Help if -h or nothing is input
            printf("%s\n", Help);
            return 1;
        }
    }

    FILE *pbfile, *pvfile;
    //opening public file
    if (pbf[0] != 0) {
        pbfile = fopen(pbf, "w");
    } else {
        pbfile = fopen("ss.pub", "w");
    }
    if (pbfile == NULL) {
        fprintf(stderr, "Public File couldn't be opened\n");
    }
    //opening private file
    if (pvf[0] != 0) {
        pvfile = fopen(pvf, "w");
    } else {
        pvfile = fopen("ss.priv", "w");
    }
    if (pvfile == NULL) {
        fprintf(stderr, "Private File couldn't be opened\n");
    }
    //setting private key file permissions
    int pvf_id = fileno(pvfile);
    fchmod(pvf_id, 0600);
    //initializing seed and keys
    randstate_init(seed);
    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);
    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);
    //getting username
    un = getenv("USER");
    //writing to files
    ss_write_pub(n, un, pbfile);
    ss_write_priv(pq, d, pvfile);
    //printing verbose output
    if (verbose) {
        gmp_fprintf(stdout, "user = %s\n", un);
        gmp_fprintf(stdout, "p (%i bits) = %Zd\n", (int) mpz_sizeinbase(p, 2), p);
        gmp_fprintf(stdout, "q (%i bits) = %Zd\n", (int) mpz_sizeinbase(q, 2), q);
        gmp_fprintf(stdout, "n (%i bits) = %Zd\n", (int) mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stdout, "pq (%i bits) = %Zd\n", (int) mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stdout, "d (%i bits) = %Zd\n", (int) mpz_sizeinbase(d, 2), d);
    }
    //clearing
    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    mpz_clears(p, q, n, d, pq, NULL);
}