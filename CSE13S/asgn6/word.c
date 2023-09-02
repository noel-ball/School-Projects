#include "word.h"
#include "code.h"
#include <stdlib.h>

#include <stdio.h>

Word *word_create(uint8_t *syms, uint32_t len) {
    //creates new word syms by copying symbols over and len based on input
    Word *w = (Word *) malloc(sizeof(Word));
    w->syms = (uint8_t *) calloc(len, sizeof(uint8_t));
    for (uint32_t n = 0; n < len; n += 1) {
        w->syms[n] = syms[n];
    }
    w->len = len;
    return w;
}

Word *word_append_sym(Word *w, uint8_t sym) {
    //creates new word appending sym to word w by copying symbols and updating len
    uint8_t *asym = (uint8_t *) calloc((w->len + 1), sizeof(uint8_t));
    //copying symbols
    for (uint32_t n = 0; n < (w->len); n += 1) {
        asym[n] = w->syms[n];
    }
    //appending sym
    asym[w->len] = sym;
    //creating new word, and freeing symbol buffer memory
    Word *aw = word_create(asym, (w->len + 1));
    return aw;
}

void word_delete(Word *w) {
    //deletes w by freeing symbols and memory
    if (w) {
        if (w->syms) {
            free(w->syms);
        }
        free(w);
    }
}

WordTable *wt_create(void) {
    //creates a new table big enough for MAX_CODE
    WordTable *t = (WordTable *) calloc(MAX_CODE, sizeof(Word *));
    //with first element as empty word at EMPTY_CODE
    if (t != NULL) {
        t[EMPTY_CODE] = word_create(NULL, 0);
    }
    return t;
}

void wt_reset(WordTable *wt) {
    //deletes all existing words after EMPTY_CODE by freeing memory
    for (int n = START_CODE; n < MAX_CODE; n += 1) {
        if (wt[n]) {
            word_delete(wt[n]);
        }
        wt[n] = NULL;
    }
}

void wt_delete(WordTable *wt) {
    //iteratively deletes all words and then the table by freeing memory
    for (int n = 0; n < MAX_CODE; n += 1) {
        if (wt[n]) {
            word_delete(wt[n]);
        }
    }
    free(wt);
}
