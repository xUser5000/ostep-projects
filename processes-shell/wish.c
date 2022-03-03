#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "vector.h"

void execute_command (char* line) {
    if (strcmp(line, "exit") == 0) exit(0);
    printf("%s\n", line);
}

int main (int argc, char* argv[])
{

    while (1)
    {
        printf("wish> ");
        char* line = NULL;
        size_t n = 0;
        int len = getline(&line, &n, stdin);
        line[len-1] = '\0';
        execute_command(line);
    }

return 0;
}
