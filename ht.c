#include "ht.h"

#include "ll.h"
#include "speck.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size,
    bool
        mtf) { // Hash table makes use of one salt divided into two because the max integer size is uint64_t
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = 0x9846e4f157fe8840;
        ht->salt[1] = 0xc5f318d7e055afb8;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    free(*ht);
    return;
}

uint32_t ht_size(HashTable *ht) {
    return (ht->size);
}

Node *ht_lookup(HashTable *ht,
    char *
        oldspeak) { // To lookup a word we retrieve the index by hashing the word w/ a salt and using modulo to adapt it to our length
    uint32_t index = hash(ht->salt, oldspeak);
    index = (index % ht->size);
    Node *result = node_create(NULL, NULL);
    if (ht->lists[index]
        == NULL) { // if the hashtable does not content a node at the index we generated then we return a null node
        return result;
    }
    Node *foundNode = ll_lookup(
        ht->lists[index], oldspeak); // Otherwise, we know a node exists and we will return it
    return foundNode;
}

void ht_insert(HashTable *ht, char *oldspeak,
    char *newspeak) { // To insert a node into a hash table we generate an index for the new node.
    uint32_t index = hash(ht->salt, oldspeak);
    index = (index % ht->size);
    //printf("index %u and oldspeak %s\n", index, oldspeak);
    if (ht->lists[index] == NULL) { // if there is no linked list at the index, then we create one
        LinkedList *ll = ll_create(ht->mtf);
        ht->lists[index] = ll;
    }
    ll_insert(ht->lists[index], oldspeak,
        newspeak); // Ultimately, we insert this new node into the linked list at our generated index
    //ll_print(ht->lists[index]);
    return;
}

uint32_t ht_count(
    HashTable *ht) { // The hash table's count is simply the amount of non null linked lists
    uint32_t count = 0;
    for (uint32_t i = 0; i < (ht->size); ++i) {
        if (ht->lists[i] != NULL) {
            count += 1;
        }
    }
    return count;
}

void ht_print(HashTable *ht) { // We wish to print out every linked list that is non null
    for (uint32_t i = 0; i < (ht->size); ++i) {
        if (ht->lists[i] != NULL) {
            printf("at index %d \n", i);
            ll_print(ht->lists[i]);
        }
    }
    return;
}
