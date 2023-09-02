#include "stats.h"

void comparator(Stats *stats, uint32_t *A, int x, int y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        //swaps the two elements if the first is greater
        swap(stats, &A[x], &A[y]);
    }
    return;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n == 0)
        return;
    uint32_t bit_length = 0;
    uint32_t length = n;
    while (length) {
        //counts how many right bit-shifts it takes to reach 0 for bit_length
        length >>= 1;
        bit_length++;
    }
    uint32_t p = 1 << (bit_length - 1);

    while (p > 0) {
        uint32_t q = 1 << (bit_length - 1);
        uint32_t r = 0;
        uint32_t d = p;

        while (d > 0) {
            for (uint32_t i = 0; i < (n - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
