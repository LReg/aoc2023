// https://github.com/LReg/clib
#include "../../clib/lib.c"
#define DEPTH 3000000
char grid[100][101];
int width = 0, height = 0;
size_t spinCycles = 1000000000;

void printGrid(char grid[][101]) {
    for (int i = 0; i < height; i++)
        printf("%s\n", grid[i]);
}

void tiltSouth() {
    for (int i = 0; i < width; i++) {
        for (int j = height - 2; j >= 0; j--) {
            if (grid[j][i] == 'O') {
                for (int k = j + 1; k < height; k++) {
                    if (grid[k][i] == '#' || grid[k][i] == 'O') {
                        if (k - 1 == j) {
                            break;
                        }
                        grid[k - 1][i] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                    if (k == height - 1) {
                        grid[k][i] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                }
            }
        }
    }
}

void tiltEast() {
    for (int j = 0; j < height; j++) {
        for (int i = width - 2; i >= 0; i--) {
            if (grid[j][i] == 'O') {
                for (int k = i + 1; k < width; k++) {
                    if (grid[j][k] == '#' || grid[j][k] == 'O') {
                        if (k - 1 == i) {
                            break;
                        }
                        grid[j][k - 1] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                    if (k == width - 1) {
                        grid[j][k] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                }
            }
        }
    }
}

void tilteWest() {
    for (int j = 0; j < height; j++) {
        for (int i = 1; i < width; i++) {
            if (grid[j][i] == 'O') {
                for (int k = i - 1; k >= 0; k--) {
                    if (grid[j][k] == '#' || grid[j][k] == 'O') {
                        if (k + 1 == i) {
                            break;
                        }
                        grid[j][k + 1] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                    if (k == 0) {
                        grid[j][k] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                }
            }
        }
    }
}



void tiltNorth() {
    for (int i = 0; i < width; i++) {
        for (int j = 1; j < height; j++) {
            if (grid[j][i] == 'O') {
                for (int k = j - 1; k >= 0; k--) {
                    if (grid[k][i] == '#' || grid[k][i] == 'O') {
                        if (k + 1 == j) {
                            break;
                        }
                        grid[k + 1][i] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                    if (k == 0) {
                        grid[k][i] = 'O';
                        grid[j][i] = '.';
                        break;
                    }
                }
            }
        }
    }
}

void tiltCycle() {
    tiltNorth();
    tilteWest();
    tiltSouth();
    tiltEast();
}

size_t calculateWeight(char grid[][101]) {
    size_t weight = 0;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (grid[i][j] == 'O')
                weight += height - i;
    return weight;
}

bool compareGrids(char grid1[][101], char grid2[][101]) {
    for (int i = 0; i < height; i++)
        if (strcmp(grid1[i], grid2[i]) != 0)
            return false;
    return true;
}

int main(int argc, char *argv[]) {
    char *path = "day14/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day14/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char linebuffer[200];
    while(fgets(linebuffer, 200, input) != NULL) {
        strncpy(grid[height], linebuffer, strlen(linebuffer) - 1);
        grid[height][strlen(linebuffer) - 1] = '\0'; // replace \n with \0
        height++;
    }
    width = (int)strlen(grid[0]);
    unsigned int *WeightHistory = malloc(sizeof(unsigned int) * DEPTH);
    size_t weightHistoryIndex = 0;
    for (size_t i = 0; i < spinCycles; i++) {
        if (i % 1000 == 0) {
            printf("percent: %f\n", (float)i / (float)spinCycles * 100.0f);
        }
        tiltCycle();
        if (i!= 0) {
            for (size_t j = 0; j < weightHistoryIndex; j++) {
                if (WeightHistory[j] == calculateWeight(grid)) {
                    printf("cycle at : %zu\n", i);
                    printf("prev: %zu\n", j);
                    printf("weight: %zu\n", calculateWeight(grid));
                    break;
                }
            }
        }
        WeightHistory[weightHistoryIndex++] = calculateWeight(grid);
    }
    printf("weight: %zu\n", calculateWeight(grid));
}