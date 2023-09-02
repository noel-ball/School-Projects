#include "trie.h"
#include "io.h"
#include "code.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>

#define OPTIONS "hi:o:"

int main(int argc, char **argv) {
    //sets synopsis, stats, and default file descriptors
    char Help[] = ("SYNOPSIS\n"
                   "   Compresses files using the LZ78 compression algorithm.\n"
                   "   Compressed files are decompressed with the corresponding decoder.\n"
                   "\n"
                   "USAGE\n"
                   "   ./encode [-h] [-i input] [-o output]\n"
                   "\n"
                   "OPTIONS\n"
                   "   -i input    Specify input to compress (stdin by default)\n"
                   "   -o output   Specify output of compressed input (stdout by default)\n"
                   "   -h          Display program help and usage\n");

    int infile = fileno(stdin), outfile = fileno(stdout), opt = 0;
    char in[64] = { 0 }, out[64] = { 0 };

    //command line parsing inputs
    while ((opt = (getopt(argc, argv, OPTIONS))) != -1) {
        switch (opt) {
        case 'i':
            //edits default file descriptor for infile
            (void) strcpy(in, optarg);
            break;
        case 'o':
            //edits default file descriptor for outfile
            (void) strcpy(out, optarg);
            break;
        case 'h':
            //prints Help if -h or nothing is input
            printf("%s\n", Help);
            return 1;
        }
    }

    //opening in and out files
    if (in[0] != 0) {
        infile = open(in, O_RDONLY);
    }
    if (infile == -1) {
        fprintf(stderr, "Infile couldn't be opened\n");
    }
    if (out[0] != 0) {
        outfile = open(out, O_WRONLY);
    }
    if (outfile == -1) {
        fprintf(stderr, "Outfile couldn't be opened\n");
    }

    //making a stat struct to store file size and protection for outfile
    struct stat s;
    struct FileHeader header;
    memset(&header, 0, sizeof(FileHeader));
    if (fstat(outfile, &s) != -1) {
        header.magic = MAGIC;
        header.protection = s.st_mode;
    } else {
        fprintf(stderr, "Fstat error!\n");
    }

    //writing header to outfile
    fchmod(outfile, header.protection);
    write_header(outfile, &header);
    //creating a trie, with a copy of root node
    TrieNode *root_node = trie_create();
    TrieNode *curr_node = root_node;
    //creating monotonic code counter
    uint16_t next_code = START_CODE;
    //keeping track of previous node and symbol
    TrieNode *prev_node;
    uint8_t prev_sym;

    //creating buffer to read symbols from infile
    uint8_t curr_sym;
    while (read_sym(infile, &curr_sym)) {
        //next node is a step down from current node to current symbol
        TrieNode *next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            //prefix is seen
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            //prefix is not seen
            write_pair(outfile, curr_node->code, curr_sym, sizeof(next_code) * 8);
            curr_node->children[curr_sym] = trie_node_create(next_code);
            next_code += 1;
        }
        if (next_code == MAX_CODE) {
            //out of finite codes, keep next_code within the max_code limit with modulo
            trie_reset(root_node);
            next_code = next_code % MAX_CODE;
        }
        prev_sym = curr_sym;
    }

    //after processing all the characters in infile, we may still be matching a prefix
    write_pair(outfile, prev_node->code, prev_sym, sizeof(next_code) * 8);
    //writes stop code pair and flushes the unwritten buffered pairs, and closes files
    write_pair(outfile, STOP_CODE, 0, sizeof(next_code) * 8);
    flush_pairs(outfile);
    trie_delete(root_node);
    close(infile);
    close(outfile);
}
