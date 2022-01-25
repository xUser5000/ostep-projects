#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_run (int length, char ch) {
    fwrite(&length, sizeof(int), 1, stdout);
    fwrite(&ch, sizeof(char), 1, stdout);
}

int main (int argc, char* argv[]) {

    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    char ch = 'a';
    int length = 0, first = 1;
    for (int i = 1; i < argc; i++) {
        FILE* file = fopen(argv[i], "r");
        char* buffer = NULL;
        long unsigned int n = 0, buffer_size = 0;
        while ((buffer_size = getline(&buffer, &n, file)) && buffer_size != -1) {
            for (int j = 0; j < buffer_size; j++) {
                if (first == 1) {
                    first = 0;
                    ch = buffer[j];
                    length = 1;
                } else if (buffer[j] == ch) {
                    length++;
                } else {
                    write_run(length, ch);
                    ch = buffer[j];
                    length = 1;
                }
            }
        }
        fclose(file);
    }
    write_run(length, ch);

    return 0;
}
