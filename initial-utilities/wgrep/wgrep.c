#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_match (char* line, char* key) {
    int line_sz = strlen(line);
    int key_sz = strlen(key);
    if (key_sz == 0) return 0;
    for (int i = 0; i < line_sz && i+key_sz-1 < line_sz; i++) {
        int same = 1;
        for (int j = 0; j < key_sz; j++) {
            if (line[i+j] != key[j]) {
                same = 0;
                break;
            }
        }
        if (same) return 1;
    }
    return 0;
}

void grep (char* searchterm, FILE* stream) {
    char* buffer = NULL;
    long unsigned int buffer_sz = 0;
    while (getline(&buffer, &buffer_sz, stream) != -1) {
        if (is_match(buffer, searchterm)) printf("%s", buffer);
    }
}

int main (int argc, char* argv[]) {

    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    char* searchterm = argv[1];

    if (argc == 2) {
        grep(searchterm, stdin);
        exit(0);
    }

    for (int i = 2; i < argc; i++) {
        FILE* file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }
        grep(searchterm, file);
        fclose(file);
    }

    return 0;
}
