// https://github.com/LReg/clib
#include "../../clib/lib.c"
#define MAXSTEPS 30

typedef struct Point{
    short x, y;
} Point;

typedef enum Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
} Direction;

typedef struct Entered{
    Point p;
    Direction dir;
    short straightCount;
    unsigned int steps;
    unsigned int sum;
} Entered;

short map[141][141];
int width, height;

Entered * deadEnds[1000000];
size_t deadEndc= 0;

Point * cyclePoints[100000];
size_t cyclePointsCount = 0;

bool pointInCycle(Point p) {
    for (size_t i = 0; i < cyclePointsCount; i++) {
        Point * cp = cyclePoints[i];
        if (cp->x == p.x && cp->y == p.y)
            return true;
    }
    return false;
}

void clearPoints() {
    for (size_t i = 0; i < cyclePointsCount; i++)
        free(cyclePoints[i]);
    cyclePointsCount = 0;
}

int alerdyEntered(Point p, Direction dir, short straightCount, unsigned int step, unsigned int sum) {
    for (size_t i = 0; i < deadEndc; i++) {
        Entered * e = deadEnds[i];
        if (e->p.x == p.x && e->p.y == p.y && e->dir == dir && e->straightCount == straightCount && e->steps < step && e->sum < sum)
            return 0;

    }
    return -2;
}

unsigned int lowest = UINT_MAX;

int findBestFrom(Point p, short straightCount, Direction lastDir, unsigned int step, unsigned int sum) {
    if (step > MAXSTEPS || sum >= lowest)
        return -1;
    if (p.x == width - 1 && p.y == height - 1) {
        //printf("Found end");
        if (sum + map[p.y][p.x] < lowest) {
            lowest = sum + map[p.y][p.x];
            printf("New lowest: %d\n", lowest);
        }
        return 0;
    }
    /*{ // to not keep the variable
        int alredyEnteredVal = alerdyEntered(p, lastDir, straightCount, step, sum);
        if (alredyEnteredVal != -2) {
            //printf("Already entered (%d, %d) with dir %d and straightCount %d\n", p.x, p.y, lastDir, straightCount);
            return -1;
        }
    }
    bool foundFurther = false;*/
    for (int i = 0; i < 4; i++) {
        Direction dir = i;
        short nextStraightCount = straightCount;
        if (dir == UP && lastDir == DOWN || dir == DOWN && lastDir == UP || dir == LEFT && lastDir == RIGHT || dir == RIGHT && lastDir == LEFT)
            continue;
        if (nextStraightCount> 3 && dir == lastDir)
            continue;
        if (dir == lastDir)
            nextStraightCount++;
        else
            nextStraightCount = 0;
        Point next = p;
        switch (dir) {
            case RIGHT:
                next.x++;
                break;
            case UP:
                next.y--;
                break;
            case DOWN:
                next.y++;
                break;
            case LEFT:
                next.x--;
                break;
        }
        if (next.x < 0 || next.y < 0 || next.x >= width || next.y >= height)
            continue;
        int erg = findBestFrom(next, nextStraightCount, dir, step + 1, sum + map[p.y][p.x]);
        /*
        if (erg == -1) {
            Entered * e = malloc(sizeof(Entered));
            e->p = next;
            e->dir = dir;
            e->straightCount = nextStraightCount;
            e->steps = step + 1;
            e->sum = sum + map[p.y][p.x];
            deadEnds[deadEndc++] = e;
        }
        else
            foundFurther = true;
             */
    }
    /*if (!foundFurther) {
        Entered * e = malloc(sizeof(Entered));
        e->p = p;
        e->dir = lastDir;
        e->straightCount = straightCount;
        e->steps = step;
        e->sum = sum;
        deadEnds[deadEndc++] = e;
    }*/
    return 0;
}

void printPath() {
    Point path[100000];
    int pathc = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool printed = false;
            for (int k = 0; k < pathc; k++) {
                if (path[k].x == j && path[k].y == i) {
                    printf("X");
                    printed = true;
                    break;
                }
            }
            if (!printed)
                printf("%hd", map[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    char *path = "day17/input.txt";
    if (1 || argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day17/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char lineBuffer[143];
    while (fgets(lineBuffer, 143, input) != NULL) {
        for (int i = 0; i < 141; i++)
            map[height][i] = lineBuffer[i] - '0';
        height++;
    }
    width = strlen(lineBuffer) - 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            printf("%hd", map[i][j]);
        printf("\n");
    }
    Point start = { 0, 0 };
    findBestFrom(start, 0, RIGHT, 0, 0);
    printPath();
    printf("Answer: %d\n", lowest);
    return 0;
}
