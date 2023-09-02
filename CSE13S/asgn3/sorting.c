#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "Hasbqhr:n:p:"

int main(int argc, char **argv) {
    //Sets synopsis, stats, default variables, and pointers
    char Help[] = ("SYNOPSIS\n"
                   "   A collection of comparison-based sorting algorithms.\n"
                   "\n"
                   "USAGE\n"
                   "   ./sorting_amd64 [-Hasbhqn:p:r:] [-n length] [-p elements] [-r seed]\n"
                   "\n"
                   "OPTIONS\n"
                   "   -H              Display program help and usage.\n"
                   "   -a              Enable all sorts.\n"
                   "   -s              Enable Shell Sort.\n"
                   "   -b              Enable Batcher Sort.\n"
                   "   -h              Enable Heap Sort.\n"
                   "   -q              Enable Quick Sort.\n"
                   "   -n length       Specify number of array elements (default: 100).\n"
                   "   -p elements     Specify number of elements to print (default: 100).\n"
                   "   -r seed         Specify random seed (default: 13371453).");
    Set sorts = set_empty();
    Stats *stats = &(Stats) { .moves = 0, .compares = 0 };
    int seed = 13371453, size = 100, printed = 100, opt = 0;
    uint32_t *sorted, *unsorted, random;

    //command line parsing into set
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        //activates all sorts
        case 'a':
            sorts = set_insert(sorts, 0);
            sorts = set_insert(sorts, 1);
            sorts = set_insert(sorts, 2);
            sorts = set_insert(sorts, 3);
            break;
        case 's':
            //activates shell-sort if not already
            if (!set_member(sorts, 0))
                sorts = set_insert(sorts, 0);
            break;
        case 'b':
            //activates batcher-sort if not already
            if (!set_member(sorts, 1))
                sorts = set_insert(sorts, 1);
            break;
        case 'q':
            //activates quick-sort if not already
            if (!set_member(sorts, 2))
                sorts = set_insert(sorts, 2);
            break;
        case 'h':
            //activates heap-sort if not already
            if (!set_member(sorts, 3))
                sorts = set_insert(sorts, 3);
            break;
        case 'r':
            //stores seed if input
            seed = (uint32_t) strtol(optarg, (char **) NULL, 10);
            break;
        case 'n':
            //stores length if input
            size = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'p':
            //stores print length if input
            printed = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'H':
            //prints Help if -H or nothing is input
            printf("%s\n", Help);
            return 1;
        default: printf("%s\n", Help); return 1;
        }
    }

    //creating arrays to be sorted
    unsorted = (uint32_t *) calloc(size, sizeof(uint32_t));
    sorted = (uint32_t *) calloc(size, sizeof(uint32_t));
    srand(seed);
    for (int i = 0; i < size; i++) {
        //bit-masking random number to 30 bits
        random = rand() & 0x3FFFFFFF;
        unsorted[i] = random;
    }
    //ensures the array and print are the correct size
    for (int i = 0; i < size; i++)
        sorted[i] = unsorted[i];
    if (printed > size) {
        printed = size;
    }

    //printing shell-sort
    if (set_member(sorts, 0)) {
        shell_sort(stats, sorted, size);
        printf("Shell Sort, %i elements, %lu moves, %lu compares\n", size, stats->moves,
            stats->compares);
        //loops the array to print desired elements
        for (int i = 0; i < printed; i++) {
            printf("%13" PRIu32 "", sorted[i]);
            if (((i + 1) % 5 == 0) && (i != 0)) {
                printf("\n");
            }
        }
        //resetting the array
        for (int i = 0; i < size; i++) {
            sorted[i] = unsorted[i];
        }
    }
    reset(stats);

    //printing batcher-sort
    if (set_member(sorts, 1)) {
        batcher_sort(stats, sorted, size);
        printf("Batcher Sort, %i elements, %lu moves, %lu compares\n", size, stats->moves,
            stats->compares);
        //loops the array to print desired elements
        for (int i = 0; i < printed; i++) {
            printf("%13" PRIu32 "", sorted[i]);
            if (i != 0 && (i + 1) % 5 == 0) {
                printf("\n");
            }
        }
        //resetting the array
        for (int i = 0; i < size; i++) {
            sorted[i] = unsorted[i];
        }
    }
    reset(stats);

    //printing quick-sort
    if (set_member(sorts, 2)) {
        quick_sort(stats, sorted, size);
        printf("Quick Sort, %i elements, %lu moves, %lu compares\n", size, stats->moves,
            stats->compares);
        //loops the array to print desired elements
        for (int i = 0; i < printed; i++) {
            printf("%13" PRIu32 "", sorted[i]);
            if (i != 0 && (i + 1) % 5 == 0) {
                printf("\n");
            }
        }
        //resetting the array
        for (int i = 0; i < size; i++) {
            sorted[i] = unsorted[i];
        }
    }
    reset(stats);

    //printing heap-sort
    if (set_member(sorts, 3)) {
        heap_sort(stats, sorted, size);
        printf("Heap Sort, %i elements, %lu moves, %lu compares\n", size, stats->moves,
            stats->compares);
        //loops the array to print desired elements
        for (int i = 0; i < printed; i++) {
            printf("%13" PRIu32 "", sorted[i]);
            if (i != 0 && (i + 1) % 5 == 0) {
                printf("\n");
            }
        }
        //resetting the array
        for (int i = 0; i < size; i++) {
            sorted[i] = unsorted[i];
        }
    }
    reset(stats);

    //freeing memory to avoid leaks
    free(unsorted);
    free(sorted);
    return 0;
}
