#include "bv.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define byte_length(x) ((x / 8) + !!(x % 8)) // Borrowed from Professor Long

struct BitVector {
    uint32_t length; // Length in bits.
    uint8_t *vector; // Array of bytes.
};

BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    assert(v);
    v->length = length;
    v->vector = (uint8_t *) calloc(
        (byte_length(length)), sizeof(uint8_t)); // Allocating with precise byte length
    assert(v->vector);
    //printf("this is length %d\n", byte_length(length));
    return v;
}

void bv_delete(BitVector **v) {
    free((*v)->vector);
    free(*v);
    v = NULL;
    return;
}
uint32_t bv_length(BitVector *bv) {
    return (bv->length);
}

void bv_set_bit(BitVector *v, uint32_t i) {
    uint32_t index = floor(i / 8);
    v->vector[index]
        |= (1 << (i
                  % 8)); // setting a bit with OR operation to ensure that it becomes 1 and all else will maintain its identity
}

void bv_clr_bit(BitVector *v,
    uint32_t
        i) { // clearing bit by ANDing with zero and everything else w/ 1. Ensures all else keep their identity execpt given index
    uint32_t index = floor(i / 8);
    v->vector[index] &= ~(1 << (i % 8));
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    uint32_t index = floor(i / 8);
    return (v->vector[index] >> (i % 8) & 1); // indexing and returning a right shifted bit
}

void bv_print(BitVector *
        v) { // To print a bit vector we simply iterate through all bits until we reach the length. Then we get the bit by using a right shifted bit and print it.
    for (uint32_t i = 0; i < (v->length); ++i) {
        uint32_t index = floor(i / 8);
        if (i % 8 == 0) {
            printf("\n");
        }
        printf("%d  ", (v->vector[index] >> (i % 8) & 1));
    }
    printf("\n");
    //printf("v[] is %d\n", v->vector[0]);
}
