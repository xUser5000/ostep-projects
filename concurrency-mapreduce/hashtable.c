#include "hashtable.h"
#include "pair.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int polynomial_rolling_hash_function (char* key, int capacity) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % capacity;
}

int (*POLYNOMIAL_ROLLING_HASH_FUNCTION)(char* key, int capacity) = polynomial_rolling_hash_function;

hashtable_t* make_hashtable (
    int capacity,
    int (*hash_function)(char* key, int capacity)
)
{
    hashtable_t* hashtable = malloc(sizeof(hashtable));
    if (hashtable == NULL) {
        printf("Malloc: error while creating a hash table\n");
        exit(1);
    }
    hashtable->size = 0;
    hashtable->capacity = capacity;
    hashtable->hash_function = hash_function;
    hashtable->data = malloc(hashtable->capacity * sizeof(vector_t*));
    if (hashtable->data == NULL) {
        printf("Malloc: error while creating a hash table\n");
        exit(1);
    }
    return hashtable;
}

void hashtable_push (hashtable_t* h, char* key, void* value) {
    pair_t* p = make_pair(key, value);
    int idx = h->hash_function(key, h->capacity);
    if (h->data[idx] == NULL)
        h->data[idx] = make_vector();
    vector_push(h->data[idx], p);
    h->size++;
}

void* hashtable_get (hashtable_t* h, char* key) {
    int hash = h->hash_function(key, h->size);
    vector_t* v = h->data[hash];
    if (v == NULL)
        return NULL;
    for (int i = 0; i < v->size; i++) {
        pair_t* p = v->data[i];
        if (strcmp((char*) p->first, key) == 0)
            return p->second;
    }
    return NULL;
}
