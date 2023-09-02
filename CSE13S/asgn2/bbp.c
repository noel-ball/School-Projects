#include "mathlib.h"

static int terms_cnt = 0; //count for total terms

double pi_bbp(void) {
    terms_cnt = 0;
    double k = 0.0; //iteration step
    double last_term = 0.0; //term from the previous step
    double pi_est = 0.0; //estimation for pi
    double term;

    do {
        pi_est += last_term;
        double exp = 1.0; //total exponential term
        for (double e = 1.0; e <= k; e++) { //calculates 16^k
            exp *= 16.0;
        }
        term = (1.0 / exp)
               * ((4.0 / ((8.0 * k) + 1)) - (2.0 / ((8.0 * k) + 4)) - (1.0 / ((8.0 * k) + 5))
                   - (1.0 / ((8.0 * k) + 6))); //creates computed term
        last_term = term;
        k++;
        terms_cnt++;
    } while (term > EPSILON);

    return pi_est;
}

int pi_bbp_terms(void) {
    return terms_cnt;
}
