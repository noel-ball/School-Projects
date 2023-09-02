#include "mathlib.h"

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
    int opt = 0;
    int b_e, b_b, b_m, b_r, b_v, b_n, b_s;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        //declares booleans
        case 'a': b_e = 1, b_b = 1, b_m = 1, b_r = 1, b_v = 1, b_n = 1; break;

        case 'e': b_e = 1; break;

        case 'b': b_b = 1; break;

        case 'm': b_m = 1; break;

        case 'r': b_r = 1; break;

        case 'v': b_v = 1; break;

        case 'n': b_n = 1; break;

        case 's': b_s = 1; break;

        case 'h':
            printf("SYNOPSIS\n"
                   "   A test harness for the small numerical library.\n"
                   "\n"
                   "USAGE\n"
                   "   ./mathlib-test [-aebmrvnsh]\n"
                   "\n"
                   "OPTIONS\n"
                   "  -a   Runs all tests.\n"
                   "  -e   Runs e test.\n"
                   "  -b   Runs BBP pi test.\n"
                   "  -m   Runs Madhava pi test.\n"
                   "  -r   Runs Euler pi test.\n"
                   "  -v   Runs Viete pi test.\n"
                   "  -n   Runs Newton square root tests.\n"
                   "  -s   Print verbose statistics.\n"
                   "  -h   Display program synopsis and usage.\n");
            return 1;
        }
    }

    //runs tests on which booleans have been enabled
    if (b_e == 1) {
        double e_est = e();
        int t = e_terms();
        double diff = (M_E - e_est);
        diff = absolute(diff);
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e_est, M_E, diff);
        if (b_s == 1) {
            printf("e() terms = %d\n", t);
        }
    }
    if (b_r == 1) {
        double r = pi_euler();
        int t = pi_euler_terms();
        double diff = (M_PI - r);
        diff = absolute(diff);
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", r, M_PI, diff);
        if (b_s == 1) {
            printf("pi_euler() terms = %d\n", t);
        }
    }
    if (b_b == 1) {
        double b = pi_bbp();
        int t = pi_bbp_terms();
        double diff = (M_PI - b);
        diff = absolute(diff);
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", b, M_PI, diff);
        if (b_s == 1) {
            printf("pi_bbp() terms = %d\n", t);
        }
    }
    if (b_m == 1) {
        double m = pi_madhava();
        int t = pi_madhava_terms();
        double diff = (M_PI - m);
        diff = absolute(diff);
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", m, M_PI, diff);
        if (b_s == 1) {
            printf("pi_madhava() terms = %d\n", t);
        }
    }
    if (b_v == 1) {
        double v = pi_viete();
        int t = pi_viete_factors();
        double diff = (M_PI - v);
        diff = absolute(diff);
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", v, M_PI, diff);
        if (b_s == 1) {
            printf("pi_viete() terms = %d\n", t);
        }
    }
    if (b_n == 1) {
        for (double k = 0.0; k <= 10.0; k += 0.1) {
            double n = sqrt_newton(k);
            double root = sqrt(k);
            int t = sqrt_newton_iters();
            double diff = (root - n);
            diff = absolute(diff);
            printf("sqrt_newton(%16.15lf) = %16.15lf, sqrt(%16.15lf) = %16.15lf, diff = %16.15lf\n",
                k, n, k, root, diff);
            if (b_s == 1) {
                printf("sqrt_newton() terms = %d\n", t);
            }
        }
    }
}
