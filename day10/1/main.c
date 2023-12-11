// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#define MAXLINE 1000
typedef struct Point {
    unsigned int horizontal;
    unsigned int vertical;
} Point;
char line[140][140];
unsigned int width, height;
char getCharAt(Point p) {
    return line[p.vertical][p.horizontal];
}
Point translatePoint(Point p, char c) {
    switch(c) {
        case '|': p.vertical++; return p;
        case '-': p.horizontal++; return p;
        case 'L': p.horizontal--; p.vertical++; return p;
        case 'J': p.horizontal--; p.vertical--; return p;
        case '7': p.horizontal++; p.vertical--; return p;
        case 'F': p.horizontal++; p.vertical++; return p;
        default: return p;
    }
}
int comparePoints(Point a, Point b) {
    return a.horizontal == b.horizontal && a.vertical == b.vertical;
}
void printPoint(Point p) {
    printf("(%i, %i) -> %c\n", p.vertical, p.horizontal, getCharAt(p));
}
Point translatePointStandingOn(Point p, char c, Point comingFrom) {
    switch(c) {
        case '|': comingFrom.vertical == p.vertical - 1 ? p.vertical++ : p.vertical--; return p;
        case '-': comingFrom.horizontal == p.horizontal - 1 ? p.horizontal++ : p.horizontal--; return p;
        case 'L': comingFrom.vertical == p.vertical - 1 ? p.horizontal++ : p.vertical--; return p;
        case 'J': comingFrom.horizontal == p.horizontal - 1 ? p.vertical-- : p.horizontal--; return p;
        case '7': comingFrom.vertical == p.vertical + 1 ? p.horizontal-- : p.vertical++ ; return p;
        case 'F': comingFrom.horizontal == p.horizontal + 1 ? p.vertical++ : p.horizontal++; return p;
        default: return p;
    }
}
Point findConnectedToS(Point p){
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            Point newPoint = {p.horizontal + i, p.vertical + j };
            if (newPoint.horizontal < 0 || newPoint.vertical < 0 || newPoint.horizontal >= width || newPoint.vertical >= height)
                continue;
            if (getCharAt(newPoint) == '.')
                continue;
            if (comparePoints(newPoint, p))
                continue;
            Point pointFromNewPoint = translatePointStandingOn(newPoint, getCharAt(newPoint), p);
            Point backToS = translatePointStandingOn(newPoint, getCharAt(newPoint), pointFromNewPoint);
            if (comparePoints(backToS, p))
                return newPoint;
        }
    }
    assert(false);
}
Point walk(Point p, char c, Point comingFrom) {
    Point afterWalk = translatePointStandingOn(p, c, comingFrom);
    return afterWalk;
}
Point findS() {
    Point p;
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            if(line[y][x] == 'S') {
                p.horizontal = x;
                p.vertical = y;
                return p;
            }
    return p;
}
int main() {
    FILE *input = assertOpenFile("day10/1/input.txt", "r");
    width = height = 0;
    while(fgets(line[height], MAXLINE, input) != NULL){
        width = strlen(line[height]);
        height++;
    }
    unsigned int steps = 0;
    Point p = findS();
    Point s = p;
    Point previous = s;
    p = findConnectedToS(p);
    printPoint(p);
    steps++;
    while (!comparePoints(p, s)) {
        Point newp = walk(p, getCharAt(p), previous);
        steps++;
        printPoint(newp);
        previous = p;
        p = newp;
    }
    printf("steps: %i\n", steps / 2);
}
