#include "mathlib.h"

static int terms_cnt = 0; //count for total terms

double e(void) {
    terms_cnt = 0;
    double k = 0.0; //iteration step
    double last_term = 0.0; //term from the previous step
    double e_est = 0.0; //estimation for e
    double term;

    do {
        e_est += last_term;
        double d = 1.0; //denominator
        for (double f = 1.0; f <= k; f++) { //calculates factorial of denominator
            d *= f;
        }
        term = 1.0 / d; //creates computed term
        last_term = term;
        k++;
        terms_cnt++;
    } while (term > EPSILON);

    return e_est;
}

int e_terms(void) {
    return terms_cnt;
}
