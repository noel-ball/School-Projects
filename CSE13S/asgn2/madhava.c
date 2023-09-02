#include "mathlib.h"

static int terms_cnt = 0; //count for total terms

double pi_madhava(void) {
    terms_cnt = 0;
    double k = 0.0; //iteration step
    double last_term = 0.0; //term from the previous step
    double pi_est = 0.0; //estimation for pi
    double term;

    do {
        pi_est += last_term;
        double exp = 1.0; //total exponential term
        for (double e = 1.0; e <= k; e++) { //calculates -3^k
            exp *= -3.0;
        }
        term = (1.0) / (((2.0 * k) + 1.0) * (exp)); //creates computed term
        last_term = term;
        term = absolute(term); //ensures term can be checked against epsilon
        k++;
        terms_cnt++;
    } while (term > EPSILON);

    pi_est *= sqrt_newton(12.0);
    return pi_est;
}

int pi_madhava_terms(void) {
    return terms_cnt;
}
