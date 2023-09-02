#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t tmp, amod, ta, tb;
    mpz_inits(tmp, amod, NULL);
    mpz_init_set(ta, a);
    mpz_init_set(tb, b);

    //while b is not 0
    while (mpz_cmp_ui(tb, 0) != 0) {
        mpz_set(tmp, tb);
        //b = a mod b
        mpz_mod(amod, ta, tb);
        mpz_set(tb, amod);
        //a = t, d = a
        mpz_set(ta, tmp);
        mpz_set(g, ta);
    }
    mpz_clears(tmp, amod, ta, tb, NULL);
}

void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    //initializes and sets r, r', o, and o'
    mpz_t r, q, rp, op, trp, top;
    mpz_inits(q, trp, top, NULL);
    mpz_init_set(r, n);
    mpz_init_set(rp, a);
    mpz_set_ui(o, 0);
    mpz_init_set_ui(op, 1);

    //while r' is not 0
    while (mpz_cmp_ui(rp, 0) != 0) {
        //q = r/r'
        mpz_fdiv_q(q, r, rp);
        //assigning r, r', o, and o'
        mpz_set(trp, rp);
        mpz_set(top, op);
        //r' = r - q * r', r = r'
        mpz_mul(rp, q, rp);
        mpz_sub(rp, r, rp);
        mpz_set(r, trp);
        //o' = o - q * o', o = o'
        mpz_mul(op, q, op);
        mpz_sub(op, o, op);
        mpz_set(o, top);
    }

    //if r > 1 there is no inverse
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
    }
    if (mpz_cmp_ui(o, 0) < 0) {
        //if o < 0 add n
        mpz_add(o, o, n);
    }
    mpz_clears(r, q, rp, op, trp, top, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t p, td, op, pp;
    mpz_inits(op, pp, NULL);
    //initializes mpz_t o = 1, p = a
    mpz_set_ui(o, 1);
    mpz_init_set(p, a);
    mpz_init_set(td, d);

    //while td greater than 0
    while (mpz_cmp_ui(td, 0) > 0) {
        //checks if d is odd
        if (mpz_odd_p(td)) {
            //o = ((o * p) % n)
            mpz_mul(op, o, p);
            mpz_mod(o, op, n);
        }
        //p = ((p * p) % n)
        mpz_mul(pp, p, p);
        mpz_mod(p, pp, n);
        //d = d/2
        mpz_fdiv_q_ui(td, td, 2);
    }
    mpz_clears(p, td, op, pp, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    //checks input is not 0 or 2
    if (mpz_cmp_ui(n, 0) == 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0)
        return 1;

    mpz_t s, r, a, max, y, j, two, tn, ts;
    mpz_inits(r, a, max, y, tn, ts, NULL);
    mpz_init_set_ui(s, 0);
    mpz_init_set_ui(two, 2);
    mpz_init_set_ui(j, 1);
    mpz_sub_ui(tn, n, 1); //tn = n-1

    //write n-1 = 2^s * r
    mpz_sub_ui(r, n, 1); //r = n-1
    while (mpz_even_p(r)) {
        mpz_fdiv_q(r, r, two); // r / 2 until odd
        mpz_add_ui(s, s, 1); //s counts how many times n-1 is divided by 2
    }
    mpz_sub_ui(ts, s, 1); //ts = s-1

    for (uint64_t i = 1; i <= iters; i += 1) {
        //random a with range (2, n-2)
        mpz_sub_ui(max, n, 4); //max = n-2, -2 to compensate lower bound
        mpz_urandomm(a, state, max);
        //ensuring a does not exceed lower bound by adding 2
        mpz_add_ui(a, a, 2);
        //y = pow-mod(a,r,n)
        pow_mod(y, a, r, n);

        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, tn) != 0)) {
            while ((mpz_cmp(j, ts) <= 0) && (mpz_cmp(y, tn) != 0)) {
                pow_mod(y, y, two, n);
                //return false if y = 1
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, a, max, y, j, two, tn, ts, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            //return false when y != n-1
            if (mpz_cmp(y, tn) != 0) {
                mpz_clears(s, r, a, max, y, j, two, tn, ts, NULL);
                return false;
            }
        }
    }
    mpz_clears(s, r, a, max, y, j, two, tn, ts, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t tb;
    mpz_init(tb);
    mpz_set_ui(p, 0);

    //p in range [2^(bits-1), (2^bits)-1]
    //generates a random p up to (2^(bits-1))-1, adds 2^(bits-1), until it is prime
    while (is_prime(p, iters) == false) {
        mpz_set_ui(p, gmp_urandomb_ui(state, (bits - 1)));
        mpz_ui_pow_ui(tb, 2, (bits - 1)); //tb = 2^(bits-1)
        mpz_add(p, p, tb);
    }
    mpz_clear(tb);
}
