#ifndef __hashtable_h__
#define __hashtable_h__

#include "vector.h"

typedef struct __hashtable_t {
    int capacity, size;
    int (*hash_function)(char* key, int capacity);
    vector_t** data;
} hashtable_t;

int (*POLYNOMIAL_ROLLING_HASH_FUNCTION)(char* key, int capacity);

hashtable_t* make_hashtable(int capacity, int (*hash_function)(char* key, int capacity));
void hashtable_push(hashtable_t* h, char* key, void* value);
void* hashtable_get(hashtable_t* h, char *key);

#endif
