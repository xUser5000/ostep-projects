#include "types.h"
#include "user.h"

int main(int argc, char* argv[]) {
    printf(1, "%d\n", mprotect(main, 1));
    printf(1, "%d\n", munprotect(main, 1));
    exit();
}
