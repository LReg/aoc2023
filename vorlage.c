// https://github.com/LReg/clib
#include "../../../clib/lib.c"


int main(int argc, char *argv[]) {
    char *path = "input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "input.test.txt";
    FILE *input = assertOpenFile(path, "r");
}