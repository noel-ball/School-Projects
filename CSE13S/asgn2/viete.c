#include "mathlib.h"

static int factors_cnt = 0; //count for total factors

double pi_viete(void) {
    factors_cnt = 0;
    double k = 1.0; //iteration step
    double last_term = 1.0; //term from the previous step
    double last_a_k = 0.0; //a_k from the previous step
    double pi_est = 1.0; //estimation for pi
    double term;

    do {
        pi_est *= last_term;
        double x = 2.0 + last_a_k;
        double a_k = sqrt_newton(x);
        term = a_k / 2.0; //creates computed term
        last_term = term;
        term = absolute((term - 1)); //ensures term can be checked against epsilon
        last_a_k = a_k;
        k++;
        factors_cnt++;
    } while (term > EPSILON);

    pi_est = 2.0 / pi_est;
    return pi_est;
}

int pi_viete_factors(void) {
    return factors_cnt;
}
