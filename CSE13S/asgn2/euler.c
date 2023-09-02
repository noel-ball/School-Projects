#include "mathlib.h"

static int terms_cnt = 0; //count for total terms

double pi_euler(void) {
    terms_cnt = 0;
    double k = 1.0; //iteration step
    double last_term = 0.0; //term from the previous step
    double pi_est = 0.0; //estimation for pi
    double term;

    do {
        pi_est += last_term;
        double d = k * k; //denominator
        term = 1.0 / d; //creates computed term
        last_term = term;
        k++;
        terms_cnt++;
    } while (term > EPSILON);

    pi_est *= 6.0;
    pi_est = sqrt_newton(pi_est);
    return pi_est;
}

int pi_euler_terms(void) {
    return terms_cnt;
}
