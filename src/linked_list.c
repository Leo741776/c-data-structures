#include <stdio.h>
#include <stdlib.h>

#include "ldsl_functions.h"

typedef struct Node {
    double data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    Node *head;
    Node *tail;
    size_t size;
} LinkedList;

LinkedList *ll_create() {
    LinkedList *ll = malloc(sizeof(LinkedList));

    if (!ll) {
        return NULL;
    }

    ll->head = NULL;
    ll->tail = NULL;
    ll->size = 0;

    return ll;
}

int ll_free(LinkedList *ll) {
    if (!ll) {
        return -1;
    }

    Node *current_node = ll->head;
    Node *next_node;

    while (current_node != NULL) {
        // Save next before freeing, since current_node->next is inaccessible after free
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }

    free(ll);

    return 0;
}

Node *ll_at(LinkedList *ll, size_t index) {
    if (index >= ll->size) {
        return NULL;
    }

    Node *current = ll->head;

    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current;
}

int ll_insert_head(LinkedList *ll, double value) {
    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        return -1;
    }

    new_node->data = value;
    new_node->next = ll->head;
    ll->head = new_node;

    // On an empty list, the first node is both head and tail
    if (ll->size == 0) {
        ll->tail = new_node;
    }

    ll->size++;

    return 0;
}

int ll_insert_tail(LinkedList *ll, double value) {
    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        return -1;
    }

    new_node->data = value;
    new_node->next = NULL;

    if (ll->head == NULL) {
        ll->head = new_node;
        ll->tail = new_node;
    } else {
        ll->tail->next = new_node;
        ll->tail = new_node;
    }

    ll->size++;

    return 0;
}

int ll_insert_at(LinkedList *ll, double value, size_t index) {
    if (index == ll->size) {
        return ll_insert_tail(ll, value);
    }

    if (index == 0) {
        return ll_insert_head(ll, value);
    }

    Node *new_node = malloc(sizeof(Node));

    if (!new_node) {
        return -1;
    }

    Node *previous = ll_at(ll, index - 1);
    new_node->data = value;
    new_node->next = previous->next;
    previous->next = new_node;
    ll->size++;

    return 0;
}

int ll_remove_head(LinkedList *ll) {
    if (!ll->head) {
        return -1;
    }

    Node *buffer = ll->head;
    ll->head = buffer->next;
    free(buffer);
    ll->size--;

    if (ll->size == 0) {
        ll->tail = NULL;
    }

    return 0;
}

int ll_remove_tail(LinkedList *ll) {
    if (ll->size == 0) {
        return -1;
    }

    if (ll->size == 1) {
        return ll_remove_head(ll);
    }

    // The tail pointer can't walk backwards, so we must traverse to find the node before it
    Node *second_to_last = ll_at(ll, ll->size - 2);
    Node *node_to_delete = second_to_last->next;

    second_to_last->next = NULL;
    ll->tail = second_to_last;

    free(node_to_delete);
    ll->size--;

    return 0;
}

int ll_remove_at(LinkedList *ll, size_t index) {
    if (!ll->head) {
        return -1;
    }

    if (index == ll->size - 1) {
        return ll_remove_tail(ll);
    }

    if (index == 0) {
        return ll_remove_head(ll);
    }

    Node *current = ll_at(ll, index - 1);

    if (!current || !current->next) {
        return -1;
    }

    Node *node_to_delete = current->next;
    current->next = node_to_delete->next;
    free(node_to_delete);
    ll->size--;

    return 0;
}

void ll_print(LinkedList *ll) {
    if (!ll || !ll->head) {
        return;
    }

    Node *current = ll->head;
    printf("List (size %zu): ", ll->size);

    while (current != NULL) {
        printf("[%.2f] -> ", current->data);
        current = current->next;
    }

    printf("NULL\n");
}