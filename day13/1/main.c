// https://github.com/LReg/clib
#include "../../../clib/lib.c"

char picture[100][50][50];
unsigned int lim[100][2];

typedef struct ReflectionAnalysis {
    bool horizontal;
    bool vertical;
    int arrayIndexHorizontal;
    int arrayIndexVertical;
    int pictureIndex;
} RA;

void printReflectionAnalysis(RA ra) {
    printf("Picture %d:\n", ra.pictureIndex);
    printf("Horizontal: %s\n", ra.horizontal ? "true" : "false");
    printf("Vertical: %s\n", ra.vertical ? "true" : "false");
    printf("ArrayIndexHorizontal: %i\n", ra.arrayIndexHorizontal);
    printf("ArrayIndexVertical: %i\n", ra.arrayIndexVertical);
}

bool compareVertical(unsigned int pindex, unsigned int ai1, unsigned int ai2) {
    for (int i = 0; i < lim[pindex][0]; i++)
        if (picture[pindex][i][ai1] != picture[pindex][i][ai2])
            return false;
    return true;
}

bool compareHorizontal(unsigned int pindex, unsigned int ai1, unsigned int ai2) {
    for (int i = 0; i < lim[pindex][1]; i++)
        if (picture[pindex][ai1][i] != picture[pindex][ai2][i])
            return false;
    return true;
}

RA analyse(int index) {
    RA ra;
    ra.pictureIndex = index;
    ra.horizontal = false;
    ra.vertical = false;
    ra.arrayIndexHorizontal = -1;
    ra.arrayIndexVertical = -1;
    // vertical Analysis
    for (int i = 0; i < lim[index][1] - 1; i++) {
        if (compareVertical(index, i, i + 1)) {
            for (int j = i + 1, k = i; j < lim[index][1] + 1; j++, k--) {
                if (k < 0 || j >= lim[index][1]) {
                    if (ra.vertical)
                        puts("problem vertical");
                    ra.vertical = true;
                    ra.arrayIndexVertical = i;
                    break;
                }
                if (!compareVertical(index, j, k)) {
                    break;
                }
            }
        }
    }
    // horizontal Analysis
    for (int i = 0; i < lim[index][0] - 1; i++) {
        if (compareHorizontal(index, i, i + 1)) {
            for (int j = i + 1, k = i; j < lim[index][0] + 1; j++, k--) {
                if (k < 0 || j >= lim[index][0]) {
                    if (ra.horizontal)
                        puts("problem horizontal");
                    ra.horizontal = true;
                    ra.arrayIndexHorizontal = i;
                    break;
                }
                if (!compareHorizontal(index, j, k)) {
                    break;
                }
            }
        }
    }
    return ra;
}

int main(int argc, char *argv[]) {
    char *path = "input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char lineBuffer[50];
    char * linePos = lineBuffer;
    int pictureCount = 0;
    int pictureLineCount = 0;
    while(fgets(lineBuffer, 100, input) != NULL) {
        if (lineBuffer[0] == '\n') {
            lim[pictureCount][0] = pictureLineCount;
            lim[pictureCount][1] = strlen(picture[pictureCount][0]) - 1;
            pictureCount++;
            pictureLineCount = 0;
            continue;
        }
        strcpy(picture[pictureCount][pictureLineCount++], lineBuffer);
    }
    size_t sum = 0;
    for (int i = 0; i < pictureCount; i++) {
        RA ra = analyse(i);
        if (ra.vertical) {
            sum += ra.arrayIndexVertical + 1;
        }
        if (ra.horizontal) {
            sum += (ra.arrayIndexHorizontal + 1) * 100;
        }
        //printReflectionAnalysis(analyse(i));
        //for (int j = 0; j < lim[i][0]; j++) {
            //for (int k = 0; k < lim[i][1]; k++)
                //printf("%c", picture[i][j][k]);
            //printf("\n");
        //}
    }
    printf("Sum: %zu\n", sum);
}