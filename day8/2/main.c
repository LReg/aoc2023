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
    unsigned int stepIndexes[10];
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
    /*while(nodePos != nodes) {
        printNode(*--nodePos);
    }*/
    printf("There are %u startPositions\n", stepIndexC);
    printf("overflow at %u\n", UINT_MAX);
    bool reached = false;
    unsigned int directionIndex = 0;
    unsigned int steps = 0;
    while (!reached) {
        if (directionIndex == dlen)
            directionIndex = 0;
        Direction dir = d[directionIndex++];
        bool reachedThisRound = true;
        for (int i = 0; i < stepIndexC; i++) {
            char * newPos = dir == LEFT ? nodes[stepIndexes[i]].left : nodes[stepIndexes[i]].right;
            stepIndexes[i] = findIndexOf(newPos);
        }
        for (int i = 0; i < stepIndexC; i++)
            if (!nodes[stepIndexes[i]].end)
                reachedThisRound = false;
        steps++;
        if (steps % 1000000 == 0)
            printf("%u/%u steps (%3.2f%)\n", steps, UINT_MAX, floor(((double)steps/(double)UINT_MAX) * 100));
        if (reachedThisRound)
            reached = true;
    }

    printf("steps: %u", steps);
    return 0;
}