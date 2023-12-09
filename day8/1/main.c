// https://github.com/LReg/clib
#include "../../../clib/lib.c"
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
    // This would complete in about 4 years on my machine
    FILE* input = assertOpenFile("day8/1/input.txt", "r");
    char line[MAXLINE];
    fgets(line, MAXLINE, input);
    unsigned int dlen = strlen(line) - 1;
    Direction d[dlen];
    for (int i = 0; i < strlen(line) - 1; i++) {
        d[i] = line[i] == 'L' ? LEFT : RIGHT;
    }
    fgets(line, MAXLINE, input);
    while(fgets(line, MAXLINE, input) != NULL) {
        Node n;
        strncpy(n.at, line, 3);
        strncpy(n.left, strchr(line, '(') + 1, 3);
        strncpy(n.right, strchr(line, ',') + 2, 3);
        *nodePos++ = n;
        nodec++;
    }
    /*while(nodePos != nodes) {
        printNode(*--nodePos);
    }*/
    bool reached = false;
    unsigned int stepIndex = findIndexOf("AAA");
    unsigned int directionIndex = 0;
    size_t steps = 0;
    while (!reached) {
        if (directionIndex == dlen)
            directionIndex = 0;
        Direction dir = d[directionIndex++];
        char * newPos = dir == LEFT ? nodes[stepIndex].left : nodes[stepIndex].right;
        stepIndex = findIndexOf(newPos);
        steps++;
        if (strncmp(nodes[stepIndex].at, "ZZZ", 3) == 0)
            reached = true;
    }
    printf("steps: %zu", steps);
    return 0;
}