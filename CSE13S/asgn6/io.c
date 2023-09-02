#include "io.h"
#include "endian.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int read_bytes(int infile, uint8_t *buf, int to_read) {
    //reads up to to_read bytes from infile to buf, returns total number of bytes read
    int br;
    int tb = 0;
    while (tb < to_read) {
        //if bytes read is less than to_read, continue reading the remaining amount
        br = read(infile, (buf + tb), to_read);
        if (br <= 0) {
            break;
        }
        if (br < to_read) {
            to_read -= br;
        }
        //add bytes read to total bytes
        tb += br;
    }
    return tb;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    //writes up to to_write bytes to outfile from buf, returns total number of bytes written
    int bw;
    int tb = 0;
    while (tb < to_write) {
        //if bytes written is less than to_write, continue writing the remaining amount
        bw = write(outfile, (buf + tb), to_write);
        if (bw < 0) {
            break;
        }
        if (bw < to_write) {
            to_write -= bw;
        }
        //add bytes written to total bytes
        tb += bw;
    }
    return tb;
}

void read_header(int infile, FileHeader *header) {
    //reads file header from infile to *header
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
    //swaps both header fields if run on big-endian system
    if (big_endian()) {
        swap32(header->magic);
        swap16(header->protection);
    }
    //asserts magic number is correct
    assert(header->magic == MAGIC);
}

void write_header(int outfile, FileHeader *header) {
    //swaps both header fields if run on big-endian system
    if (big_endian()) {
        swap32(header->magic);
        swap16(header->protection);
    }
    //writes file header to outfile from *header
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//read_sym buffer
static uint8_t symbuf[BLOCK];
static int symbuf_pos = 0;
static int symbuf_size = 0;

bool read_sym(int infile, uint8_t *sym) {
    //If there are no bytes in the buffer, it is filled from infile
    if (symbuf_pos == symbuf_size) {
        symbuf_size = read_bytes(infile, symbuf, BLOCK);
        symbuf_pos = 0;
        //returns false if there are no more more bytes to fill buffer
        if (symbuf_size <= 0) {
            return false;
        }
    }
    //reads the next symbol from the buffer and updates position
    *sym = symbuf[symbuf_pos];
    symbuf_pos += 1;
    return true;
}

//write_pair buffer
static uint8_t pairbuf[BLOCK];
static int pairbuf_pos = 0;

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    //writes code to buffer, starting at lsb of each byte
    for (int n = 0; n < bitlen; n += 1) {
        int bit = (code >> n) & 1;
        //write bit to buffer and increment position
        pairbuf[pairbuf_pos / 8] |= (bit << (pairbuf_pos % 8));
        pairbuf_pos += 1;
    }
    //writes sym to buffer, starting at lsb of each byte
    for (int n = 0; n < 8; n += 1) {
        int bit = (sym >> n) & 1;
        //write bit to buffer and increment position
        pairbuf[pairbuf_pos / 8] |= (bit << (pairbuf_pos % 8));
        pairbuf_pos += 1;
    }
    //if full, flush to output
    if (pairbuf_pos >= BLOCK * 8) {
        flush_pairs(outfile);
    }
}

void flush_pairs(int outfile) {
    //write the buffer to outfile, prints an error if failed
    int bw = write_bytes(outfile, pairbuf, pairbuf_pos / 8);
    if (bw != pairbuf_pos / 8) {
        fprintf(stderr, "Couldn't write pairs to outfile!\n");
        exit(1);
    }
    //zeroes out buffer with memset, resets position
    memset(pairbuf, 0, BLOCK);
    pairbuf_pos = 0;
}

//read_pair helper function and buffer
static uint8_t bitbuf[BLOCK];
static int bitbuf_pos = 0;
static int bitbuf_len = 0;

//reads one bit from infile into *bit, returns true if a bit was successfully read, false otherwise.
bool read_bit(int infile, uint8_t *bit) {
    //if all bits have been read from bitbuf, it is filled from infile
    if (bitbuf_pos == bitbuf_len) {
        bitbuf_len = read_bytes(infile, bitbuf, BLOCK);
        if (bitbuf_len <= 0) {
            return false;
        }
        bitbuf_pos = 0;
    }
    //get the next bit from bit_buffer
    *bit = (bitbuf[bitbuf_pos / 8] >> (bitbuf_pos % 8)) & 1;
    bitbuf_pos += 1;
    return true;
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    //read the code from the input file, starting with the least significant bit
    for (int n = 0; n < bitlen; n += 1) {
        uint8_t *c_bit = NULL;
        if (!read_bit(infile, c_bit)) {
            return false;
        }
        *code |= (*c_bit << n);
    }
    *sym = 0;
    //read the symbol from the input file, starting with the least significant bit
    for (int n = 0; n < 8; n += 1) {
        uint8_t *s_bit = NULL;
        if (!read_bit(infile, s_bit)) {
            return false;
        }
        *sym |= (*s_bit << n);
    }
    return true;
}

//write_word buffer
static uint8_t wordbuf[BLOCK];
static int wordbuf_pos = 0;

void write_word(int outfile, Word *w) {
    //writes every symbol from w to outfile
    for (uint32_t n = 0; n < w->len; n += 1) {
        wordbuf[wordbuf_pos++] = w->syms[n];
        if (wordbuf_pos == BLOCK) {
            flush_words(outfile);
        }
    }
}

void flush_words(int outfile) {
    //write the buffer to outfile, prints an error if failed
    int bw = write_bytes(outfile, wordbuf, wordbuf_pos);
    if (bw != wordbuf_pos) {
        fprintf(stderr, "Couldn't write words to outfile!\n");
        exit(1);
    }
    //zeroes out buffer with memset, resets position
    memset(wordbuf, 0, BLOCK);
    wordbuf_pos = 0;
}
