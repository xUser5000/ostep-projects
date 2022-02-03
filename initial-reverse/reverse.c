#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define INITIAL_CAPACITY 8

char** arr = NULL;
int capacity = INITIAL_CAPACITY, size = 0;

char** allocate_arr (int capacity) {
    char** ans = (char**) malloc(capacity * sizeof(char*));
    if (ans == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    return ans;
}

FILE* open_file (char* file_name, char* mode) {
    FILE* file = fopen(file_name, mode);
    if (file == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", file_name);
        exit(1);
    }
    return file;
}

int same_file(int fd1, int fd2) {
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) return -1;
    if(fstat(fd2, &stat2) < 0) return -1;
    return (stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino);
}

void reverse (FILE* input_file, FILE* output_file) {
    if (arr == NULL) arr = allocate_arr(INITIAL_CAPACITY);
    if (size == capacity) {
        char** new_arr = allocate_arr(capacity * 2 * sizeof(char*));
        for (int i = 0; i < size; i++) new_arr[i] = arr[i];
        capacity <<= 1;
    }
    size_t sz = 0;
    while (getline(&arr[size], &sz, input_file) != -1) {
        size++;
    }
    for (int i = size-1; i >= 0; i--) fprintf(output_file, "%s", arr[i]);
}

int main (int argc, char* argv[]) {
    FILE *input_file, *output_file;

    switch (argc) {
        
        case 1:
            reverse(stdin, stdout);
            break;

        case 2:
            input_file = open_file(argv[1], "r");
            reverse(input_file, stdout);
            break;

        case 3:
            input_file = open_file(argv[1], "r");
            output_file = open_file(argv[2], "w");
            if (same_file(fileno(input_file), fileno(output_file))) {
                fprintf(stderr, "reverse: input and output file must differ\n");
                exit(1);
            }
            reverse(input_file, output_file);
            break;

        default:
            fprintf(stderr, "usage: reverse <input> <output>\n");
            exit(1);
    }

    return 0;
}
