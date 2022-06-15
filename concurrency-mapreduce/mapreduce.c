#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mapreduce.h"
#include "vector.h"

/*
Hash Table
-- Indexed List (AKA vector)
-- Pair
Merge Sort
*/

unsigned long MR_DefaultHashPartition(char *key, int num_partitions)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

void MR_Emit(char *key, char *value)
{

}

void MR_Run(int argc, char *argv[], 
	    Mapper map, int num_mappers, 
	    Reducer reduce, int num_reducers, 
	    Partitioner partition)
{

    

}

int main (int argc, char* argv[]) {
    srand(time(NULL));
    return 0;
}
