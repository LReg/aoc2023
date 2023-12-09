#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 5000
#define MAXDATASETS 2000

typedef struct CardStruct {
    int *win;
    int winc;
    int winp;
    int *num;
    int numc;
    int nump;
} Card;

void printCard(Card c) {
    int i;
    puts("");
    puts("Numbers:");
    for (i = 0; i <= c.nump; i++) {
        printf("%i ", c.num[i]);
    }
    puts("");
    puts("Win nums:");
    for (i = 0; i <= c.winp; i++) {
        printf("%i ", c.win[i]);
    }
}

Card initCard() {
    Card c;
    c.winc = 10;
    c.win = malloc(sizeof(int) * 10);
    c.winp = -1;
    c.numc = 10;
    c.num = malloc(sizeof(int) * 10);
    c.nump = -1;
    return c;
}

Card increase(Card c) {
    puts("inc");
    c.winc += 10;
    c.numc += 10;
    c.win = realloc(c.win, sizeof(int) * c.winc);
    c.num = realloc(c.num, sizeof(int) * c.numc);
    return c;
}

void deleteCard(Card c) {
    free(c.win);
    free(c.num);
}

int readNum(char * num) {
    int i = 0;
    char c[3];
    while (isdigit(c[i] = num[i]))
        i++;
    c[i] = '\n';
    return atoi(c);
}

Card readCard(char * line) {
    Card c = initCard();
    line = strchr(line, ':') + 1;
    char ch;
    int i;
    for (i = 0; (ch = line[i]) != '|'; i++) {
        if (ch == ' ')
            continue;
        if (isdigit(ch)) {
            if (c.winp == c.winc - 1)
                c = increase(c);
            c.win[++c.winp] = readNum(&line[i]);
            while(isdigit(line[i]))
                i++;
        }
    }
    i++;
    for (; (ch = line[i]) != '\n'; i++) {
        if (ch == ' ')
            continue;
        if (isdigit(ch)) {
            if (c.nump == c.numc - 1)
                c = increase(c);
            c.num[++c.nump] = readNum(&line[i]);
            while(isdigit(line[i]))
                i++;
            i--;
        }
    }
    return c;
}

unsigned int getPoints(Card c) {
    unsigned int points = 0;
    int i, j;
    for (i = 0; i <= c.nump; i++) {
        for (j = 0; j <= c.winp; j++)  {
            if (c.num[i] == c.win[j]) {
                if (points == 0)
                    points++;
                else
                    points *= 2;
            }
        }
    }
    return points;
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day4/1/input.txt", "r");
    if (input == NULL) {
        fputs("Could not read file ./input.txt", stderr);
        exit(1);
    }
    size_t sum = 0;
    char line[MAXLINE];
    while(fgets(line, MAXLINE, input) != NULL) {
        Card c = readCard(line);
        sum += getPoints(c);
        deleteCard(c);
    }
    printf("\nPOINTS: %zu", sum);
}
