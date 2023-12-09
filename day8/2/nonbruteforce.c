// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#include <math.h>
#define MAXLINE 1000
#define MAXDATASETS 3000
typedef enum Direction {
    LEFT = false,
    RIGHT = true
} Direction;
typedef struct Node {
    char at[3];
    char left[3];
    char right[3];
    bool end;
} Node;
Node nodes[MAXDATASETS];
Node * nodePos = nodes;
unsigned int nodec = 0;
void printNode(Node n) {
    printf("%.3s -> %.3s | %.3s\n", n.at, n.left, n.right);
}
unsigned int findIndexOf(char * str) {
    for (int i = 0; i < nodec; i++) {
        if (strncmp(nodes[i].at, str, 3) == 0)
            return i;
    }
    assert(false);
}
int main() {
    FILE* input = assertOpenFile("day8/2/input.txt", "r");
    char line[MAXLINE];
    fgets(line, MAXLINE, input);
    unsigned int dlen = strlen(line) - 1;
    Direction d[dlen];
    for (int i = 0; i < strlen(line) - 1; i++) {
        d[i] = line[i] == 'L' ? LEFT : RIGHT;
    }
    fgets(line, MAXLINE, input);
    unsigned int stepIndexes[6];
    unsigned int stepUntilEnd[6];
    unsigned int * stepindexPos = stepIndexes;
    unsigned int stepIndexC = 0;
    while(fgets(line, MAXLINE, input) != NULL) {
        Node n;
        strncpy(n.at, line, 3);
        strncpy(n.left, strchr(line, '(') + 1, 3);
        strncpy(n.right, strchr(line, ',') + 2, 3);
        n.end = n.at[2] == 'Z' ? true : false;
        *nodePos++ = n;
        if (n.at[2] == 'A') {
            *stepindexPos++ = nodec;
            stepIndexC++;
        }
        nodec++;
    }
    size_t sumSteps = 1;
    for (int i = 0; i < 6; i++) {
        bool reached = false;
        unsigned int directionIndex = 0;
        unsigned int steps = 0;
        while (!reached) {
            if (directionIndex == dlen)
                directionIndex = 0;
            Direction dir = d[directionIndex++];
            char * newPos = dir == LEFT ? nodes[stepIndexes[i]].left : nodes[stepIndexes[i]].right;
            stepIndexes[i] = findIndexOf(newPos);
            steps++;
            if (nodes[stepIndexes[i]].end) {
                stepUntilEnd[i] = steps;
                reached = true;
            }
        }
    }

    unsigned int maximum = 0;

    for (int i = 0; i < 6; i++) {
        printf("%u\n", stepUntilEnd[i]);
        if (stepUntilEnd[i] > maximum)
            maximum = stepUntilEnd[i];
    }

    unsigned int ggt;

    for (unsigned int i = maximum; i > 0; i--) {
        if (
                stepUntilEnd[0] % i == 0 &&
                stepUntilEnd[1] % i == 0 &&
                stepUntilEnd[2] % i == 0 &&
                stepUntilEnd[3] % i == 0 &&
                stepUntilEnd[4] % i == 0 &&
                stepUntilEnd[5] % i == 0
                ) {
            ggt = i;
            break;
        }

    }

    for (int i = 0; i < 6; i++) {
        sumSteps *= stepUntilEnd[i] / ggt;
    }

    sumSteps *= ggt;

    printf("ggt: %u\n", ggt);
    printf("sum: %zu", sumSteps);
    return 0;
}