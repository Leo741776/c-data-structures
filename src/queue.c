#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "ldsl_functions.h"

#define DEFAULT_SIZE 100

typedef struct Queue {
    double *data;
    size_t head;
    size_t tail;
    size_t capacity;
    size_t size;
} Queue;

Queue *q_create() {
    Queue *q = malloc(sizeof(Queue));

    if (!q) {
        return NULL;
    }

    q->data = malloc(DEFAULT_SIZE * sizeof(double));

    if (!q->data) {
        free(q);
        return NULL;
    }

    q->head = 0;
    q->tail = 0;
    q->capacity = DEFAULT_SIZE;
    q->size = 0;

    return q;
}

int q_free(Queue *q) {
    if (!q) {
        return -1;
    }

    if (q->data) {
        free(q->data);
        q->data = NULL;
    }

    free(q);

    return 0;
}

int q_resize(Queue *q) {
    if (!q) {
        return -1;
    }

    size_t old_capacity = q->capacity;
    size_t new_capacity = q->capacity * 2;
    double *buffer = malloc(new_capacity * sizeof(double));

    if (!buffer) {
        return -1;
    }

    // Unwrap the circular layout into a flat [0..size] sequence in the new buffer
    for (size_t i = 0; i < q->size; i++) {
        buffer[i] = q->data[(q->head + i) % old_capacity];
    }

    free(q->data);
    q->data = buffer;
    q->head = 0;
    q->tail = q->size;
    q->capacity = new_capacity;

    return 0;
}

int q_enqueue(Queue *q, double value) {
    if (!q) {
        return -1;
    }

    if (q->size == q->capacity) {
        if (q_resize(q) != 0) {
            return -1;
        }
    }

    q->data[q->tail] = value;
    // Modulo wraps tail back to 0 when it reaches the end of the array
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;

    return 0;
}

double q_dequeue(Queue *q) {
    if (!q || q->size == 0) {
        return NAN;
    }

    double dequeued_value = q->data[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;

    return dequeued_value;
}

double q_peek(Queue *q) {
    if (!q || q->size == 0) {
        return NAN;
    }

    return q->data[q->head];
}

int q_print(Queue *q) {
    if (!q) {
        return -1;
    }

    printf("Queue: [");
    for (size_t i = 0; i < q->size; i++) {
        size_t index = (q->head + i) % q->capacity;
        printf("%.2f%s", q->data[index], (i < q->size - 1) ? ", " : "");
    }
    printf("]\n");

    return 0;
}