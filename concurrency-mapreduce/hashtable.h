#include "vector.h"

typedef struct __hashtable_t {
    int capacity, size;
    vector_t** data;
} hashtable_t;

hashtable_t* make_hashtable(int capacity);
void hashtable_push(char* key, void* value);
void hashtable_remove(char* key);
