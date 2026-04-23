#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "ldsl_functions.h"

#define DEFAULT_SIZE 100

typedef struct Stack {
    double *data;
    size_t top;
    size_t capacity;
} Stack;

Stack *s_create() {
    Stack *s = malloc(sizeof(Stack));

    if (!s) {
        return NULL;
    }

    s->data = malloc(DEFAULT_SIZE * sizeof(double));

    if (!s->data) {
        free(s);
        return NULL;
    }

    s->top = 0;
    s->capacity = DEFAULT_SIZE;

    return s;
}

int s_free(Stack *s) {
    if (!s) {
        return -1;
    }

    if (s->data) {
        free(s->data);
        s->data = NULL;
    }

    free(s);

    return 0;
}

int s_resize(Stack *s) {
    if (!s) {
        return -1;
    }

    size_t new_capacity = s->capacity * 2;

    // realloc extends in-place if possible, avoiding a malloc+memcpy+free
    double *buffer = realloc(s->data, new_capacity * sizeof(double));

    if (buffer == NULL) {
        return -1;
    }

    s->data = buffer;
    s->capacity = new_capacity;

    return 0;
}

int s_push(Stack *s, double value) {
    if (!s) {
        return -1;
    }

    if (s->top >= s->capacity) {
        if (s_resize(s) != 0) {
            return -1;
        }
    }

    // Post-increment: assign to data[top], then advance top to the next free slot
    s->data[s->top++] = value;

    return 0;
}

double s_pop(Stack *s) {
    if (!s || s->top == 0) {
        return NAN;
    }

    // Pre-decrement: move top back first, then read the value now at that index
    return s->data[--s->top];
}

double s_peek(Stack *s) {
    if (!s || s->top == 0) {
        return NAN;
    }

    return s->data[s->top - 1];
}

int s_print(Stack *s) {
    if (!s) {
        return -1;
    }

    printf("Values in current stack: [");
    for (size_t i = 0; i < s->top; i++) {
        printf("%.2f", s->data[i]);
        if (i < s->top - 1) {
            printf(", ");
        }
    }
    printf("]\n");

    return 0;
}