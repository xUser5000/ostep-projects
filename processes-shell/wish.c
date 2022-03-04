#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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

/* Given the path of a binary, execute it */
void spawn_process (char* bin, char* argv[]) {
    int rc = fork();

    // child process
    if (rc == 0) {
        execv(bin, argv);
        return;
    }

    // parent process
    wait(NULL);
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

/* Given two strings, return the concatenation of them */
char* concat (char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

void execute_command (Vector line) {
    char* command = get(&line, 0);
    if (line.size == 1 && strcmp(command, "exit") == 0) exit(0);
    if (
        line.size == 2 &&
        strcmp(command, "cd") == 0
    ) {
        cd(get(&line, 1));
        return;
    }
    if (line.size >= 1 && strcmp("path", command) == 0) {
        Vector params = create_vector();
        for (int i = 1; i < line.size; i++) push_back(&params, get(&line, i));
        path(params);
        return;
    }
    for (int i = 0; i < PATH.size; i++) {
        char* p = concat(get(&PATH, i), concat("/", command));
        if (access(p, X_OK) == 0) {
            char** argv = (char**) malloc((line.size + 1) * sizeof(char*));
            argv[0] = p;
            for (int i = 1; i < line.size; i++) argv[i] = get(&line, i);
            argv[line.size] = NULL;
            spawn_process(p, argv);
            return;
        }
    }
    printf("Can't find the specified executable\n");
}

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
        execute_command(getTokens(trim(line)));
    }

return 0;
}
