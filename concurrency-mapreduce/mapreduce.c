#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#include "mapreduce.h"
#include "pair.h"
#include "vector.h"
#include "hashtable.h"

hashtable_t* h;
hashtable_t* idx;
vector_t* raw;

int compare_pair (pair_t* a, pair_t* b) {
    return STR_COMPARATOR(a->first, b->first);
}

char* get_next (char* key, int partition_number) {
    vector_t* values = hashtable_get(h, key);
    int ind = hashtable_get(idx, key);
    hashtable_set(idx, key, ind+1);
    if (ind < values->size) {
        return values->data[ind];
    }
    return NULL;
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

void MR_Emit(char *key, char *value)
{
    pair_t* p = make_pair(strdup(key), strdup(value));
    vector_push(raw, p);
}

int file_number = 1;
pthread_mutex_t file_number_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    char** argv;
    int argc;
    Mapper map;
} mapper_arg;

void* mapper_thread (void* a) {
    mapper_arg* arg = (mapper_arg*) a;
    while (1) {
        pthread_mutex_lock(&file_number_lock);
        if (file_number < arg->argc) {
            int turn = file_number++;
            pthread_mutex_unlock(&file_number_lock);
            arg->map(arg->argv[turn]);
        } else {
            pthread_mutex_unlock(&file_number_lock);
            break;
        }
    }
}

typedef struct {
    int partition_number;
    Reducer reduce;
} reducer_arg;


pthread_mutex_t l = PTHREAD_MUTEX_INITIALIZER;

void* reducer_thread (void* a) {
    pthread_mutex_lock(&l);
    reducer_arg* arg = (reducer_arg*) a;
    // printf("%d\n", arg->partition_number);
    for (int i = 0; i < h->data[arg->partition_number]->size; i++) {
        pair_t* p = h->data[arg->partition_number]->data[i];
        char* key = p->first;
        arg->reduce(key, get_next, i);
    }
    pthread_mutex_unlock(&l);
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition)
{

    // map
    raw = make_vector();
    pthread_t* mappers = malloc(sizeof(pthread_t) * num_mappers);
    if (mappers == NULL) {
        printf("Error while creating mappers\n");
        exit(1);
    }
    for (int i = 0; i < num_mappers; i++) {
        mapper_arg* a = malloc(sizeof(mapper_arg));
        if (a == NULL) {
            printf("Error while creating mappers\n");
            exit(1);
        }
        a->argv = argv;
        a->argc = argc;
        a->map = map;
        pthread_create(&mappers[i], NULL, mapper_thread, a);
    }

    for (int i = 0; i < num_mappers; i++) {
        pthread_join(mappers[i], NULL);
    }

    // store the values associated with each key
    h = make_hashtable(num_reducers, (hash_function) partition);
    for (int i = 0; i < raw->size; i++) {
        pair_t* p = (pair_t*) raw->data[i];
        char* key = p->first;
        char* value = p->second;
        vector_t* v = hashtable_get(h, key);
        if (v == NULL) {
            v = make_vector();
            hashtable_set(h, key, v);
        }
        vector_push(v, value);
    }

    // sort the keys in each bucket in asc order
    for (int i = 0; i < h->capacity; i++) {
        if (h->data[i] == NULL) continue;
        vector_t* v = h->data[i];
        vector_sort(v, compare_pair);
    }

    // keep track of which value
    // we are at for each key
    idx = make_hashtable(h->size, (hash_function) partition);
    for (int i = 0; i < h->capacity; i++) {
        if (h->data[i] == NULL) continue;
        vector_t* v = h->data[i];
        for (int j = 0; j < v->size; j++) {
            pair_t* p = v->data[j];
            hashtable_set(idx, p->first, 0);
        }
    }

    // reduce
    pthread_t* reducers = malloc(sizeof(pthread_t) * num_reducers);
    if (reducers == NULL) {
        printf("Error while creating reducers\n");
        exit(1);
    }
    for (int i = 0; i < num_reducers; i++) {
        if (h->data[i] == NULL) continue;
        reducer_arg* arg = malloc(sizeof(reducer_arg));
        if (arg == NULL) {
            printf("Error while creating reducers\n");
            exit(1);
        }
        arg->partition_number = i;
        arg->reduce = reduce;
        pthread_create(&reducers[i], NULL, reducer_thread, arg);
    }
    for (int i = 0; i < num_reducers; i++) {
        if (h->data[i] == NULL) continue;
        pthread_join(reducers[i], NULL);
    }
}
