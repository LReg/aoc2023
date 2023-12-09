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
char getFirstDigit(char * buffer) {
    int position = 0;
    char c;
    while((c = buffer[position++]) != '\n') {
        if (isdigit(c)) {
            return c;
        }
    }
}
char getLastDigit(char * buffer) {
    int position = 0;
    char latestMatch;
    char c;
    while((c = buffer[position++]) != '\n') {
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