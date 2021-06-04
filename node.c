#include "node.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Node *node_create(
    char *oldspeak, char *newspeak) { // Nodes have two words/identifiers... oldspeak and newspeak.
    Node *n = (Node *) malloc(sizeof(Node));
    assert(n);
    n->oldspeak = oldspeak;
    n->newspeak = newspeak;
    return n;
}
void node_delete(Node **n) {
    free(*n);
    *n = NULL;
    return;
}

void node_print(Node *n) { // We wish to print the oldspeak and newspeak of a node
    if (n->oldspeak != NULL && n->newspeak != NULL) { // If both are non null we print both
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
        return;
    } else { // if only one is non null we know that whatever oldspeak is (including null) we will print it
        printf("%s\n", n->oldspeak);
        return;
    }
}
