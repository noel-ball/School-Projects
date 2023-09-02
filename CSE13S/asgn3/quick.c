#include "quick.h"

#include "shell.h"
#include "stats.h"

//partitions the array based on a pivot
static int partition(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t hi = n - 1;
    uint32_t pivot = move(stats, A[hi]);
    int i = 0;
    for (uint32_t j = 0; j < hi; j++) {
        if (cmp(stats, A[j], pivot) == -1) {
            //swaps based on comparisons against pivot
            swap(stats, &A[i], &A[j]);
            i++;
        }
    }
    swap(stats, &A[i], &A[hi]);
    return i;
}

void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    if (n <= 2) {
        //uses shell-short when all partitions have finished
        shell_sort(stats, A, n);
        return;
    }
    int p = partition(stats, A, n);
    //recursively calls both sub-arrays after partition
    quick_sort(stats, A, p);
    quick_sort(stats, (A + p + 1), (n - p - 1));
}
