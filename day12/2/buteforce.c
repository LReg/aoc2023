// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#include <time.h>

unsigned int orders[1000][300];
char springs[1000][300];
unsigned int lim[1000][2];
unsigned int positionMapping[1000][300];
unsigned int springCount = 0;
size_t arrangementSum = 0;
size_t arrangements = 0;

size_t binomialCoefficient(size_t n, size_t k) {
    if (k == 0 || k == n) {
        return 1;
    } else {
        // Verwende die rekursive Formel: C(n, k) = C(n-1, k-1) + C(n-1, k)
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }
}

void printArray(char * array, unsigned int size) {
    for (int i = 0; i < size; i++) {
        printf("%c", array[i]);
    }
    printf("\n");
}

bool validateArray(char * array, unsigned int size, int i) {
    char * arrayPos = array;
    int position = 0;
    for (int j = 0; j < lim[i][1]; j++) {
        unsigned int waitingFor = orders[i][j];
        unsigned int found = 0;
        while(*arrayPos == '.') {
            arrayPos++;
            position++;
        }
        while(*arrayPos++ == '#') {
            found++;
            position++;
            if (found > waitingFor) {
                return false;
            }
        }
        if (found != waitingFor) {
            return false;
        }
    }
    return true;
}

bool earlyValidateArray(char * array, unsigned int size, int i) {
    int position = 0;

    char arrangementArray[lim[i][0]];
    for (int k = 0, skipped = 0; k < lim[i][0]; k++) {
        if (springs[i][k] == '#' || springs[i][k] == '.') {
            arrangementArray[k] = springs[i][k];
            skipped++;
            continue;
        }
        arrangementArray[k] = array[k - skipped];
    }

    char * arrayPos = arrangementArray;
    for (int j = 0; j < lim[i][1]; j++) {
        unsigned int waitingFor = orders[i][j];
        unsigned int found = 0;
        while(*arrayPos == '.') {
            if (position == size) {
                return true;
            }
            arrayPos++;
            position++;
        }
        while(*arrayPos == '#') {
            if (position == size) {
                return true;
            }
            found++;
            if (found > waitingFor) {
                //printf("found: %d waitingFor: %d, order: %i\n", found, waitingFor, j);
                //printArray(arrangementArray, lim[i][0]);
                //printf("position: %i size: %u\n", position, size);
                return false;
            }
            arrayPos++;
            position++;
        }
        if (position == size) {
            return true;
        }
        if (found < waitingFor && *arrayPos == '.') {
            return false;
        }
    }
    return true;
}

void fillArrayWithHashes(char array[], int arr_length, int num_hashes, int index, int j) {
    if (num_hashes == 0) {
        char * arrangementsArray = malloc(sizeof(char) * lim[j][0]);
        for (int k = 0, skipped = 0; k < lim[j][0]; k++) {
            if (springs[j][k] == '#' || springs[j][k] == '.') {
                arrangementsArray[k] = springs[j][k];
                skipped++;
                continue;
            }
            arrangementsArray[k] = array[k - skipped];
        }
        if (validateArray(arrangementsArray, lim[j][0], j)) {
            free(arrangementsArray);
            arrangements++;
        }
        else {
            free(arrangementsArray);
        }
        return;
    }
    int i;
    for (i = index; i < arr_length; i++) {
        array[i] = '#';
        if (i % 2 == 0 && earlyValidateArray(array, i, j) == false) {
            return;
        }
        fillArrayWithHashes(array, arr_length, num_hashes - 1, i + 1, j);
        array[i] = '.';
    }
}

unsigned int calculateArrangements(unsigned int i) {
    clock_t start = clock();
    arrangements = 0;
    unsigned int * o = orders[i];
    char * s = springs[i];
    int numberOfSprings = 0;
    int numberOfNotSetSprings = 0;
    for (int j = 0; j < lim[i][0]; j++) {
        if (s[j] == '#')
            numberOfSprings++;
        else if (s[j] == '?')
            numberOfNotSetSprings++;
    }
    int numberOfSetabbleSprings = 0;
    for (int j = 0; j < lim[i][1]; j++) {
        numberOfSetabbleSprings += (int) o[j];
    }
    numberOfSetabbleSprings -= numberOfSprings;
    char arrangement[numberOfNotSetSprings];
    for (int j = 0; j < numberOfNotSetSprings; j++) {
        arrangement[j] = '.';
    }
    for (int j = 0; j < lim[i][0]; j++) {
        if (springs[i][j] == '.' || springs[i][j] == '#')
            continue;
        else
            positionMapping[i][j] = j;
    }
    fillArrayWithHashes(arrangement, numberOfNotSetSprings, numberOfSetabbleSprings, 0, i);
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("number: %d done %zu, in %f seconds\n", i, arrangements, seconds);
    return arrangements;
}

int main(int argc, char *argv[]) {
    clock_t start = clock();
    char *path = "day12/2/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day12/2/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char lineBuffer[200];
    char * linePos = lineBuffer;
    while(fgets(lineBuffer, 200, input) != NULL) {
        char * middle = strchr(lineBuffer, ' ');
        int i;
        lim[springCount][0] = 0;
        for (int k = 0; k < 5; k++) {
            for(i = 0; linePos != middle; i++) {
                springs[springCount][i + lim[springCount][0]] = *linePos++;
            }
            springs[springCount][lim[springCount][0] + i++] = '?';
            linePos = lineBuffer;
            lim[springCount][0] += i;
        }
        // delete last ?
        springs[springCount][--lim[springCount][0]] = 'X';
        ParsedInts ints = parseInts(middle, 20);
        lim[springCount][1] = 0;
        for (int k =0; k < 5; k++) {
            for (i = 0; i < ints.len; i++) {
                orders[springCount][lim[springCount][1] + i] = ints.ints[i];
            }

            lim[springCount][1] += i;
        }
        free(ints.ints);
        linePos = lineBuffer;
        springCount++;
    }
    for (int i = 0; i < springCount; i++) {
        arrangementSum += calculateArrangements(i);
    }
    clock_t end = clock();
    float seconds = (float)(end - start) / CLOCKS_PER_SEC;
    printf("arrangements: %zu, in %f seconds", arrangementSum, seconds);

}