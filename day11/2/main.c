// https://github.com/LReg/clib
#include "../../../clib/lib.c"

char ** expanded;
unsigned int ewidth, eheight;
bool expandedLine[150];
bool expandedColumn[150];

typedef struct Point {
    unsigned int horizontal;
    unsigned int vertical;
} Point;

int comparePoints(Point a, Point b) {
    return a.horizontal == b.horizontal && a.vertical == b.vertical;
}

typedef struct Galaxy {
    Point p;
    unsigned int id;
} Galaxy;

Galaxy * galaxies;
unsigned int galaxyCount;

typedef struct GalaxyRoute {
    Galaxy * from;
    Galaxy * to;
    size_t route;
} GalaxyRoute;

GalaxyRoute * galaxyRoutes;
unsigned int galaxyRouteCount;

bool pointIsOnExpansion(Point p) {
    bool isOnExpanded = false;
    if (expandedColumn[p.horizontal] || expandedLine[p.vertical])
        isOnExpanded = true;
    return isOnExpanded;
}

void expand(char lines[150][150], unsigned int height, unsigned int width) {
    for (int i = 0; i < height; i++) {
        expandedLine[i] = true;
        for (int j = 0; j < width; j++) {
            if (lines[i][j] == '#')
                expandedLine[i] = false;
        }
    }
    for (int i = 0; i < width; i++) {
        expandedColumn[i] = true;
        for (int j = 0; j < height; j++) {
            if (lines[j][i] == '#')
                expandedColumn[i] = false;
        }
    }
    // copy to expanded
    eheight = height;
    ewidth = width;
    expanded = malloc(sizeof(char *) * eheight);
    for (int i = 0; i < eheight; i++) {
        expanded[i] = malloc(sizeof(char) * ewidth);
        for (int j = 0; j < ewidth; j++) {
            expanded[i][j] = lines[i][j];
        }
    }
}

void storeGalaxies() {
    unsigned int number = 0;
    // count galaxies
    for (int i = 0; i < eheight; i++) {
        for (int j = 0; j < ewidth; j++) {
            if (expanded[i][j] == '#')
                number++;
        }
    }
    galaxies = malloc(sizeof(Galaxy) * number);
    for (int i = 0; i < eheight; i++) {
        for (int j = 0; j < ewidth; j++) {
            if (expanded[i][j] == '#') {
                Galaxy g = { { j, i }, galaxyCount };
                galaxies[galaxyCount++] = g;
            }
        }
    }
}

size_t calculateRouteFromTo(Galaxy * g1, Galaxy * g2) {
    size_t route = 0;
    bool isOnExpandedLine = false;
    Point p = g1->p;
    while(!comparePoints(p, g2->p)) {
        if (p.horizontal < g2->p.horizontal)
            p.horizontal++;
        else if (p.horizontal > g2->p.horizontal)
            p.horizontal--;
        else if (p.vertical < g2->p.vertical)
            p.vertical++;
        else if (p.vertical > g2->p.vertical)
            p.vertical--;
        if (pointIsOnExpansion(p) && !isOnExpandedLine) {
            isOnExpandedLine = true;
        }
        if (!pointIsOnExpansion(p) && isOnExpandedLine) {
            isOnExpandedLine = false;
            route += 1000000;
            continue;
        }
        route++;
    }
    return route;
}

void calculateAllRoutes() {
    galaxyRoutes = malloc(sizeof(GalaxyRoute) * galaxyCount * galaxyCount);
    for (unsigned int i = 0; i < galaxyCount; i++) {
        for (unsigned int j = i; j < galaxyCount; j++) {
            size_t route = calculateRouteFromTo(&galaxies[i], &galaxies[j]);
            GalaxyRoute gr = { &galaxies[i], &galaxies[j], route };
            galaxyRoutes[galaxyRouteCount++] = gr;
            //printf("Route from %i to %i: %i\n", i, j, route);
        }
    }
}

size_t calculateSum() {
    size_t sum = 0;
    for (unsigned int i = 0; i < galaxyRouteCount; i++) {
        sum += galaxyRoutes[i].route;
    }
    return sum;
}

void printExpanded() {
    for (int i = 0; i < eheight; i++) {
        for (int j = 0; j < ewidth; j++) {
            if (pointIsOnExpansion((Point) { j, i }))
                printf("O");
            else
                printf("%c", expanded[i][j]);
        }
        puts("");
    }
}

int main(int argc, char *argv[]) {
    char *path = "day11/2/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day11/2/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char lines[150][150];
    unsigned int height = 0;
    unsigned int width = 0;
    while(fgets(lines[height], 150, input) != NULL){
        height++;
    }
    width = strlen(lines[height - 1]) - 1;
    expand(lines, height, width);
    storeGalaxies();
    printExpanded();
    calculateAllRoutes();
    printf("Sum of legths: %zu\n", calculateSum());
    /* Test tests
     * printf("isExpanded: %i\n", pointIsOnExpansion((Point) { 0, 0 }));
    printf("isExpanded: %i\n", pointIsOnExpansion((Point) { 2, 0 }));
    printf("isExpanded: %i\n", pointIsOnExpansion((Point) { 0, 3 }));
    printf("isExpanded: %i\n", pointIsOnExpansion((Point) { 0, 4 }));
    */
}

