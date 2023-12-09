#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 5000
#define MAXDATASETS 2000

typedef struct Race {
    size_t time;
    size_t distance;
} Race;

size_t readNum(char * num) {
    size_t out;
    sscanf(num, "%zu", &out);
    return out;
}

void printRaces(Race * races, unsigned int racec) {
    for (int i = 0; i < racec; i++)  {
        printf("time %zu, dist %zu\n", races[i].time, races[i].distance);
    }
}

unsigned int beatRecordAmount(Race r) {
    size_t  lowest = 0, highest = ULONG_LONG_MAX;

    for (size_t pressTime = 1; pressTime < r.time; pressTime++) {
        size_t leftTime = r.time - pressTime;
        size_t resultingDistance = leftTime * pressTime;
        if (resultingDistance > r.distance) {
            lowest = pressTime;
            break;
        }
    }

    for (size_t pressTime = r.time; pressTime > 0; pressTime--) {
        size_t leftTime = r.time - pressTime;
        size_t resultingDistance = leftTime * pressTime;
        if (resultingDistance > r.distance) {
            highest = pressTime;
            break;
        }
    }

    //printf("highest: %u, lowest %u", highest, lowest);
    return highest - lowest + 1;
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day6/2/input.txt", "r");
    if (input == NULL) {
        fputs("Could not read file ./input.txt", stderr);
        exit(1);
    }
    char line[MAXLINE];
    Race * races = malloc(sizeof(Race) * MAXDATASETS);
    Race * racePos = races;
    unsigned int racec = 0;

    //times
    fgets(line, MAXLINE, input);
    char * pos = strchr(line, ':') + 1;

    while(*pos != '\n') {
        while(*pos== ' ')
            pos++;
        Race r;
        r.time = readNum(pos);
        *racePos++ = r;
        racec++;
        while(isdigit(*pos))
            pos++;
    }

    // Distances
    fgets(line, MAXLINE, input);
    racePos = races;
    pos = strchr(line, ':') + 1;
    while(*pos != '\n') {
        while(*pos== ' ')
            pos++;
        (*racePos++).distance = readNum(pos);
        while(isdigit(*pos))
            pos++;
    }

    printRaces(races, racec);

    size_t sum = 1;
    for (int i = 0; i < racec; i++) {
        sum *= beatRecordAmount(races[i]);
    }

    printf("LSG: %zu", sum);
    return 0;
}
