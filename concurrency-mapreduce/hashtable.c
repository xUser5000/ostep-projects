#include "hashtable.h"
#include "pair.h"
#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hashtable_t* make_hashtable (
    int capacity,
    hash_function hash
)
{
    hashtable_t* hashtable = malloc(sizeof(hashtable));
    if (hashtable == NULL) {
        printf("Malloc: error while creating a hash table\n");
        exit(1);
    }
    hashtable->size = 0;
    hashtable->capacity = capacity;
    hashtable->hash = hash;
    hashtable->data = calloc(hashtable->capacity, sizeof(vector_t*));
    if (hashtable->data == NULL) {
        printf("Malloc: error while creating a hash table\n");
        exit(1);
    }
    return hashtable;
}

void hashtable_set (hashtable_t* h, char* key, void* value) {
    pair_t* p = make_pair(key, value);
    int idx = h->hash(key, h->capacity);
    if (h->data[idx] == NULL)
        h->data[idx] = make_vector();
    h->size++;
    for (int i = 0; i < h->data[idx]->size; i++) {
        pair_t* q = h->data[idx]->data[i];
        if (strcmp(key, q->first) == 0) {
            q->second = value;
            return;
        }
    }
    vector_push(h->data[idx], p);
}

void* hashtable_get (hashtable_t* h, char* key) {
    int hash = h->hash(key, h->capacity);
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
