#include "heap.h"

#include "stats.h"

static int max_child(Stats *stats, uint32_t *A, uint32_t n, uint32_t first) {
    //compares child nodes to find which is greater
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= n) && (cmp(stats, A[right - 1], A[left - 1]) == 1)) {
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, uint32_t n, uint32_t first) {
    //compares a mother node to its child nodes to find which is greater
    int found = 0;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, n, mother);
    while ((mother <= (n / 2)) && (found == 0)) {
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            //swaps mother and great if mother is smaller
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            great = max_child(stats, A, n, mother);
        } else {
            found = 1;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, uint32_t n, uint32_t first) {
    //organizes larger data towards the top of the heap
    for (uint32_t father = (n / 2); father > (first - 1); father--) {
        fix_heap(stats, A, n, father);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //moves data at the top of the heap to the bottom to be removed and sorted
    uint32_t first = 1;
    build_heap(stats, A, n, first);
    for (uint32_t leaf = n; leaf > first; leaf--) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, leaf - 1, first);
    }
}
