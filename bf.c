#include "bf.h"

#include "bv.h"
#include "math.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct
    BloomFilter { // Bloom filter structure has three salts that we will use to hash particular strings/keys
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = 0x5adf08ae86d36f21;
        bf->primary[1] = 0xa267bbd3116f3957;

        bf->secondary[0] = 0x419d292ea2ffd49e;
        bf->secondary[1] = 0x09601433057d5786;

        bf->tertiary[0] = 0x50d8bb08de3818df;
        bf->tertiary[1] = 0x4deaae187c16ae1d;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

void bf_delete(BloomFilter **bf) {
    free(*bf);
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    return (bv_length(bf->filter));
}

void bf_insert(BloomFilter *bf,
    char *
        oldspeak) { // To insert a word/oldspeak into our bloom filter we require three indexes created by hasing the same string/key with three different salts.
    uint32_t index1 = hash(bf->primary,
        oldspeak); // An index is created by hashing the string/key w/ a salt and using modulus to adapt it into the size of our filter.
    uint32_t index2 = hash(bf->secondary, oldspeak);
    uint32_t index3 = hash(bf->tertiary, oldspeak);
    index1 = (index1 % bf_size(bf));
    index2 = (index2 % bf_size(bf));
    index3 = (index3 % bf_size(bf));
    bv_set_bit(bf->filter, index1); // We set the bits at each of the three given indexes
    bv_set_bit(bf->filter, index2);
    bv_set_bit(bf->filter, index3);
    //printf("i1: %d i2: %d i3: %d \n", index1, index2, index3);
}

bool bf_probe(BloomFilter *bf,
    char *
        oldspeak) { // To probe our bloom filter we retrieve the three indexes. If it is in our filter, these indexes should be identical to the ones we used to insert the string/key
    uint32_t index1 = hash(bf->primary, oldspeak);
    uint32_t index2 = hash(bf->secondary, oldspeak);
    uint32_t index3 = hash(bf->tertiary, oldspeak);
    index1 = (index1 % bf_size(bf));
    index2 = (index2 % bf_size(bf));
    index3 = (index3 % bf_size(bf));
    uint8_t bit1 = bv_get_bit(bf->filter,
        index1); // By using the bit vector structure we created, we get the bit at each index.
    uint8_t bit2 = bv_get_bit(bf->filter, index2);
    uint8_t bit3 = bv_get_bit(bf->filter, index3);
    int sum = (bit1 + bit2 + bit3);
    if (sum == 3) { // If one of the bits are not set we know for certain that the
        return true;
    }
    return false;
}

uint32_t bf_count(
    BloomFilter *bf) { // The count of our filter is simply the amount of bits that are set.
    uint32_t count = 0;
    for (uint32_t i = 0; i < bv_length(bf->filter); ++i) {
        uint8_t bit = bv_get_bit(bf->filter, i);
        if (bit == 1) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter
        *bf) { // To print we simply print the filter part and use bv_print since it is a bitvector
    bv_print(bf->filter);
    return;
}
