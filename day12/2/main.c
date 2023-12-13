// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#include <math.h>

unsigned int orders[2000][200];
char springs[2000][200];
unsigned int lim[2000][2];
unsigned int springCount = 0;
size_t arrangementSum = 0;
size_t arrangements = 0;
char ** arrangementsArray;

int binomialCoefficient(int n, int k) {
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
    for (int j = 0; j < lim[i][1]; j++) {
        unsigned int waitingFor = orders[i][j];
        unsigned int found = 0;
        while(*arrayPos == '.')
            arrayPos++;
        while(*arrayPos++ == '#')
            found++;
        if (found != waitingFor)
            return false;
    }
    return true;
}

void fillArrayWithHashes(char array[], int arr_length, int num_hashes, int index, int j) {
    if (num_hashes == 0) {
        arrangementsArray[arrangements] = malloc(sizeof(char) * lim[j][0]);
        for (int k = 0, skipped = 0; k < lim[j][0]; k++) {
            if (springs[j][k] == '#' || springs[j][k] == '.') {
                arrangementsArray[arrangements][k] = springs[j][k];
                skipped++;
                continue;
            }
            arrangementsArray[arrangements][k] = array[k - skipped];
        }
        if (validateArray(arrangementsArray[arrangements], lim[j][0], j)) {
            arrangements++;
        }
        return;
    }

    int i;
    for (i = index; i < arr_length; i++) {
        array[i] = '#';
        fillArrayWithHashes(array, arr_length, num_hashes - 1, i + 1, j);
        array[i] = '.';
    }
}

unsigned int calculateArrangements(unsigned int i) {
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
    arrangementsArray = malloc(sizeof(char *) * binomialCoefficient(numberOfNotSetSprings, numberOfSetabbleSprings));
    fillArrayWithHashes(arrangement, numberOfNotSetSprings, numberOfSetabbleSprings, 0, i);
    // generate all possible arrangements
    //printf("number: %d done %zu\n", i, arrangements);
    return arrangements;
}

int main(int argc, char *argv[]) {
    char *path = "day12/2/input.txt";
    if (1 || argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day12/2/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char lineBuffer[100];
    char * linePos = lineBuffer;
    while(fgets(lineBuffer, 100, input) != NULL) {
        char * middle = strchr(lineBuffer, ' ');
        int i;
        for(i = 0; linePos != middle; i++) {
            springs[springCount][i] = *linePos++;
        }
        lim[springCount][0] = i;
        // previous stops with #
        bool previousWithHash = false;
        bool endsWithQuestionMark = false;
        if (springs[springCount][i - 1] == '?') {
            endsWithQuestionMark = true;
        }
        if (springs[springCount][i - 1] == '#') {
            previousWithHash = true;
            springs[springCount + 1][0] = '#';
            springs[springCount + 1][1] = '?';
            i = 2;
        }
        else {
            springs[springCount + 1][0] = '?';
            i = 1;
        }
        linePos = lineBuffer;
        for(; linePos != middle; i++) {
            springs[springCount + 1][i] = *linePos++;
        }
        if (endsWithQuestionMark)
            springs[springCount + 1][i++] = '?';
        lim[springCount + 1][0] = i;
        ParsedInts ints = parseInts(middle, 30);
        for (i = 0; i < ints.len; i++) {
            orders[springCount][i] = ints.ints[i];
        }
        lim[springCount][1] = ints.len;
        int offset = 0;
        if (previousWithHash) {
            orders[springCount + 1][0] = 1;
            offset = 1;
        }
        for (i = 0 + offset; i < ints.len + offset; i++) {
            orders[springCount + 1][i] = ints.ints[i - offset];
        }
        lim[springCount + 1][1] = ints.len + offset;
        free(ints.ints);
        linePos = lineBuffer;
        springCount += 2;
    }
    for (int i = 0; i < springCount; i++) {
        printArray(springs[i], lim[i][0]);
        for (int j = 0; j < lim[i][1]; j++) {
            printf("%d ", orders[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < springCount; i += 2) {
        if (i == 1242) {
            printf("springs: %s\n", springs[i]);
        }
        int normalArrangement= calculateArrangements(i);
        int increadedArrangement = calculateArrangements(i + 1);
        size_t calc = normalArrangement * ((long long int )(pow(increadedArrangement, 4)));
        //printf("calc: %zu\n", calc);
        arrangementSum += calc;
    }
    printf("arrangements: %zu", arrangementSum);

}