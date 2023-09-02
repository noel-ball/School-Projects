#include "universe.h"

#include <inttypes.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    //allocating memory for the matrix and it's rows
    Universe *u = (Universe *) malloc(sizeof(Universe));
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    u->rows = rows;
    //allocates memory for every column through iterating rows
    for (uint32_t r = 0; r < rows; r += 1) {
        u->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    u->cols = cols;
    return u;
}

void uv_delete(Universe *u) {
    //frees memory for every column through iterating rows
    for (uint32_t r = 0; r < u->rows; r += 1) {
        free(u->grid[r]);
    }
    //frees memory for the matrix and it's rows
    free(u->grid);
    free(u);
    u = NULL;
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    //makes cell true if within bounds
    if (r < (u->rows)) {
        if (c < (u->cols)) {
            u->grid[r][c] = true;
        }
    }
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    //makes cell false if within bounds
    if (r < (u->rows)) {
        if (c < (u->cols)) {
            u->grid[r][c] = false;
        }
    }
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    //only returns true if a cell within bounds is true
    if (r < (u->rows)) {
        if (c < (u->cols)) {
            if ((u->grid[r][c]) == true) {
                return true;
            }
        }
    }
    return false;
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t r, c;
    uint32_t *x = &r, *y = &c;
    for (r = 0; r < u->rows; r += 1) {
        for (uint32_t c = 0; c < u->cols; c += 1) {
            //creates every cell as dead
            uv_dead_cell(u, r, c);
        }
    }
    while (fscanf(infile, "%" SCNu32 " %" SCNu32 "\n", x, y) != EOF) {
        //first line already read from main
        //populates every cell from file, returning false if out of bounds
        if ((r < (u->rows)) && (c < (u->cols))) {
            uv_live_cell(u, r, c);
        } else {
            fprintf(stderr, "Malformed input.\n");
            return false;
        }
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t neighbors = 0;
    long rows = (long) r, cols = (long) c;
    long t_adj_r, t_adj_c;
    //iterates through every adjacent row and col, and calculates toroidal adjacents
    for (long adj_r = rows - 1; adj_r <= rows + 1; adj_r += 1) {
        if (adj_r < 0) {
            t_adj_r = (adj_r + (u->rows));
        } else {
            t_adj_r = (adj_r % (u->rows));
        }
        for (long adj_c = cols - 1; adj_c <= cols + 1; adj_c += 1) {
            if (adj_c < 0) {
                t_adj_c = (adj_c + (u->cols));
            } else {
                t_adj_c = (adj_c % (u->cols));
            }
            //counts a neighbor if adjacent cells are live, excluding target cell for standard matrix
            if ((!u->toroidal) && uv_get_cell(u, adj_r, adj_c)
                && (adj_r != rows || adj_c != cols)) {
                neighbors += 1;
            }
            //counts a neighbor if toroidally adjacent cells are live, excluding target cell for toroidal matrix
            if ((u->toroidal) && uv_get_cell(u, t_adj_r, t_adj_c)
                && (adj_r != rows || adj_c != cols)) {
                neighbors += 1;
            }
        }
    }
    return neighbors;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < (u->rows); r += 1) {
        for (uint32_t c = 0; c < (u->cols); c += 1) {
            if (uv_get_cell(u, r, c)) {
                fprintf(outfile, "o");
            } else {
                fprintf(outfile, ".");
            }
        }
        fprintf(outfile, "\n");
    }
}
