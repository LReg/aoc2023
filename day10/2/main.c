// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#define MAXLINE 1000
#define PATHFINDDEPTH 400
typedef struct Point {
    unsigned int horizontal;
    unsigned int vertical;
    bool isPath;
} Point;

typedef struct DotPoint {
    Point p;
    bool hasPathToBorder;
} DotPoint;
DotPoint dotPoints[6000];
DotPoint * dotPointPos = dotPoints;
unsigned int dotPointsLen = 0;
char line[140][140];
unsigned int width, height;
unsigned int sumOfPathsToBorder = 0;
Point points[100000];
Point * pointPos = points;
unsigned int pointsLen = 0;
char map[140 * 2][140 * 2];

typedef enum Direction {
    NONE = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
} Direction;

typedef enum FIELD{
    VISITED,
    UNVISITED,
    WALL,
} FIELD;

typedef struct Mappoint {
    Point p;
    struct Mappoint * previous;
    bool hasPathToBorder;
    bool noFurtherPath;
} Mappoint;

Mappoint mappoints[1000000];
Mappoint * mappointPos = mappoints;
unsigned int mappointsLen = 0;

char getCharAt(Point p) {
    return line[p.vertical][p.horizontal];
}
char getCharAtMap(Point p) {
    return map[p.vertical][p.horizontal];
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
void printPointMap(Point p) {
    printf("(%i, %i) -> %c\n", p.vertical, p.horizontal, getCharAtMap(p));
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

bool hasMappoint(Point p) {
    for (int i = 0; i < mappointsLen; i++) {
        if (comparePoints(mappoints[i].p, p))
            return true;
    }
    return false;
}

Mappoint * getMappoint(Point p) {
    for (int i = 0; i < mappointsLen; i++) {
        if (comparePoints(mappoints[i].p, p))
            return &mappoints[i];
    }
    return NULL;
}

void borderReachedForMappoint(Mappoint * mappoint) {
    mappoint->hasPathToBorder = true;
    mappoint->noFurtherPath = true;
    while(mappoint->previous != NULL) {
        mappoint = mappoint->previous;
        mappoint->hasPathToBorder = true;
    }
}
bool isWall(Point p) {
    return getCharAtMap(p) == 'X';
}
bool pointIsOutside(Point p) {
    return p.horizontal < 0 || p.vertical < 0 || p.horizontal >= width * 2 || p.vertical >= height * 2;
}
bool hasNotVisitedNeighbor(Mappoint * mappoint) {
    for (int i = 1; i <= 4; i++) {
        Point newPoint = mappoint->p;
        switch(i) {
            case UP: newPoint.vertical--; break;
            case DOWN: newPoint.vertical++; break;
            case LEFT: newPoint.horizontal--; break;
            case RIGHT: newPoint.horizontal++; break;
        }
        if (pointIsOutside(newPoint)) {
            //borderReachedForMappoint(mappoint);
            continue;
        }
        if (isWall(newPoint))
            continue;
        if (hasMappoint(newPoint)) {
            continue;
        }
        return true;
    }
    return false;
}

bool neighborHasReachedBorder(Mappoint * mappoint) {
    for (int i = 1; i <= 4; i++) {
        Point newPoint = mappoint->p;
        switch(i) {
            case UP: newPoint.vertical--; break;
            case DOWN: newPoint.vertical++; break;
            case LEFT: newPoint.horizontal--; break;
            case RIGHT: newPoint.horizontal++; break;
        }
        if (pointIsOutside(newPoint)) {
            return true;
        }
        if (isWall(newPoint))
            continue;
        if (hasMappoint(newPoint)) {
            Mappoint * neighbor = getMappoint(newPoint);
            if (neighbor->hasPathToBorder)
                return true;
            /*if (pointIsOutside(neighbor->p)) {
                return true;
            }*/
        }
    }
    return false;
}

void pathfindToBorder() {
    for (int i = 0; i < height * 2; i++) {
        for (int j = 0; j < width * 2; j++) {
            if (map[i][j] == '.') {
                *dotPointPos++ = (DotPoint) { (Point) {j, i}, false };
                dotPointsLen++;
            }
        }
    }

    for (int i = 0; i < dotPointsLen; i++) {
        Point p = dotPoints[i].p;
        if (hasMappoint(p))
            continue;
        Mappoint mappoint  = { p, NULL, false, false};
        *mappointPos++ = mappoint;
        mappointsLen++;
    }
    printf("mappointsLen: %u\n", mappointsLen);
    for (int j = 0; j < PATHFINDDEPTH; j++) {
        printf("j: %i\n", j);
        for (int k = j; k < mappointsLen; k+=3) {
            Mappoint * mappoint = &mappoints[k];
            if (mappoint->hasPathToBorder || mappoint->noFurtherPath)
                continue;
            if (!hasNotVisitedNeighbor(mappoint)) {
                mappoint->noFurtherPath = true;
                continue;
            }
            for (int dir = 1; dir <= 4; dir ++){
                Point newPoint = mappoint->p;
                switch(dir) {
                    case UP: newPoint.vertical--; break;
                    case DOWN: newPoint.vertical++; break;
                    case LEFT: newPoint.horizontal--; break;
                    case RIGHT: newPoint.horizontal++; break;
                }
                if (pointIsOutside(newPoint)) {
                    borderReachedForMappoint(mappoint);
                    continue;
                }
                if (isWall(newPoint))
                    continue;
                if (hasMappoint(newPoint)) {
                    continue;
                }
                Mappoint newMappoint = { newPoint, mappoint, false, false };
                *mappointPos++ = newMappoint;
                mappointsLen++;
            }
        }
    }


    // spread reached border
    for (int i = 0; i < PATHFINDDEPTH; i++) {
        printf("i: %i\n", i);
        for (int j = i; j < mappointsLen; j++) {
            Mappoint * mappoint = &mappoints[j];
            if (mappoint->hasPathToBorder)
                continue;
            if (neighborHasReachedBorder(mappoint)) {
                borderReachedForMappoint(mappoint);
            }
        }
    }

    for (int i = 0; i < mappointsLen; i++) {
        if (!pointIsOutside(mappoints[i].p)) {
            if (!mappoints[i].hasPathToBorder && mappoints[i].previous == NULL)
                map[mappoints[i].p.vertical][mappoints[i].p.horizontal] = 'O';
            else
                map[mappoints[i].p.vertical][mappoints[i].p.horizontal] = 'M';
        }
    }

    for (int i = 0; i < mappointsLen; i++) {
        if (!mappoints[i].hasPathToBorder && mappoints[i].previous == NULL) {
            sumOfPathsToBorder++;
            printPointMap(mappoints[i].p);
        }
    }

}

void printMap() {
    for (int i = 0; i < height * 2; i++) {
        for (int j = 0; j < width * 2; j++) {
            printf("%c", map[i][j]);
        }
        puts("");
    }
}

int main() {
    FILE *input = assertOpenFile("day10/2/input.txt", "r");
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
    steps++;
    while (!comparePoints(p, s)) {
        Point newp = walk(p, getCharAt(p), previous);
        steps++;
        previous = p;
        p.isPath = true;
        *pointPos++ = p;
        pointsLen++;
        p = newp;
    }
    for (int i = 0; i < height * 2; i++)
        for (int j = 0; j < width * 2; j++)
            map[i][j] = ' ';

    // fill map with air from lines
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            if (line[i][j] == '.')
                map[i * 2][j * 2] = line[i][j];
    }


    for (int i = 0; i < pointsLen; i++) {
        Point p = points[i];
        map[p.vertical * 2][p.horizontal * 2] = getCharAt(p);
    }
    map[s.vertical* 2][s.horizontal * 2] = 'S';



    // fill patterns with X
    // -X- FX- LX-
    /*
     * |    F   7
     * X    X   X
     *
     */
    for (int i = 0; i < height * 2; i++) {
        for (int j = 0; j < width * 2; j++) {
            if (map[i][j] == 'F' || map[i][j] == 'L' || map[i][j] == '-') {
                map[i][j + 1] = 'X';
            }
            if (map[i][j] == '7' || map[i][j] == 'F' || map[i][j] == '|') {
                map[i + 1][j] = 'X';
            }
            if (map[i][j] == 'J' || map[i][j] == '7' || map[i][j] == '-') {
                map[i][j - 1] = 'X';
            }
            if (map[i][j] == 'L' || map[i][j] == 'J' || map[i][j] == '|') {
                map[i - 1][j] = 'X';
            }
        }
    }

    // replace symbols with X
    for (int i = 0; i < height * 2; i++) {
        for (int j = 0; j < width * 2; j++) {
            if (map[i][j] == 'F' || map[i][j] == 'L' || map[i][j] == '7' || map[i][j] == 'J' || map[i][j] == '-' || map[i][j] == '|' || map[i][j] == 'S') {
                map[i][j] = 'X';
            }
        }
    }

    // place all points on lines with X
    for (int i = 0; i < pointsLen; i++) {
        Point p = points[i];
        line[p.vertical][p.horizontal] = 'X';
    }

    // put all sybols from lines on map as .
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            if (line[i][j] == 'F' || line[i][j] == 'L' || line[i][j] == '7' || line[i][j] == 'J' || line[i][j] == '-' || line[i][j] == '|')
                map[i * 2][j * 2] = '.';
    }

    printMap();
    pathfindToBorder();
    printMap();

    printf("sum: %u\n", sumOfPathsToBorder);
}
