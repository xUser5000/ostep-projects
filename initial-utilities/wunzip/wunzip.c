#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_run (int length, char ch) {
    while (length--) printf("%c", ch);
}

int main (int argc, char* argv[]) {

    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        FILE* file = fopen(argv[i], "r");
        int length;
        char ch;

        while (1) {
            if (
                fread(&length, sizeof(int), 1, file) != 0 &&
                fread(&ch, sizeof(char), 1, file) != 0
            ) {
                write_run(length, ch);
            } else {
                break;
            }
        }
        fclose(file);
    }

    return 0;
}