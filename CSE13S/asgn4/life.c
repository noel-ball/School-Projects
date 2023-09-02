#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"
#define DELAY   50000

int main(int argc, char **argv) {
    //Sets synopsis, stats, default variables, and pointers
    char Help[] = ("SYNOPSIS\n"
                   "    Conway's Game of Life\n"
                   "\n"
                   "USAGE\n"
                   "    ./life_amd64 tsn:i:o:h\n"
                   "\n"
                   "OPTIONS\n"
                   "    -t             Create your universe as a toroidal\n"
                   "    -s             Silent - do not use animate the evolution\n"
                   "    -n {number}    Number of generations [default: 100]\n"
                   "    -i {file}      Input file [default: stdin]\n"
                   "    -o {file}      Output file [default: stdout]");

    bool toroidal = false, display = true;
    int generations = 100, opt = 0;
    FILE *in, *out;
    in = stdin, out = stdout;

    //command line parsing for booleans
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        //activates toroidal universe
        case 't': toroidal = true; break;
        case 's':
            //silence ncurses
            display = false;
            break;
        case 'n':
            //stores generations if input
            generations = (int) strtol(optarg, (char **) NULL, 10);
            break;
        case 'i':
            //stores input file
            in = fopen(optarg, "r");
            break;
        case 'o':
            //stores length if input
            out = fopen(optarg, "w");
            break;
        case 'h':
            //prints Help if -h or nothing is input
            printf("%s\n", Help);
            return 1;
        default: printf("%s\n", Help); return 1;
        }
    }

    uint32_t rows, cols;
    uint32_t *x = &rows, *y = &cols;
    //only reads the bounds on first line to get universe dimensions
    if (fscanf(in, "%" SCNu32 " %" SCNu32 "\n", x, y) != 2) {
        fprintf(stderr, "Empty read.\n");
        return 1;
    }
    //creates universes A and B and populates the first step-universe
    Universe *A = NULL, *B = NULL, *Swap = NULL;
    A = uv_create(rows, cols, toroidal), B = uv_create(rows, cols, toroidal);
    if (!uv_populate(A, in)) {
        return 1;
    }
    //sets up ncurses unless silenced

    if (display) {
        initscr();
        curs_set(FALSE);
    }
    //universe generation loop
    for (int cur_gen = 0; cur_gen < generations; cur_gen += 1) {
        if (display) {
            clear();
            //displays each cell one at a time
            for (uint32_t r = 0; r < uv_rows(A); r += 1) {
                for (uint32_t c = 0; c < uv_cols(A); c += 1) {
                    if (uv_get_cell(A, r, c)) {
                        mvprintw(r, c, "o");
                    } else {
                        mvprintw(r, c, ".");
                    }
                }
            }
            refresh();
            usleep(DELAY);
        }
        //generation calculation cell by cell
        for (uint32_t r = 0; r < uv_rows(A); r += 1) {
            for (uint32_t c = 0; c < uv_cols(A); c += 1) {
                bool live = uv_get_cell(A, r, c);
                uint32_t neighbors = uv_census(A, r, c);
                //live cell with 2 or 3 neighbors stays live
                if ((live) && (neighbors >= 2) && (neighbors <= 3)) {
                    uv_live_cell(B, r, c);
                    continue;
                }
                //dead cell with 3 neighbors becomes live
                if ((!live) && (neighbors == 3)) {
                    uv_live_cell(B, r, c);
                    continue;
                }
                //all other cells die
                uv_dead_cell(B, r, c);
            }
        }
        //universe pointers are swapped
        Swap = B;
        B = A;
        A = Swap;
    }

    //closes window and prints and deletes universes
    endwin();
    uv_print(A, out);
    uv_delete(A);
    uv_delete(B);
    Swap = NULL;
    fclose(in);
    fclose(out);
    return 0;
}
