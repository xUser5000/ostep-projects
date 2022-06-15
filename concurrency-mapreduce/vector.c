#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

vector_t* make_vector () {
    vector_t *v = (vector_t*) malloc(sizeof(vector_t));
    if (v == NULL) {
        printf("Error during vector creation\n");
        exit(1);
    }
    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->data = malloc(sizeof(void*) * INITIAL_CAPACITY);
    if (v->data == NULL) {
        printf("Error during vector creation\n");
        exit(1);
    }
    return v;
}

void grow (vector_t *v) {
    void** new_arr = (void**) malloc(sizeof(void*) * v->capacity * 2);
    for (int i = 0; i < v->size; i++)
        new_arr[i] = v->data[i];
    free(v->data);
    v->data = new_arr;
    v->capacity = v->capacity * 2;
}

void vector_push (vector_t* v, void* element) {
    if (v->capacity == v->size) grow(v);
    v->data[v->size] = element;
    v->size++;
}

void vector_pop (vector_t* v) {
    if (v->size == 0) {
        printf("Error: Vector is empty\n");
        exit(1);
    }
    v->size--;
}

void vector_sort (vector_t* v, int (*compare)(const void*, const void*)) {
    for (int i = 0; i < v->size; i++) {
        for (int j = i+1; j < v->size; j++) {
            if (compare(v->data[i], v->data[j]) == 0) {
                void* tmp = v->data[i];
                v->data[i] = v->data[j];
                v->data[j] = tmp;
            }
        }
    }
}

static int compare_int (const void* a, const void* b) {
    return (int) a <= (int) b;
}

static int compare_str (const void* a, const void* b) {
    return strcmp((char*) a, (char*) b) <= 0;
}

int (*INT_COMPARATOR)(const void*, const void*) = compare_int;
int (*STR_COMPARATOR)(const void*, const void*) = compare_str;
