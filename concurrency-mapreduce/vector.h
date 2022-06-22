#include <pthread.h>

#ifndef __vector_h__
#define __vector_h__

#define INITIAL_CAPACITY 8

typedef struct __vector_t {
    int size, capacity;
    void** data;
    pthread_mutex_t lock;
} vector_t;

vector_t* make_vector ();
void vector_push (vector_t* v, void* element);
void vector_pop (vector_t* v);
void vector_sort (vector_t* v, int (*compare)(const void*, const void*));
void vector_reverse(vector_t* v);
int (*INT_COMPARATOR)(const void*, const void*);
int (*STR_COMPARATOR)(const void*, const void*);

#endif
