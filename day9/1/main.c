// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#define MAXLINE 1000
#define MAXDATASETS 5
#define DEBUG 0
int extrapolateNumber(int * numbers, unsigned int numc) {
    if (DEBUG) {
        for (int p = 0; p < numc; p++) {
            printf("%i\t", numbers[p]);
        }
        puts("");
    }
    int num = 0;
    int lastNum[numc];
    int newLine[numc];
    lastNum[0] = numbers[numc - 1];
    int lastNumC = 1;
    for (int i = 0; i<numc; i++) {
        int j = 0;
        for (; j < numc - i - 1; j++) {
            newLine[j] = numbers[j+1] - numbers[j];
        }
        lastNum[lastNumC++] = newLine[j];
        if (DEBUG) {
            for (int p = 0; p < j; p++) {
                printf("%i\t", newLine[p]);
            }
            puts("");
        }
        bool isAllZero = true;
        for (int t = 0; t < j; t++)
            if (newLine[t] != 0)
                isAllZero = false;
       if (isAllZero)
           break;
       numbers = newLine;
    }
    for ( int i = 0 ; i < lastNumC; i++) {
        num += lastNum[i];
    }
    return num;
}
int main() {
    FILE * input = assertOpenFile("day9/1/input.txt", "r");
    char line[MAXLINE];
    char * linePos = line;
    int numbers[MAXDATASETS];
    int * numbersPos = numbers;
    long int sum = 0;
    while(fgets(line, MAXLINE, input) != NULL){
        while(*linePos != '\0') {
            *numbersPos++ = atoi(linePos);
            linePos = skipOperator(linePos);
            linePos = skipDecimals(linePos) + 1;
        }
        linePos = line;
        int res =  extrapolateNumber(numbers, numbersPos - numbers);
        sum+= res;
        if (DEBUG) {
            printf("number: %i \n", res);
        }
        numbersPos = numbers;
    }
    printf("SUM: %li", sum);
    return 0;
}
