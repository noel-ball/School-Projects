#include "shell.h"

#include "gaps.h"
#include "stats.h"

void shell_sort(Stats *stats, uint32_t *arr, uint32_t length) {
    uint32_t cur_gap = 1;
    uint32_t range = 0;
    while (cur_gap > 0) { //loops through all of gaps ranges
        cur_gap = gaps[range];
        for (uint32_t i = cur_gap; i < length; i++) {
            //only runs when current gap is within the length of the array
            uint32_t j = i;
            uint32_t tmp = move(stats, arr[i]);
            while ((j >= cur_gap) && (cmp(stats, tmp, arr[j - cur_gap])) == -1) {
                //moves based on comparisons
                arr[j] = move(stats, arr[j - cur_gap]);
                j -= cur_gap;
            }
            arr[j] = move(stats, tmp);
        }
        range++;
    }
}
