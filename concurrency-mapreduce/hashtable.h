#ifndef __hashtable_h__
#define __hashtable_h__

#include "vector.h"

typedef unsigned long (*hash_function)(char* key , int capacity);

typedef struct __hashtable_t {
    int capacity, size;
    hash_function hash;
    vector_t** data;
} hashtable_t;

hashtable_t* make_hashtable(int capacity, hash_function hash);
void hashtable_set(hashtable_t* h, char* key, void* value);
void* hashtable_get(hashtable_t* h, char *key);

#endif
