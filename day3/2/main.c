//
// Created by abbing on 04/2/2023.
//
# include "../../lib/lib.c"
#define MAXLINE 5000
#define MAXDATASETS 2000
int isSymbol(char c) {
    if (isdigit(c))
        return 0;
    if (c == '.' || c == '\n')
        return 0;
    return 1;
}
typedef struct Position {
    int x;
    int y;
} Position;
typedef struct NumberAnalysis {
    int count;
    char symbol;
    Position position;
} NA;

bool comparePosition(Position p1, Position p2) {
    if (p1.x != p2.x)
        return false;
    if (p1.y != p2.y)
        return false;
    return true;
}

void printAnalysis(NA an) {
    printf("Anysis: Count: %i, Pos: %i, %i\n", an.count, an.position.x, an.position.y);
}

NA analyseNumber(char **lines, int n, int i, int width, int height) {
    int end = i, t, j = 0;
    char count[10];
    while (end < width && isdigit(lines[n][end])) {
        count[j++] = lines[n][end++];
    }
    end--;
    count[j] = '\n';
    NA na;
    na.count = atoi(count);
    if (n>0) {
        for (t = i; t <= end; t++) {
            if (isSymbol(lines[n-1][t])) {
                na.symbol = lines[n-1][t];
                Position p = { n-1, t};
                na.position = p;
                return na;
            }
        }
        if (i > 0 && isSymbol(lines[n-1][i-1])) {
            na.symbol = lines[n-1][i-1];
            Position p = { n-1, i-1};
            na.position = p;
            return na;
        }
        if (end < width - 1 && isSymbol(lines[n-1][end + 1])) {
            na.symbol = lines[n-1][end + 1];
            Position p = { n-1, end + 1};
            na.position = p;
            return na;
        }
    }
    if (n < height - 1){
        for (t = i; t <= end; t++) {
            if (isSymbol(lines[n+1][t])) {
                na.symbol = lines[n+1][t];
                Position p = { n+1, t};
                na.position = p;
                return na;
            }
        }
        if (i > 0 && isSymbol(lines[n+1][i-1])) {
            na.symbol = lines[n+1][i-1];
            Position p = { n+1, i-1};
            na.position = p;
            return na;
        }
        if (end < width - 1 && isSymbol(lines[n+1][end + 1])) {
            na.symbol = lines[n+1][end + 1];
            Position p = { n+1, end + 1};
            na.position = p;
            return na;
        }
    }
    if (i > 0 && isSymbol(lines[n][i-1])) {
        na.symbol =lines[n][i-1];
        Position p = { n, i-1};
        na.position = p;
        return na;
    }
    if (end < width - 1 && isSymbol(lines[n][end + 1])) {
        na.symbol = lines[n][end + 1];
        Position p = { n, end + 1};
        na.position = p;
        return na;
    }
    return (NA) {0, ' ', {-1, -1}};
}

size_t analyseList(NA * list, int listc) {
    bool eleminations[listc];
    for(int i = 0; i < listc; i++) {
        eleminations[i] = false;
    }
    size_t accumulator = 0;
    for (int i = 0; i < listc; i++) {
        if (eleminations[i] == false) {
            int equalCounter = 1;
            int * equalIndexes = malloc(sizeof(int) * 10);
            for (int j = i+1; j < listc; j++) {
                if (comparePosition(list[i].position, list[j].position)) {
                    equalCounter++;
                    *equalIndexes++ = j;
                }
            }
            if (equalCounter == 2) {
                accumulator += (size_t) list[i].count * (size_t) list[*--equalIndexes].count;
            }
            else {
                while(equalCounter != 1) {
                    eleminations[*--equalIndexes] = true;
                    equalCounter--;
                }
            }
            free(equalIndexes);
        }
    }
    return accumulator;
}

NA analysisList[MAXDATASETS];
NA *analysisPos = analysisList;
int analysisCount = 0;

void walkLine(char **lines, int n, int width, int height) {
    int i = 0;
    while(i < width) {
        if (!isdigit(lines[n][i])) {
            i++;
        }
        else {
            //printf("Added Number: %i\n", analyseNumber(lines, n, i, width, height));
            NA analysis = analyseNumber(lines, n, i, width, height);
            if (analysis.symbol == '*') {
                *analysisPos++ = analysis;
                analysisCount++;
            }
            while(i < width && isdigit(lines[n][i]))
                i++;
        }
    }
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day3/2/input.txt", "r");
    if (input == NULL) {
        fputs("Could not read file ./input.txt", stderr);
        exit(1);
    }
    char**lines = (char **)malloc(sizeof(char*) * MAXDATASETS);
    char line[MAXLINE];
    fgets(line, MAXLINE, input);
    // len of one array with \n
    const int width = strlen(line);
    int i = 0;
    lines[i] = (char *)malloc(sizeof(char) * width);
    strcpy(lines[i++], line);
    for(; fgets(line, MAXLINE, input) != NULL; i++) {
        lines[i] = (char *)malloc(sizeof(char) * width);
        strcpy(lines[i], line);
    }
    // number of arrays created
    const int height = i;
    for (i = 0; i < height; i++) {
        walkLine(lines, i, width, height);
    }
    printf("LSG: %zu\n", analyseList(analysisList, analysisCount));
}
