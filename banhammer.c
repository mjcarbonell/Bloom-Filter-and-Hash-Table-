/* 
Created by: Carbonell-Kiamtia, Maxim 
mjcarbon 
June 3rd, 2021

Assignment 7:  Great Firewall of Santa Cruz  
CSE 13S, Computer Systems and C Programming 
UC Santa Cruz, Spring 2021

Description: This directory is for the purposes of generating a memory of badspeak words, badspeak words with translations, reading a user's input and determing whether the have transgressions or not. Directory uses Bloom filter(uses a bitvector), Hash Table(uses linked lists and nodes), and a parsing module to read and determine transgressions. Depending on the transgression, the user will receive a particular message...
*/

#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"

#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    char c[50]; // Character array to hold each word we read from the input file
    char *arr[15000]; // Array to keep all of the dangerous words in memory
    bool mtf = false; // move to front option
    bool stats = false; // statistics option
    int count = 0; // count for our transgression array
    bool isBF = false; // Boolean to check if it passed the bloom filter
    int bloomDef = 1048576; // Bloom filter default size
    int hashDef = 10000; // Hash table default size

    FILE *badspeak = fopen("badspeak.txt", "r");
    FILE *newspeak = fopen("newspeak.txt", "r");
    for (int i = 0; i < argc; i++) { // Handles arguments
        if (strcmp(argv[i], "-h") == 0) {
            fprintf(stdout, "SYNOPSIS\n");
            fprintf(stdout, "	A word filtering program for the GPRSC.\nFilters out and reports "
                            "bad words parsed from stdin.\n\n");
            fprintf(stdout, "USAGE\n");
            fprintf(stdout, "	./banhammer [-hsm] [-t size] [-f size]\n");
            fprintf(stdout, "OPTIONS\n");
            fprintf(stdout, "	-h	program usage and help.\n");
            fprintf(stdout, "	-s	print program statistics.\n");
            fprintf(stdout, "	-m 	enabled move-to-front rule.\n");
            fprintf(stdout, "	-t size	Specificy hash table size (default: 10000).\n");
            fprintf(stdout, "	-f size	Specificy Bloom filter size (default: 2^20).\n");
        }
        if (strcmp(argv[i], "-t") == 0) {
            hashDef = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-f") == 0) {
            bloomDef = atoi(argv[i + 1]);
        }
        if (strcmp(argv[i], "-s") == 0) {
            stats = true;
        }
        if (strcmp(argv[i], "-m") == 0) {
            mtf = true;
        }
    }
    BloomFilter *bf = bf_create(bloomDef);
    HashTable *ht = ht_create(hashDef, mtf);
    //printf("HASH SIZE: %d\n", ht_size(ht));
    while (
        fscanf(badspeak, "%s", c)
        == 1) { // Reading in each word from badspeak and ensuring it gets added to our bloom filter and and hash table
        arr[count] = malloc(strlen(c) + 1);
        strcpy(arr[count], c);
        if (strcmp(arr[count], "the") == 0) {
            continue;
        }
        bf_insert(bf, arr[count]);
        ht_insert(ht, arr[count], NULL);
        count += 1;
    }
    while (
        fscanf(newspeak, "%s", c)
        == 1) { // Reading in each word from newspeak and ensuring it gets added to our bloom filter and hash table
        arr[count] = malloc(strlen(c) + 1);
        strcpy(arr[count], c);
        count += 1;
        fscanf(newspeak, "%s", c); // AGAIN FOR THE TRANSLATION
        arr[count] = malloc(strlen(c) + 1);
        strcpy(arr[count], c);
        bf_insert(bf, arr[count - 1]);
        ht_insert(ht, arr[count - 1], arr[count]);
        count += 1;
    }
    // READING FORM STDIN
    regex_t re; // To compile our regular expression
    if (regcomp(&re, "([a-zA-Z])*(-)*(')*([a-zA-Z0-9])+", REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex. \n");
        return 1;
    }
    char *word = NULL;
    LinkedList *thought
        = ll_create(false); // Linked list to store each transgression we find in the user's text
    LinkedList *correction
        = ll_create(false); // Hash table  to store each transgression we find in the user's text
    while ((word = next_word(stdin, &re)) != NULL) {
        isBF = false;
        for (int i = 0; word[i];
             i++) { // Converting to lowercase to match w/ the lowercase bad words...
            word[i] = tolower(word[i]);
        }
        //printf("Word: %s\n", word);
        isBF = bf_probe(
            bf, word); // If Bloomfilter has all three bits set, then it probably is a transgression
        if (isBF == true) {
            Node *foundNode = ht_lookup(ht, word);
            if (foundNode->oldspeak != NULL) {
                Node *foundNodeC = ll_lookup(correction,
                    foundNode
                        ->oldspeak); // We initialize two nodes to check whether we already have a transgression in our lists
                Node *foundNodeT = ll_lookup(thought, foundNode->oldspeak);
                if (foundNodeC->oldspeak
                    == foundNode
                           ->oldspeak) { // Check to see if we have already added to the user's transgressions
                    continue;
                }
                if (foundNodeT->oldspeak
                    == foundNode
                           ->oldspeak) { // Check to see if we have already added to the user's transgressions
                    continue;
                }
                if (foundNode->newspeak != NULL) {
                    ll_insert(correction, foundNode->oldspeak, foundNode->newspeak);
                    continue;
                }
                ll_insert(thought, foundNode->oldspeak, foundNode->newspeak);
            }
        }
    }
    if (stats == true) {
        printf("Seeks: %lu\n", seeks);
        double seeksFloat = seeks;
        double linksFloat = links;
        double slDiv = linksFloat / seeksFloat;
        printf("Average seek length: %f\n", slDiv);
        printf("Hash table load: %d\n",
            (ht_count(ht) / ht_size(ht))); // amount of items / total capacity
        double htCount = ht_count(ht);
        double htSize = ht_size(ht);
        double htDiv = htCount / htSize;
        printf("hash table load %f%c\n", (htDiv * 100), 37);
        double bfCount = bf_count(bf);
        double bfSize = bf_size(bf);
        double bfDiv = bfCount / bfSize;
        printf("Bloom filter load: %f%c", (bfDiv * 100), 37); // amount of items / total capacity
        return 0;
    }
    if (ll_length(thought) > 0
        && ll_length(correction)
               > 0) { // If thought and correct lists are nonzero then we must output a mixed message
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("We have some good news, and we have some bad news.\n");
        printf("The good news is that there is bad news. The bad news is that you will\n");
        printf("be sent to joycamp and subjected to a week-long destitute existence.\n");
        printf("This is the penalty for using degenerate words, as well as using\n");
        printf("oldspeak in place of newspeak. We hope you can correct your behavior.\n\n");
        printf("Your transgressions, followed by the words you must think on:\n\n");
        ll_print(thought);
        ll_print(correction);
        return 0;
    }
    if (ll_length(thought) > 0) { // if thought is nonzero then we output a thoughtcrime message
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("You have been caught using degenerate words that may cause\n");
        printf("distress among the moral and upstanding citizens of the GPSRC.\n");
        printf("As such, you will be sent to joycamp. It is there where you will\n");
        printf("sit and reflect on the consequences of your choice in language.\n\n");
        printf("Your transgressions:\n\n");
        ll_print(thought);
        return 0;
    }
    if (ll_length(correction)
        > 0) { // If correction list is nonzero then we output correction message
        printf("Dear beloved citizen of the GPRSC,\n\n");
        printf("We recognize your efforts in conforming to the language standards\n");
        printf("of the GPSRC. Alas, you have been caught uttering questionable words\n");
        printf("and thinking unpleasant thoughts. You must correct your wrongspeak\n");
        printf("and badthink at once. Failure to do so will result in your deliverance\n");
        printf("to joycamp.\n\n");
        printf("Words that you must think on:\n\n");
        ll_print(correction);
        return 0;
    }
    clear_words(); // Clearing our words
    regfree(&re); // Freeing our regular expression
    return 0;
}
