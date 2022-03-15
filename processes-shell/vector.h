#define INITIAL_VECTOR_CAPACITY 8

typedef struct Vector
{
    char** data;
    int capacity;
    int size;
} Vector;

/* Returns a new instance of Vector */
Vector create_vector();

/* Push the given string to the end of the vector */
void push_back(Vector*, char*);

/* Remove the last element in the vector */
void pop_back(Vector*);

/* Gets the string that resides in the given index */
char* get(Vector*, int);

/* Returns the first occurrence of the given key in a vector. If not found, returns -1 */
int search_key (Vector*, char*);

/* Free the space allocated by the vector to store its elements */
void destroy (Vector*);
