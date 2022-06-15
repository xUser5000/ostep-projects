#define INITIAL_CAPACITY 8

typedef struct __vector_t {
    int size, capacity;
    void** data;
} vector_t;

vector_t* make_vector ();
void push (vector_t* v, void* element);
void pop (vector_t* v);
void sort (vector_t* v, int (*compare)(const void*, const void*));
int (*INT_COMPARATOR)(const void*, const void*);
int (*STR_COMPARATOR)(const void*, const void*);
