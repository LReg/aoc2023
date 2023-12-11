// https://github.com/LReg/clib
#include "../../../clib/lib.c"

char ** expanded;
unsigned int ewidth, eheight;

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
    unsigned int route;
} GalaxyRoute;

GalaxyRoute * galaxyRoutes;
unsigned int galaxyRouteCount;

void expand(char lines[150][150], unsigned int height, unsigned int width) {
    bool verticalExpansions[width];
    for (int i = 0; i < width; i++)
        verticalExpansions[i] = true;
    unsigned int newWidth = width;
    unsigned int newHeight = 0;
    char ** newLines = malloc(sizeof(char **) * height * 2);
    for (int i = 0; i < height; i++) {
        newLines[newHeight] = malloc(sizeof(char) * width * 2);
        bool linesHadGalaxies = false;
        for (int j = 0; j < width; j++) {
            newLines[newHeight][j] = lines[i][j];
            if (lines[i][j] == '#') {
                linesHadGalaxies = true;
                verticalExpansions[j] = false;
            }
        }
        newHeight++;
        if (!linesHadGalaxies) {
            newLines[newHeight] = malloc(sizeof(char) * width * 2);
            for (int j = 0; j < width; j++) {
                newLines[newHeight][j] = '.';
            }
            newHeight++;
        }
    }
    unsigned int expansions = 0;
    for (int i = 0; i < newWidth; i++) {
        if (verticalExpansions[i - expansions]) {
            for (int j = 0; j < newHeight; j++) {
                char rest[newWidth - i];
                for (int k = i; k < newWidth; k++) {
                    rest[k - i] = newLines[j][k];
                }
                newLines[j][i] = '.';
                for (int k = i + 1; k < newWidth + 1; k++) {
                    newLines[j][k] = rest[k - i - 1];
                }
            }
            expansions++;
            newWidth++;
            i++;
        }
    }
    ewidth = newWidth;
    eheight = newHeight;
    expanded = newLines;
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

unsigned int calculateRouteFromTo(Galaxy * g1, Galaxy * g2) {
    unsigned int route = 0;
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
        route++;
    }
    return route;
}

void calculateAllRoutes() {
    galaxyRoutes = malloc(sizeof(GalaxyRoute) * galaxyCount * galaxyCount);
    for (unsigned int i = 0; i < galaxyCount; i++) {
        for (unsigned int j = i; j < galaxyCount; j++) {
            unsigned int route = calculateRouteFromTo(&galaxies[i], &galaxies[j]);
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
            printf("%c", expanded[i][j]);
        }
        puts("");
    }
}

int main(int argc, char *argv[]) {
    char *path = "day11/1/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day11/1/input.test.txt";
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
}

