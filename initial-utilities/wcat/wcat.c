#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 512

void cat_file (char* path) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("wcat: cannot open file\n");
        exit(1);
    }
    char buff[BUFFER_SIZE];
    while (fgets(buff, BUFFER_SIZE, file) != NULL) {
        printf("%s", buff);
    }
    fclose(file);
}

int main (int argc, char* argv[]) {

    if (argc == 1) {
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        cat_file(argv[i]);
    }

    return 0;
}