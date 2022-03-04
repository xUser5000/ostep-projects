#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "vector.h"

Vector PATH;

/* Change directory to the specified path */
void cd (char* path) {
    chdir(path);
}

/* Set the PATH Variable */
void path (Vector params) {
    PATH = create_vector();
    for (int i = 0; i < params.size; i++) push_back(&PATH, get(&params, i));
}

/* Given a line, return a vector of space-separated tokens */
Vector getTokens (char* l) {
    char* line = (char*) malloc( (strlen(l) + 1) * sizeof(char));
    strcpy(line, l);
    Vector ans = create_vector();
    char* token = NULL;
    while (line != NULL) {
        while(*line == ' ') line++;
        token = strsep(&line, " ");
        push_back(&ans, token);
    }
    return ans;
}

/* Given a string, removes leading and trailing white spaces */
char* trim (char* line) {
    int l = 0, r = strlen(line) - 1;
    while (line[l] == ' ') l++;
    while (line[r] == ' ') r--;
    int length = r - l + 1;
    char* ans = (char*) malloc((length + 1) * sizeof(char));
    strncpy(ans, line+l, length);
    ans[length] = '\0';
    return ans;
}


void execute_command (Vector line) {
    if (line.size == 1 && strcmp(get(&line, 0), "exit") == 0) exit(0);
    if (
        line.size == 2 &&
        strcmp(get(&line, 0), "cd") == 0
    ) {
        cd(get(&line, 1));
        return;
    }
    if (line.size >= 1 && strcmp("path", get(&line, 0)) == 0) {
        Vector params = create_vector();
        for (int i = 1; i < line.size; i++) push_back(&params, get(&line, i));
        path(params);
        for (int i = 0; i < PATH.size; i++) printf("%s ", get(&PATH, i));
        printf("\n");
        return;
    }
    for (int i = 0; i < line.size; i++) printf("%s ", get(&line, i));
    printf("\n");
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
        execute_command(getTokens(trim(line)));
    }

return 0;
}
