#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "vector.h"

Vector PATH;

/* Given a command line, return a vector of logical tokens */
Vector parse (char* line);

/* check if the given character should be used as a delimeter for parse() */
int isDelimiter (char);

/* Given a string, removes leading and trailing white spaces */
char* trim (char* str);

/* Given two strings, return the concatenation of them */
char* concat (char* a, char* b);

/* Given a command as a vector of tokens, execute the command */
void execute_command (Vector tokens);

int main (int argc, char* argv[])
{

    push_back(&PATH, "/bin");

    while (1)
    {
        printf("wish> ");
        char* line = NULL;
        size_t n = 0;
        int len = getline(&line, &n, stdin);
        line[len-1] = '\0';
        execute_command(parse(line));
    }

return 0;
}

int isDelimiter (char c) {
    return (c == ' ') || (c == '\t') || (c == '>');
}

Vector parse (char* line) {
    Vector ans = create_vector();
    int n = strlen(line);
    char* s = NULL;
    int start = -1;
    for (int i = 0; i < n; i++) {
        if (line[i] == '>') {
            push_back(&ans, ">");
            continue;
        }
        if (!isDelimiter(line[i])) {
            if (i == 0 || isDelimiter(line[i-1])) start = i;
            if (i == n-1 || isDelimiter(line[i+1])) {
                s = strndup(line+start, i-start+1);
                push_back(&ans, s);
            }
        }
    }
    return ans;
}

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

char* concat (char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

void execute_command (Vector tokens) {
    char* command = get(&tokens, 0);
    if (tokens.size == 1 && strcmp(command, "exit") == 0) exit(0);
    if (
        tokens.size == 2 &&
        strcmp(command, "cd") == 0
    ) {
        chdir(get(&tokens, 1));
        return;
    }
    if (tokens.size >= 1 && strcmp("path", command) == 0) {
        Vector params = create_vector();
        for (int i = 1; i < tokens.size; i++) push_back(&params, get(&tokens, i));
        PATH = params;
        return;
    }
    for (int i = 0; i < PATH.size; i++) {
        char* p = concat(get(&PATH, i), concat("/", command));
        if (access(p, X_OK) == 0) {
            char** argv = (char**) malloc((tokens.size + 1) * sizeof(char*));
            argv[0] = p;
            for (int i = 1; i < tokens.size; i++) argv[i] = get(&tokens, i);
            argv[tokens.size] = NULL;
            if (fork() == 0) execv(p, argv);
            wait(NULL);
            return;
        }
    }
    printf("Can't find the specified executable\n");
}
