#include "trie.h"
#include "word.h"
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
                   "   Decompresses files with the LZ78 decompression algorithm.\n"
                   "   Used with files compressed with the corresponding encoder.\n"
                   "\n"
                   "USAGE\n"
                   "   ./decode [-h] [-i input] [-o output]\n"
                   "\n"
                   "OPTIONS\n"
                   "   -v          Display decompression statistics\n"
                   "   -i input    Specify input to decompress (stdin by default)\n"
                   "   -o output   Specify output of decompressed input (stdout by default)\n"
                   "   -h          Display program usage");

    int infile = 0, outfile = 1, opt = 0;
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

    //making the fileheader struct, and a stat struct to store protection for outfile
    struct FileHeader header;
    memset(&header, 0, sizeof(FileHeader));
    read_header(infile, &header);
    //setting permissions to outfile
    fchmod(outfile, header.protection);

    //creating a new word table with the empty word at index empty_code
    WordTable *table = wt_create();
    //code counters and symbol buffer
    uint16_t next_code = START_CODE;
    uint16_t curr_code;
    uint8_t curr_sym;

    //reading pairs from infile
    while (read_pair(infile, &curr_code, &curr_sym, sizeof(next_code) * 8)) {
        //stops reading at stop_code
        if (curr_code == STOP_CODE) {
            break;
        }
        //appending read symbol with the word denoted by the read code, and writing to file
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[curr_code]);
        //incrementing next_code and checking against max_code
        next_code += 1;
        if (next_code == MAX_CODE) {
            //mimics resetting of the trie during compression
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    //flushes the contents of our buffers and closes files
    flush_words(outfile);
    wt_delete(table);
    close(infile);
    close(outfile);
}
