// https://github.com/LReg/clib
#include "../../clib/lib.c"

typedef struct Glass {
    short int vocalLength;
    char label[20];
    int lim;
} Glass;

Glass * boxes[256][8];

int val(char * word, int lim) {
   /*
    Determine the ASCII code for the current character of the string.
    Increase the current value by the ASCII code you just determined.
    Set the current value to itself multiplied by 17.
    Set the current value to the remainder of dividing itself by 256.
    */
    long int current = 0;
    for (int i = 0; i < lim; i++) {
        current += word[i];
        current *= 17;
        current %= 256;
    }
    return current;
}

void printBoxes() {
    for (int i = 0; i < 5; i++) {
        if (boxes[i][0] != NULL)
            printf("Box %d:[ ", i);
            for (int j = 0; j < 8; j++) {
                if (boxes[i][j] != NULL)
                    printf("%s, ", boxes[i][j]->label);
            }
            printf("]");
        }

}

void placeInBox(Glass * glass, bool add) {
    int position = val(glass->label, glass->lim);
    int isInBox = -1;
    for (int i = 0; i < 8; i++)
        if (boxes[position][i] != NULL && strcmp(boxes[position][i]->label, glass->label) == 0)
            isInBox = i;
    if (add) {
        if (isInBox != -1) {
            boxes[position][isInBox] = glass;
            return;
        }
        for (int i = 0; i < 8; i++) {
            if (boxes[position][i] == NULL) {
                boxes[position][i] = glass;
                return;
            }
        }
        assert(false);
    }
    else {
        if (isInBox != -1) {
            boxes[position][isInBox] = NULL;
            for (int j = isInBox + 1; j < 8; j++)
                boxes[position][j - 1] = boxes[position][j];
        }
    }
}

void clearArray(char * arr, size_t len) {
    for (int i = 0; i < len; i++)
        arr[i] = '.';
}

int main(int argc, char *argv[]) {
    char *path = "day15/input.txt";
    if (argc > 1 && strcmp(argv[1], "test") == 0)
        path = "day15/input.test.txt";
    FILE *input = assertOpenFile(path, "r");
    char wordBuffer[20];
    clearArray(wordBuffer, 20);
    int charCount = 0;
    long int sum = 0;
    while((wordBuffer[charCount] = (char)fgetc(input)) != EOF) {
        if (wordBuffer[charCount] == ',' || wordBuffer[charCount] == '\n') {
            char * equalPos = strchr(wordBuffer, '=');
            char * minusPos = strchr(wordBuffer, '-');
            if (equalPos != NULL) {
                *equalPos = '\0';
                char * label = wordBuffer;
                char * vocalLength = equalPos + 1;
                Glass * glass = malloc(sizeof(Glass));
                strcpy(glass->label, label);
                glass->vocalLength = atoi(vocalLength);
                glass->lim = strlen(glass->label);
                placeInBox(glass, true);
            } else if (minusPos != NULL) {
                *minusPos = '\0';
                char * label = wordBuffer;
                Glass * glass = malloc(sizeof(Glass));
                strcpy(glass->label, label);
                glass->lim = strlen(glass->label);
                placeInBox(glass, false);
            } else {
                assert(false);
            }
            clearArray(wordBuffer, 20);
            charCount = 0;
            continue;
        }
        charCount++;
    }
    /*
    One plus the box number of the lens in question.
    The slot number of the lens within the box: 1 for the first lens, 2 for the second lens, and so on.
    The focal length of the lens.
     */
    printBoxes();
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < 8; j++)
            if (boxes[i][j] != NULL)
                sum += (i + 1) * (j + 1) * boxes[i][j]->vocalLength;
    printf("Sum: %ld\n", sum);
}