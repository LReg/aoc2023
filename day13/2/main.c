// https://github.com/LReg/clib
#include "../../../clib/lib.c"

char picture[100][50][50];
unsigned int lim[100][2];

typedef enum ReflectionType {
   HORIONTAL,
   VERTICAL,
   NONE
}ReflectionType;

typedef struct Reflection{
    ReflectionType reflectionType;
    int index;
} Reflection;

typedef struct ReflectionAnalysis {
    Reflection reflections[5];
    int reflectionCount;
} RA;

bool compareRA(RA ra1, RA ra2) {
    if (ra1.reflectionCount != ra2.reflectionCount)
        return false;
    for (int i = 0; i < ra1.reflectionCount; i++) {
        if (ra1.reflections[i].reflectionType != ra2.reflections[i].reflectionType)
            return false;
        if (ra1.reflections[i].index != ra2.reflections[i].index)
            return false;
    }
    return true;
}

void printReflectionAnalysis(RA ra) {
    for (int i = 0; i < 5; i++) {
        if (ra.reflections[i].reflectionType == NONE)
            continue;
        printf("Reflection %d:\n", i);
        printf("ReflectionType: %s\n", ra.reflections[i].reflectionType == HORIONTAL ? "horizontal" : "vertical");
        printf("Index: %i\n", ra.reflections[i].index);
    }
}

void printReflection(Reflection r) {
    printf("ReflectionType: %s\n", r.reflectionType == HORIONTAL ? "horizontal" : "vertical");
    printf("Index: %i\n", r.index);
}

bool compareReflection(Reflection r1, Reflection r2) {
    return r1.reflectionType == r2.reflectionType && r1.index == r2.index;
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
    ra.reflectionCount = 0;
    for (int i = 0; i < 5; i++) {
        ra.reflections[i].reflectionType = NONE;
        ra.reflections[i].index = -1;
    }

    // vertical Analysis
    for (int i = 0; i < lim[index][1] - 1; i++) {
        if (compareVertical(index, i, i + 1)) {
            for (int j = i + 1, k = i; j < lim[index][1] + 1; j++, k--) {
                if (k < 0 || j >= lim[index][1]) {
                    ra.reflections[ra.reflectionCount].reflectionType = VERTICAL;
                    ra.reflections[ra.reflectionCount].index = i;
                    ra.reflectionCount++;
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
                    ra.reflections[ra.reflectionCount].reflectionType = HORIONTAL;
                    ra.reflections[ra.reflectionCount].index = i;
                    ra.reflectionCount++;
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

RA fixSmudge(RA before, int index) {
    for (int i = 0; i < lim[index][0]; i++) {
        for (int j = 0; j < lim[index][1]; j++) {
           picture[index][i][j] = picture[index][i][j] == '#' ? '.' : '#';
           RA analysis = analyse(index);
           if (analysis.reflectionCount != 0 && !compareRA(before, analysis)) {
               return analysis;
           }
            picture[index][i][j] = picture[index][i][j] == '#' ? '.' : '#';
        }
    }
    assert(false);
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
        RA before = analyse(i);
        RA ra = fixSmudge(before, i);
        Reflection newReflection;
        newReflection.reflectionType = NONE;
        for (int j = 0; j < ra.reflectionCount; j++) {
            for (int k = 0; k < before.reflectionCount; k++) {
                if (compareReflection(ra.reflections[j], before.reflections[k]))
                    continue;
                newReflection = ra.reflections[j];
                break;
            }
            if (newReflection.reflectionType != NONE)
                break;
        }
        printReflection(newReflection);
        if (newReflection.reflectionType == NONE) {
            assert(false);
        }
        if (newReflection.reflectionType == VERTICAL) {
            sum += newReflection.index + 1;
        }
        else if (newReflection.reflectionType == HORIONTAL) {
            sum += (newReflection.index + 1) * 100;
        }
        else {
            assert(false);
        }
        //for (int j = 0; j < lim[i][0]; j++) {
            //for (int k = 0; k < lim[i][1]; k++)
                //printf("%c", picture[i][j][k]);
            //printf("\n");
        //}
    }
    printf("Sum: %zu\n", sum);
}