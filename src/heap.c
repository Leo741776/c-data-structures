#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "ldsl_functions.h"

#define DEFAULT_CAPACITY 100

typedef struct Heap {
    double *data;
    size_t size;
    size_t capacity;
} Heap;

Heap *h_create() {
    Heap *h = malloc(sizeof(Heap));

    if (!h) {
        return NULL;
    }

    h->data = malloc(DEFAULT_CAPACITY * sizeof(double));
    h->size = 0;
    h->capacity = DEFAULT_CAPACITY;

    return h;
}

int h_free(Heap *h) {
    if (!h) {
        return -1;
    }

    free(h->data);
    free(h);

    return 0;
}

int h_resize(Heap *h) {
    if (!h) {
        return -1;
    }

    size_t new_capacity = h->capacity * 2;
    double *buffer = realloc(h->data, new_capacity * sizeof(double));

    if (!buffer) {
        return -1;
    }

    h->data = buffer;
    h->capacity = new_capacity;

    return 0;
}

void h_max_heapify(Heap *h, size_t i) {
    if (!h || h->size == 0) {
        return;
    }

    // For a 0-indexed array, left child = 2i+1 and right child = 2i+2
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    size_t largest = i;

    if (left < h->size && h->data[left] > h->data[largest]) {
        largest = left;
    }

    if (right < h->size && h->data[right] > h->data[largest]) {
        largest = right;
    }

    if (largest != i) {
        double buffer = h->data[i];
        h->data[i] = h->data[largest];
        h->data[largest] = buffer;

        h_max_heapify(h, largest);
    }
}

void h_insert(Heap *h, double value) {
    if (!h) {
        return;
    }

    if (h->size >= h->capacity) {
        if (h_resize(h) != 0) {
            return;
        }
    }

    size_t i = h->size;
    h->data[i] = value;
    h->size++;

    // For a 0-indexed array, the parent of node i is at (i-1)/2
    while (i > 0) {
        size_t parent = (i - 1) / 2;
        if (h->data[i] > h->data[parent]) {
            double buffer = h->data[i];
            h->data[i] = h->data[parent];
            h->data[parent] = buffer;
            i = parent;
        } else {
            break;
        }
    }
}

double h_peek(Heap *h) {
    return (h && h->size > 0) ? h->data[0] : NAN;
}

double h_pop_max(Heap *h) {
    if (!h || h->size == 0) {
        return NAN;
    }

    double root = h->data[0];

    // Move the last element to the root so the array stays contiguous, then sift it down
    h->data[0] = h->data[h->size - 1];
    h->size--;

    h_max_heapify(h, 0);

    return root;
}