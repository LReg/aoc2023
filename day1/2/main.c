#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int getline(char * buffer) {
    char c;
    int position = 0;
    while((c = getc(stdin)) != EOF && c != '\n') {
        buffer[position++] = c;
    }
    if (c == EOF) {
        buffer[position] = '\n';
        return 0;
    }
    buffer[position] = '\n';
    return 1;
}
void printline(char * buffer) {
    int position = 0;
    char c;
    while((c = buffer[position++]) != '\n' && c != EOF) {
        putchar(c);
    }
    putchar('\n');
}
char intToChar(int num) {
    if (num >= 1 && num <= 9) {
        return (char)(num + '0');
    } else {
        return '\0';
    }
}
int isWrittenDigit(char * buffer, int pos) {
    size_t len = strlen(buffer + pos);
    if (len >= 5) {
        if (strncmp(buffer + pos, "three", 5) == 0)
            return 3;
        if (strncmp(buffer + pos, "seven", 5) == 0)
            return 7;
        if (strncmp(buffer + pos, "eight", 5) == 0)
            return 8;
    }
    if (len >= 4) {
        if (strncmp(buffer + pos, "four", 4) == 0)
            return 4;
        if (strncmp(buffer + pos, "five", 4) == 0)
            return 5;
        if (strncmp(buffer + pos, "nine", 4) == 0)
            return 9;
    }
    if (len >= 3) {
        if (strncmp(buffer + pos, "one", 3) == 0)
            return 1;
        if (strncmp(buffer + pos, "two", 3) == 0)
            return 2;
        if (strncmp(buffer + pos, "six", 3) == 0)
            return 6;
    }
    return 0;
}
char getFirstDigit(char * buffer) {
    int position = 0;
    char c;
    int wd;
    while((c = buffer[position++]) != '\n') {
        if ((wd = isWrittenDigit(buffer, position - 1)) != 0) {
            return intToChar(wd);
        }
        if (isdigit(c)) {
            return c;
        }
    }
    return 'E';
}
char getLastDigit(char * buffer) {
    int position = 0;
    char latestMatch = 'E';
    char c;
    int wd;
    while((c = buffer[position++]) != '\n') {
        if ((wd = isWrittenDigit(buffer, position - 1)) != 0) {
            latestMatch = intToChar(wd);
        }
        if (isdigit(c)) {
            latestMatch = c;
        }
    }
    return latestMatch;
}
int getValue(char * buffer) {
    char * cat = malloc(3);
    sprintf(cat,"%c%c\n", getFirstDigit(buffer), getLastDigit(buffer));
    const int ret = atoi(cat);
    free(cat);
    return ret;
}
int main(int argc, char const *argv[]) {
    int buffersize = 50;
    char * linebuffer = (char *) malloc(buffersize * sizeof(char));
    int sum = 0;
    while(getline(linebuffer)) {
        sum += getValue(linebuffer);
    }
    printf("%i", sum);
    return 0;
}