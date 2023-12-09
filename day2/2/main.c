#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 2000
#define MAXDATASETS 15
struct ColorCounts {
    int red;
    int green;
    int blue;
    int gameNr;
};

struct ElementFound {
    char color;
    char *next;
    int number;
};

int charsUntilNext(const char * input, char c, int lim) {
    int n = 0, ch;
    while((ch = input[n++]) != '\n' && ch != c) {
        if (c == lim)
            return -1;
    }
    if (ch == '\n')
        return -1;
    return n;
}

struct ElementFound findNext(char *input) {
    //good debug output
    // TODO: I dont know why yet but if you comment this input out, it does not work
    //printf("INPUT:%s", input);
    struct ElementFound ef;
    int pos = 0, countLen = 0;
    while(isdigit(input[pos++])) {
        countLen++;
    }
    char count[countLen + 1];
    strncpy(count, input, countLen);
    count[countLen] = '\n';
    ef.number = atoi(count);
    char c;
    while((c = input[pos++]) != 'g' && c != 'r' && c != 'b') {
    }
    ef.color = c;
    int nextSem = charsUntilNext(&input[pos], ';', MAXLINE);
    int nextCom = charsUntilNext(&input[pos], ',', MAXLINE);
    if (nextCom == -1 && nextSem == -1)
        ef.next = NULL;
    else if (nextCom == -1)
        ef.next = strchr(&input[pos], ';')+ 2;
    else if (nextSem == -1)
        ef.next = strchr(&input[pos], ',') + 2;
    else {
        ef.next = (nextCom < nextSem ? strchr(&input[pos], ',') + 2 : strchr(&input[pos], ';') + 2);
    }
    return ef;
}

struct ColorCounts readLine(char * input) {
    struct ColorCounts colorCounts = { 0,0,0};
    int scanRet = sscanf(input, "Game%i:", &colorCounts.gameNr);
    if (scanRet == 0) {
        colorCounts.gameNr = -1;
        return colorCounts;
    }
    struct ElementFound element;
    // +2 to have pointer on first count
    char * nextInput = strchr(input, ':') + 2;
    do {
        element = findNext(nextInput);
        if (element.color == 'r' && colorCounts.red < element.number) {
            colorCounts.red = element.number;
        } else if (element.color == 'b' && colorCounts.blue < element.number) {
            colorCounts.blue = element.number;
        } else if (element.color == 'g' && colorCounts.green < element.number) {
            colorCounts.green = element.number;
        }
    }
    while((nextInput = element.next) != NULL);
    return colorCounts;
}

void printColorCounts(struct ColorCounts cc) {
    printf("ANALYSIS: GameNr:%i, green:%i, blue:%i, red:%i \n",cc.gameNr, cc.green, cc.blue, cc.red);
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day2/2/input.txt", "r");
    if (input == NULL) {
        fputs("Could not read file ./input.txt", stderr);
        exit(1);
    }
    char line[MAXLINE];
    int addUpPower = 0;
    while(fgets(line, MAXLINE, input) != NULL) {
        struct ColorCounts erg = readLine(line);
        printColorCounts(erg);
        addUpPower += erg.green * erg.blue * erg.red;
    }
    printf("%i\n", addUpPower);
}
