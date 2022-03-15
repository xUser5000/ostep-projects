#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "vector.h"

Vector create_vector () {
    Vector v;
    v.data = (char**) malloc(INITIAL_VECTOR_CAPACITY * sizeof(char*));
    v.capacity = INITIAL_VECTOR_CAPACITY;
    v.size = 0;
    return v;
}

void push_back(Vector* v, char* val) {
    if (v->capacity == v->size) {
        char** new_data = (char**) malloc( (v->capacity << 1) * sizeof(char*));
        int n = v->size;
        for (int i = 0; i < n; i++) new_data[i] = v->data[i];
        free(v->data);
        v->data = new_data;
        v->capacity <<= 1;
    }
    v->data[v->size++] = val;
}

void pop_back(Vector* v) {
    if (v->size > 0) v->size--;
}

char* get (Vector* v, int index) {
    if (index < 0 || index >= v->size) return NULL;
    return v->data[index];
}

int get_size(Vector* v) {
    return v->size;
}

int search_key (Vector* v, char* key) {
    int n = v->size;
    for (int i = 0; i < n; i++) {
        if (strcmp(key, v->data[i]) == 0) return i;
    }
    return -1;
}

void destroy (Vector* v) {
    free(v->data);
}
