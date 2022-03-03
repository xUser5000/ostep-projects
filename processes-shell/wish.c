#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "vector.h"

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
