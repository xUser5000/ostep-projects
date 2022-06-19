#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mapreduce.h"
#include "pair.h"
#include "vector.h"
#include "hashtable.h"

unsigned long MR_DefaultHashPartition(char *key, int num_partitions)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

vector_t* v;



void MR_Emit(char *key, char *value)
{
    if (v == NULL)
        v = make_vector();
    pair_t* p = make_pair(key, value);
    vector_push(v, p);
}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition)
{

    for (int i = 1; i < argc; i++)
        map(argv[i]);
    

}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    hashtable_t* h = make_hashtable(111, POLYNOMIAL_ROLLING_HASH_FUNCTION);
    hashtable_push(h, "hello", 2);
    return 0;
}
