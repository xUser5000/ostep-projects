#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "vector.h"

Vector PATH;

/* Given a command line, return a vector of logical tokens */
Vector parse (char* line);

/* check if the given character should be used as a delimeter for parse() */
int isDelimiter (char);

/* Returns true if the given command is valid in terms of the redirection syntax */
int isValidRedirection (Vector tokens);

/* Returns true if the given command is valid in terms of the Ampersand syntax */
int isValidAmpersand (Vector tokens);

/* Given two strings, return the concatenation of them */
char* concat (char* a, char* b);

/* Output the one and only error message to the screen */
void showError ();

/**
 * Given a command as a vector of tokens, execute the command
 * Returns the process id of the created process
 */
int execute_command (Vector tokens);

int main (int argc, char* argv[])
{

    if (argc > 2) exit(1);
    FILE* input_file = NULL;
    if (argc == 2) {
        input_file = freopen(argv[1], "r", stdin);
        if (input_file == NULL) exit(1);
    }

    push_back(&PATH, "/bin");

    while (1)
    {
        if (input_file == NULL) printf("wish> ");
        char* line = NULL;
        size_t n = 0;
        int len = getline(&line, &n, stdin);
        if (len == -1) {
            if (input_file == NULL) continue;
            break;
        }
        line[len-1] = '\0';
        Vector tokens = parse(line);
        if (!isValidAmpersand(tokens)) {
            showError();
            continue;
        }
        int commandsCount = 1;
        for (int i = 0; i < tokens.size; i++) {
            if (strcmp("&", get(&tokens, i)) == 0) commandsCount++;
        }
        Vector command = create_vector();
        int processIds[commandsCount];
        int sz = 0;
        for (int i = 0; i < tokens.size; i++) {
            if (strcmp("&", get(&tokens, i)) != 0) {
                push_back(&command, get(&tokens, i));
            }
            if (
                i == tokens.size - 1 ||
                strcmp("&", get(&tokens, i+1)) == 0
            ) {
                if (!isValidRedirection(command)) {
                    showError();
                    break;
                }
                int res = execute_command(command);
                if (res != -1) processIds[sz++] = res;
                command = create_vector();
            }
        }

        for (int i = 0; i < sz; i++) waitpid(processIds[i], NULL, 0);
    }

return 0;
}

int isDelimiter (char c) {
    return (c == ' ') || (c == '\t') || (c == '>') || (c == '&');
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
        if (line[i] == '&') {
            push_back(&ans, "&");
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

int isValidRedirection (Vector tokens) {
    int n = tokens.size;
    for (int i = 0; i < n; i++) {
        if (strcmp(">", get(&tokens, i)) == 0) {
            if (i == 0 || i == n-1 || n-1-i > 1) return 0;
            if (i != n-1 && strcmp(">", get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

int isValidAmpersand (Vector tokens) {
    int n = tokens.size;
    for (int i = 0; i < n; i++) {
        if (strcmp("&", get(&tokens, i)) == 0) {
            if (i == 0 || i == n-1) return 0;
            if (i != n-1 && strcmp("&", get(&tokens, i+1)) == 0) return 0;
        }
    }
    return 1;
}

char* concat (char* a, char* b) {
    char* ans = (char*) malloc((strlen(a) + strlen(b) + 1) * sizeof(char));
    strcpy(ans, a);
    strcat(ans, b);
    return ans;
}

void showError () {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message)); 
}

int execute_command (Vector tokens) {
    char* command = get(&tokens, 0);
    if (strcmp(command, "exit") == 0) {
        if (tokens.size != 1) showError();
        else exit(0);
        return -1;
    }
    if (
        strcmp(command, "cd") == 0
    ) {
        if (tokens.size != 2) showError();
        else chdir(get(&tokens, 1));
        return -1;
    }
    if (tokens.size >= 1 && strcmp("path", command) == 0) {
        Vector params = create_vector();
        for (int i = 1; i < tokens.size; i++) push_back(&params, get(&tokens, i));
        PATH = params;
        return -1;
    }
    
    /**
     * Spawn the required process to execute the command
     * Supports redirection to files
     */
    int pos = search_key(&tokens, ">");
    if (pos == -1) pos = tokens.size;
    for (int i = 0; i < PATH.size; i++) {
        char* p = concat(get(&PATH, i), concat("/", command));
        if (access(p, X_OK) == 0) {
            char* argv[pos+1];
            for (int i = 0; i < pos; i++) argv[i] = get(&tokens, i);
            argv[pos] = NULL;
            
            int rc = fork();
            if (rc == 0) {
                if (pos != tokens.size) {
                    close(STDOUT_FILENO);
                    open(
                        get(&tokens, tokens.size - 1),
                        O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRWXU
                    );
                }
                execv(p, argv);
            }
            return rc;
        }
    }

    showError();
    return -1;
}
