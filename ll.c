#include "ll.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t seeks; // Number of seeks performed.
uint64_t links; // Number of links traversed.
struct LinkedList {
    uint32_t length;
    Node *head; // Head sentinel node
    Node *tail; // Tail sentinel node
    bool mtf;
};

LinkedList *ll_create(bool mtf) { // Linked list creation
    LinkedList *l = (LinkedList *) malloc(sizeof(LinkedList));
    Node *head = node_create("head",
        NULL); // Head and tail to keep list organized... neither have a newspeak because they are sentinel nodes
    Node *tail = node_create("tail", NULL);
    if (l) {
        l->head = head;
        l->tail = tail;
        l->mtf = mtf;
        l->length = 0;
        return l;
    }
    return NULL;
}

void ll_delete(LinkedList **
        ll) { // DELETIGN A SERIES OF NODES... USING a "next" to know the next node before we delete the current node!
    free(*ll);
    return;
}

uint32_t ll_length(LinkedList *ll) {
    return (ll->length);
}

Node *ll_lookup(LinkedList *ll,
    char *
        oldspeak) { // To lookup a word we begin with the node in front of the head. We iterate through every single node up to the list's length and check if the current node's oldspeak is equal to the oldspeak given. If they are we return the node. Else we wish to move the current node to the next node in the list
    Node *cur = (ll->head->next);
    //printf("ll length: %d \n", ll->length);
    seeks += 1;
    for (uint32_t i = 0; i < (ll->length); ++i) {
        if (strcmp(cur->oldspeak, oldspeak) == 0) {
            // PERFORM LL INSERT TO FRONT OF LIST HERE
            if (ll->mtf == true) {
                cur->next->prev = cur->prev;
                cur->prev->next = cur->next;
            }
            return cur;
        }
        cur = cur->next;
        links += 1;
    }
    Node *nullNode
        = node_create(NULL, NULL); // In the event we do not find anything we return a null node
    return nullNode;
}

void ll_insert(LinkedList *ll, char *oldspeak,
    char *
        newspeak) { // To insert a node we check if length is zero for special case. IF it is then we create a node and place it in between the head and tail and increment length
    if (ll->length == 0) {
        Node *n = node_create(oldspeak, newspeak);
        //printf("this is node\n");
        //node_print(n);
        ll->head->next = n;
        n->prev = ll->head;
        n->next = ll->tail;
        ll->tail->prev = n;
        ll->length += 1;
        return;
    }
    Node *n = node_create(oldspeak,
        newspeak); // If length is nonzero then we create a new node and place it in front of the head. The node previously in front of head becomes the new "next" of n and the head becomes n's previous. Vice versa for the node in front of n.
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next = n;
    n->next->prev = n; // SEG FAULT BC NOTHING IS IN THE LIST YET
    ll->length += 1;
    return;
}

void ll_print(LinkedList
        *ll) { // We print from the node in front of head until we reach linked list's length.
    Node *cur = ll->head->next;
    for (uint32_t i = 0; i < (ll->length); ++i) {
        node_print(cur);
        cur = cur->next;
    }
    return;
}
