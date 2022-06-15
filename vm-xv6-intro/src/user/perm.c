#include "types.h"
#include "user.h"

const int PGSIZE = 4096;

char arr[4096 * 8];

int main (int argc, char* argv[]) {
    int l = (int) arr;
    int r = (int) arr + PGSIZE * 8;
    int x;
    for (x = l; x < r; x++) {
        if (x % PGSIZE == 0) break;
    }
    printf(1, "x = %d\n", x);
    mprotect((void*) x, 1);
    arr[x] = 'a';
    printf(1, "End\n");
    exit();
}
