//
// Created by abbing on 04/2/2023.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int getline(char * buffer) {
    char c;
    int position = 0;
    while((c = (char) getc(stdin)) != EOF && c != '\n') {
        putchar(c);
        buffer[position++] = c;
    }
    if (c == EOF) {
        putchar(c);
        buffer[position] = '\n';
        return 0;
    }
    putchar(c);
    buffer[position] = '\n';
    return 1;
}
int main(int argc, char const *argv[]) {
    char buffer[5];
    itoa(strlen("123"), buffer, 10);
    puts(buffer);
    /*char c;
    while((c = getchar()) != EOF) {
        if (c == '\n') {
            putchar('X');
        }
        putchar(c);
    }
     */
}
