#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ldsl_functions.h"

#define DEFAULT_SIZE 100

typedef struct Entry {
    char *key;
    double value;
    struct Entry *next;
} Entry;

typedef struct HashTable {
    Entry **buckets;
    size_t count;
    size_t capacity_table;
} HashTable;

HashTable *ht_create() {
    HashTable *ht = malloc(sizeof(HashTable));

    if (!ht) {
        return NULL;
    }

    // calloc ensures all bucket pointers start as NULL without a manual loop
    ht->buckets = calloc(DEFAULT_SIZE, sizeof(Entry *));

    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->count = 0;
    ht->capacity_table = DEFAULT_SIZE;

    return ht;
}

int ht_free(HashTable *ht) {
    if (!ht) {
        return -1;
    }

    if (ht->buckets != NULL) {
        for (size_t i = 0; i < ht->capacity_table; i++) {
            Entry *current_entry = ht->buckets[i];
            Entry *next_entry;

            while (current_entry != NULL) {
                next_entry = current_entry->next;
                free(current_entry->key);
                free(current_entry);
                current_entry = next_entry;
            }
        }
        free(ht->buckets);
    }

    free(ht);

    return 0;
}

// djb2: bit-shifting by 5 then adding is equivalent to hash * 33, a well-tested multiplier for string hashing
unsigned int ht_hash(char *key) {
    if (!key) {
        return -1;
    }

    unsigned int hash = 5381;

    for (size_t i = 0; key[i] != '\0'; i++) {
        hash = ((hash << 5) + hash) + key[i];
    }

    return hash;
}

int ht_rehash(HashTable *ht) {
    if (!ht) {
        return -1;
    }

    size_t old_capacity = ht->capacity_table;
    size_t new_capacity = ht->capacity_table * 2;
    Entry **new_buckets = calloc(new_capacity, sizeof(Entry *));

    if (!new_buckets) {
        return -1;
    }

    for (size_t i = 0; i < old_capacity; i++) {
        Entry *current_entry = ht->buckets[i];
        while (current_entry != NULL) {
            Entry *next_entry = current_entry->next;

            // Index must be recalculated against the new capacity, not the old one
            unsigned int new_index = ht_hash(current_entry->key) % new_capacity;

            current_entry->next = new_buckets[new_index];
            new_buckets[new_index] = current_entry;

            current_entry = next_entry;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity_table = new_capacity;

    return 0;
}

void ht_insert(HashTable *ht, char *key, double value) {
    if (!ht || !key) {
        return;
    }

    if ((double) ht->count / ht->capacity_table > 0.7) {
        ht_rehash(ht);
    }

    unsigned int hash_value = ht_hash(key);
    size_t index = (size_t) hash_value % ht->capacity_table;
    Entry *current_entry = ht->buckets[index];

    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {
            current_entry->value = value;
            return;
        }
        current_entry = current_entry->next;
    }

    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) return;

    // strdup allocates its own copy so the caller's string lifetime doesn't affect the table
    new_entry->key = strdup(key);
    if (!new_entry->key) {
        free(new_entry);
        return;
    }

    new_entry->value = value;
    new_entry->next = ht->buckets[index];
    ht->buckets[index] = new_entry;
    ht->count++;
}

int ht_delete(HashTable *ht, char *key) {
    if (!ht || !key) {
        return -1;
    }

    size_t index = ht_hash(key) % ht->capacity_table;
    Entry *current_entry = ht->buckets[index];
    Entry *previous_entry = NULL;

    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {
            if (previous_entry == NULL) {
                ht->buckets[index] = current_entry->next;
            } else {
                previous_entry->next = current_entry->next;
            }

            free(current_entry->key);
            free(current_entry);
            ht->count--;

            return 0;
        }
        previous_entry = current_entry;
        current_entry = current_entry->next;
    }

    return -1;
}

double ht_search(HashTable *ht, char *key) {
    if (!ht || !key) {
        return NAN;
    }

    size_t index = ht_hash(key) % ht->capacity_table;
    Entry *current_entry = ht->buckets[index];

    while (current_entry != NULL) {
        if (strcmp(current_entry->key, key) == 0) {
            return current_entry->value;
        }
        current_entry = current_entry->next;
    }

    return NAN;
}

void ht_print(HashTable *ht) {
    for (size_t i = 0; i < ht->capacity_table; i++) {
        printf("Bucket %zu: ", i);
        Entry *current_entry = ht->buckets[i];
        while (current_entry) {
            printf("[%s: %.2f] -> ", current_entry->key, current_entry->value);
            current_entry = current_entry->next;
        }
        printf("NULL\n");
    }
}