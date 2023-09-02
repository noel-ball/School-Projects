#include "mathlib.h"

static int iter_cnt = 0; //count for total iterations

double sqrt_newton(double x) {
    iter_cnt = 0;
    double last_term = 0.0;
    double term = 1.0;
    while (absolute(term - last_term) > EPSILON) {
        last_term = term;
        term = 0.5 * (last_term + x / last_term);
        iter_cnt++;
    }
    return term;
}

int sqrt_newton_iters(void) {
    return iter_cnt;
}
