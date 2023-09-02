#include "trie.h"
#include "code.h"
#include <stdint.h>
#include <stdlib.h>

#include <stdio.h>

TrieNode *trie_node_create(uint16_t code) {
    //creates new TrieNode, sets children NULL, and returns pointer to it
    TrieNode *tn = (TrieNode *) malloc(sizeof(TrieNode));
    tn->code = code;
    for (int n = 0; n < ALPHABET; n += 1) {
        tn->children[n] = NULL;
    }
    return tn;
}

void trie_node_delete(TrieNode *n) {
    //deletes node by freeing memory
    free(n);
}

TrieNode *trie_create(void) {
    //creates root TrieNode, returns node if allocated correctly
    TrieNode *tn = trie_node_create(EMPTY_CODE);
    if (tn == NULL) {
        return NULL;
    } else {
        return tn;
    }
}

void trie_reset(TrieNode *root) {
    //resets trie when code reaches MAX_CODE by freeing root and child memory
    for (int n = 0; n < ALPHABET; n += 1) {
        trie_delete(root->children[n]);
    }
}

void trie_delete(TrieNode *n) {
    //trie destructor deletes nodes starting at n
    //recursively deletes children before root by freeing memory
    for (int k = 0; k < ALPHABET; k += 1) {
        if (n->children[k] != NULL) {
            trie_delete(n->children[k]);
        }
    }
    if (n != NULL) {
        trie_node_delete(n);
    }
}

TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    //checks if node has children called sym, returns the address if found, null if else
    if (n->children[sym] != NULL) {
        return n->children[sym];
    }
    return NULL;
}
