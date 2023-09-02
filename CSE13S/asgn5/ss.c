#include "ss.h"
#include "numtheory.h"
#include "randstate.h"
#include <stdlib.h>
#include <unistd.h>

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    //random number of bit count for p in range [nbits/5, (2nbits)/5)
    mpz_t gp, gq, psub1, qsub1;
    mpz_init_set_ui(gp, 0);
    mpz_init_set_ui(gq, 0);
    mpz_inits(psub1, qsub1, NULL);

    //loops until p is not divisible by q-1 and q is not divisible by p-1
    while ((mpz_cmp_ui(gp, 1) != 0) || (mpz_cmp_ui(gq, 1) != 0)) {
        uint64_t p_bits = rand();
        //ensuring p_bits does not exceed bounds [nbits/5, 2nbits/5)
        p_bits = (p_bits % (((2 * nbits) / 5) - (nbits / 5)) + 1);
        p_bits += (nbits / 5);
        //makes primes p and q, p with a random number of bits in nbits range, and q with nbits - p's bits
        make_prime(p, p_bits, iters);
        make_prime(q, (nbits - p_bits), iters);
        //makes n as p^2 * q
        mpz_mul(n, p, p);
        mpz_mul(n, n, q);
        //finds gcd between p and q-1 as well as q and p-1 to determine if they are divisible
        mpz_sub_ui(psub1, p, 1);
        mpz_sub_ui(qsub1, q, 1);
        gcd(gp, p, qsub1);
        gcd(gq, q, psub1);
    }
    mpz_clears(gp, gq, psub1, qsub1, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {
    mpz_t tp, tq, ld, n, g;
    mpz_inits(tp, tq, ld, n, g, NULL);
    mpz_sub_ui(tp, p, 1);
    mpz_sub_ui(tq, q, 1);
    //lcm(p-1, q-1) = (p-1 * q-1) / gcd(p-1, q-1)
    mpz_mul(pq, tp, tq); //using pq = p-1 * q-1 for lcm
    gcd(g, tp, tq);
    mpz_fdiv_q(ld, pq, g); //ld = lcm(p-1, q-1)
    //d = inverse of n mod ld(pq)
    mpz_mul(pq, p, q); //setting pq normally
    mpz_mul(n, p, pq); //n = p^2 * q
    mod_inverse(d, n, ld);
    mpz_clears(tp, tq, ld, n, g, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    //format n as hex, username as string, printed with trailing newline
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    //format pq and d as hex, printed with trailing newline
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    //scans n as hex, username as string, with trailing newline
    (void) gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    //scans pq and d as hex, with trailing newline
    (void) gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    //stores m^n mod (n) in c
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    uint64_t j, k;
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    //allocates block size k, as (log_2(n)/2 - 1) / 8, and prepends first byte
    k = (((mpz_sizeinbase(n, 2) / 2) - 1) / 8);
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));
    block[0] = 0xFF;

    //iteratively reading infile from block after buffer, using fread with file descriptors
    while ((j = fread((block + 1), 1, (k - 1), infile)) != 0) {
        //importing block to mpz_t to encrypt
        mpz_import(m, (j + 1), 1, 1, 1, 0, block);
        ss_encrypt(c, m, n);
        //prints to file
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    fprintf(outfile, "\n");
    //clears
    free(block);
    mpz_clears(m, c, NULL);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    //stores c^d mod (pq) in m
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    uint64_t j, k, i;
    mpz_t c, m;
    mpz_inits(c, m, NULL);
    //allocates block size k, as (log_2(pq)-1 / 8)
    k = ((mpz_sizeinbase(pq, 2) - 1) / 8);
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    while (gmp_fscanf(infile, "%Zx\n", c) != EOF) {
        //line by line decrypts to array
        ss_decrypt(m, c, d, pq);
        mpz_export(block, &j, 1, 1, 1, 0, m);
        //printing block to outfile
        for (i = 1; i < j; i += 1) {
            fprintf(outfile, "%c", block[i]);
        }
    }
    fprintf(outfile, "\n");
    free(block);
    mpz_clears(c, m, NULL);
}
