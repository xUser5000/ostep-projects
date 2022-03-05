#define INITIAL_VECTOR_CAPACITY 8

typedef struct Vector
{
    char** data;
    int capacity;
    int size;
} Vector;

Vector create_vector();
void push_back(Vector*, char*);
void pop_back(Vector*);
char* get(Vector*, int);

/* Returns the first occurrence of the given key in a vector. If not found, returns -1 */
int search_key (Vector*, char*);
