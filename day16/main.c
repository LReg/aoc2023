// https://github.com/LReg/clib
#include "../../clib/lib.c"
#include <time.h>
#define MAXLIGHTBEAMS 1000000
#define MAXSTEPS 1000

char grid[1000][112];
int width, height;

typedef enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct Point {
    int x, y;
} Point;

typedef struct LightBeam {
    Point p;
    Direction dir;
} LightBeam;

LightBeam * lightBeams[MAXLIGHTBEAMS];
int lightBeamCount = 0;

LightBeam * newLightBeam(Point p, Direction dir) {
    LightBeam * beam = malloc(sizeof(LightBeam));
    beam->p = p;
    beam->dir = dir;
    lightBeamCount++;
    return beam;
}

Point pointHistory[200000];
int pointHistoryCount = 0;

bool pointInHistory(Point p) {
    for (int i = 0; i < pointHistoryCount; i++)
        if (pointHistory[i].x == p.x && pointHistory[i].y == p.y)
            return true;
    return false;
}

void printLightBeams() {
    for (int i = 0; i < lightBeamCount; i++)
        if (lightBeams[i] != NULL)
            printf("Beam %d: (%d, %d)\n", i, lightBeams[i]->p.x, lightBeams[i]->p.y);
        else
            printf("Beam %d: NULL\n", i);
}

bool isOutOfBounds(Point p) {
    return p.x < 0 || p.x >= width || p.y < 0 || p.y >= height;
}

void addPointToHistory(Point p) {
    if (!pointInHistory(p) && pointHistoryCount < 200000 && !isOutOfBounds(p))
        pointHistory[pointHistoryCount++] = p;
}

void defragmentLightBeams() {
    int i = 0;
    while (i < lightBeamCount) {
        if (lightBeams[i] == NULL) {
            lightBeams[i] = lightBeams[i + 1];
            lightBeams[i + 1] = NULL;
        }
        i++;
    }
}

void deleteLightBeam(LightBeam * beam) {
    free(beam);
}

void deleteLightBeamAtPos(int pos) {
    deleteLightBeam(lightBeams[pos]);
    lightBeams[pos] = NULL;
    defragmentLightBeams();
    lightBeamCount--;
}

void lightBeamHitSymbol(LightBeam * beam, char symbol) {
    if (symbol == '/') {
        Point newPoint = beam->p;
        switch (beam->dir) {
            case UP:
                beam->dir = RIGHT;
                break;
            case DOWN:
                beam->dir = LEFT;
                break;
            case LEFT:
                beam->dir = DOWN;
                break;
            case RIGHT:
                beam->dir = UP;
                break;
        }
        beam->p = newPoint;
        return;
    }
    if (symbol == '\\') {
        Point newPoint = beam->p;
        switch (beam->dir) {
            case UP:
                beam->dir = LEFT;
                break;
            case DOWN:
                beam->dir = RIGHT;
                break;
            case LEFT:
                beam->dir = UP;
                break;
            case RIGHT:
                beam->dir = DOWN;
                break;
        }
        beam->p = newPoint;
        return;
    }
    if (symbol == '-') {
        if (beam->dir == RIGHT || beam->dir == LEFT) {
            return;
        }
        if (beam->dir == UP || beam->dir == DOWN) {
            Point p1 = beam->p, p2 = beam->p;
            lightBeams[lightBeamCount] = newLightBeam(p2, RIGHT);
            beam->p = p1;
            beam->dir = LEFT;
        }
    }
    if (symbol == '|') {
        if (beam->dir == UP || beam->dir == DOWN) {
            return;
        }
        if (beam->dir == RIGHT || beam->dir == LEFT) {
            Point p1 = beam->p, p2 = beam->p;
            lightBeams[lightBeamCount] = newLightBeam(p2, DOWN);
            beam->p = p1;
            beam->dir = UP;
        }
    }
}


void printGrid() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            printf("%c", grid[i][j]);
        printf("\n");
    }
}

void printHistory() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == '.') {
                if (pointInHistory((Point){j, i}))
                    printf("#");
                else
                    printf(".");
            }
            else
                printf("%c", grid[i][j]);
        }
        printf("\n");
    }
}

unsigned int countPoisitions() {
    unsigned int count = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            count += pointInHistory((Point){j, i});
    return count;
}

int calcForStartingpoint(Point p, Direction dir) {
    lightBeams[0] = newLightBeam(p, dir);
    for (int k = 0; k < MAXSTEPS; k++) {
        printf("Step %d\n", k);
        clock_t start = clock();
        for (int i = 0; i < lightBeamCount; i++) {
            if (lightBeams[i] == NULL)
                continue;
            LightBeam * beam = lightBeams[i];
            Point newPoint = beam->p;
            addPointToHistory(beam->p);
            switch (beam->dir) {
                case UP:
                    newPoint.y--;
                    break;
                case DOWN:
                    newPoint.y++;
                    break;
                case LEFT:
                    newPoint.x--;
                    break;
                case RIGHT:
                    newPoint.x++;
                    break;
            }
            if (isOutOfBounds(newPoint)) {
                //puts("Out of bounds!");
                deleteLightBeamAtPos(i);
                continue;
            }
            if (grid[newPoint.y][newPoint.x] == '.') {
                beam->p = newPoint;
                continue;
            }
            beam->p = newPoint;
            lightBeamHitSymbol(beam, grid[newPoint.y][newPoint.x]);
            if (lightBeamCount > MAXLIGHTBEAMS) {
                printf("Too many light beams!\n");
                exit(1);
            }
        }
        clock_t end = clock();
        const double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        if (seconds > 1.0) {
            break;
        }
        if (lightBeamCount == 0) {
            printf("No more light beams!\n");
            break;
        }
    }
    return countPoisitions();
}

void clean() {
    for (int i = 0; i < lightBeamCount; i++)
        deleteLightBeam(lightBeams[i]);
    lightBeamCount = 0;
    pointHistoryCount = 0;
}


int main(int argc, char *argv[]) {
    char *path = "day16/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day16/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    while(fgets(grid[height], 112, input) != NULL) {
        height++;
    }
    width = strlen(grid[0]);
    if (grid[0][width - 1] == '\n')
        width--;
    //printGrid();
    int highestCalc = 0;

    for (int i = 0; i < height; i++) {
        Point startingPoint = (Point){-1, i};
        int cal = calcForStartingpoint(startingPoint, RIGHT);
        if (cal > highestCalc) {
            highestCalc = cal;
        }
        clean();
        startingPoint = (Point){height, i};
        cal = calcForStartingpoint(startingPoint, LEFT);
        if (cal > highestCalc) {
            highestCalc = cal;
        }
        clean();

    }
    for (int i = 0; i < width; i++) {
        Point startingPoint = (Point){i, -1};
        int cal = calcForStartingpoint(startingPoint, DOWN);
        if (cal > highestCalc) {
            highestCalc = cal;
        }
        clean();
        startingPoint = (Point){i, width};
        cal = calcForStartingpoint(startingPoint, UP);
        if (cal > highestCalc) {
            highestCalc = cal;
        }
        clean();
    }

    printf("Counts Positions: %i\n", highestCalc);
}
