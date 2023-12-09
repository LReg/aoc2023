#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 5000
#define MAXDATASETS 2000
int isSymbol(char c) {
    if (isdigit(c))
        return 0;
    if (c == '.' || c == '\n')
        return 0;
    return 1;
}

int analyseNumber(char **lines, int n, int i, int width, int height) {
    int end = i, t, j = 0;
    char count[10];
    while (end < width && isdigit(lines[n][end])) {
        count[j++] = lines[n][end++];
    }
    end--;
    count[j] = '\n';
    int realNum = atoi(count);
    if (n>0) {
        for (t = i; t <= end; t++) {
            if (isSymbol(lines[n-1][t])) {
                return realNum;
            }
        }
        if (i > 0 && isSymbol(lines[n-1][i-1])) {
            return realNum;
        }
        if (end < width - 1 && isSymbol(lines[n-1][end + 1])) {
            return realNum;
        }
    }
    if (n < height - 1){
        for (t = i; t <= end; t++) {
            if (isSymbol(lines[n+1][t])) {
                return realNum;
            }
        }
        if (i > 0 && isSymbol(lines[n+1][i-1])) {
            return realNum;
        }
        if (end < width - 1 && isSymbol(lines[n+1][end + 1])) {
            return realNum;
        }
    }
    if (i > 0 && isSymbol(lines[n][i-1])) {
        return realNum;
    }
    if (end < width - 1 && isSymbol(lines[n][end + 1])) {
        return realNum;
    }
    return 0;
}



/* return sum of line */
int walkLine(char **lines, int n, int width, int height) {
    int i = 0, sum = 0;
    while(i < width) {
        if (!isdigit(lines[n][i])) {
            i++;
        }
        else {
            //printf("Added Number: %i\n", analyseNumber(lines, n, i, width, height));
            sum += analyseNumber(lines, n, i, width, height);
            while(i < width && isdigit(lines[n][i]))
                i++;
        }
    }
    return sum;
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day3/1/input.txt", "r");
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
    long int sum = 0;
    for (i = 0; i < height; i++) {
        sum += walkLine(lines, i, width, height);
    }
    printf("%li", sum);
    for (i = 0; i < height; i++) {
        free(lines[i]);
    }
    free(lines);
    fclose(input);
}
