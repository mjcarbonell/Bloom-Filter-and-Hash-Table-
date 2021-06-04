Maxim Joel Carbonell-Kiamtia
mjcarbon
Spring 2021
Assignment 7 

---------------------
DESCRIPTION: This directory is for the purposes of generating a memory of badspeak words, badspeak words with translations, reading a user's input and determing whether the have transgressions or not. Directory uses Bloom filter(uses a bitvector), Hash Table(uses linked lists and nodes), and a parsing module to read and determine transgressions. 

1. Enter "make" in the terminal
2. Enter "./banhammer" -[INSERT COMMANDs HERE]". To insert an input file for the program to read user must type " < [insert input file here]". Possible commands for ./encode i.e. "-h". Possible commands are ./banhammer i.e. "-h" for help message and "-s" for printing statistics, "-m" to enable move-to-front option to accelerate lookups into our hash linked lists, "-t" to specify the hash table size (default: 10000), and "-f" to specift bloom filter size (defualt: 2^20). 
---------------------
FILES
DESIGN.pdf
WRITEUP.pdf
Makefile
README.md
banhammer.c 
bf.c 
bf.h
bv.c
bv.h
ht.c
ht.h
ll.c
ll.h
messages.h
node.c 
node.h
parser.c
parser.h
speck.c
speck.h

-----------------------
banhammer.c contains the c program to perform all of the logic needed to utilize hash table, bloom filter, and parsing module to determine the transgressions and whether the user violated any of them.

bf.c contains the c program to maintain a bloom filter. Uses bit vector to store bits of the filter. Uses Hash function to generate indexes. 

bv.c contains the logic needed to set, c,lear, and get bits. Uses by bloom filter. 

ht.c contains the logic needed to maintain a hash table. Uses linked lists and nodes. If bloom filter has all three bits set of the given word/key, then hash table is used to ultimately check if the word is a transgression. If move-to-front is enabled then each time we lookup a word it will move the node to the front of the appropriate linked list. 

ll.c contains the logic needed to hold nodes. Uses sentinel nodes head and tail. We insert at the head and appropriately set the "nexts and prevs" of each node. 

messages.h contains the messages needed to output to the user. 

node.c contains the logic needed to maintain a linked list. Has attributes oldspeak, newspeak, next, and prev. Oldspeak and newspeak are used to identify the bad words/transgressions, while next and prev are for organizing our linked list. 

parser.c contains logic needed to parse through a given input file and return each word. 

speck.c contains logic needed to hash a word/key with a given salt. Returns a number that we use to index words/keys.

DESIGN.pdf contains a walkthrough or behind the scenes process to create or recreate this directory. 

WRITEUP.pdf contains graps comparing the total numberof seeks and average seek length as we vary the hash table and bloom filter size. Appropriate analysis of linked lists and bloom filter. 
---------------------



















