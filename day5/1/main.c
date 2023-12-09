#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define MAXLINE 5000
#define MAXDATASETS 2000
#define MAXSEEDS 25
typedef struct MapStruct{
    int type;
    size_t from;
    size_t to;
    size_t width;
}Map;

size_t readNum(char * num) {
    size_t out;
    sscanf(num, "%zu", &out);
    return out;
}

void printMap(Map m) {
    printf("%i %zu %zu %zi \n", m.type, m.to, m.from, m.width);
}

Map readMap(int type, char * line) {
    Map m;
    m.type = type;
    m.to = readNum(line);
    while(isdigit(*line++))
        ;
    m.from= readNum(line);
    while(isdigit(*line++))
        ;
    m.width = readNum(line);
    return m;
}

size_t findCorrectMapping(size_t id, int type, Map * maps, int mapc) {
    Map * m = NULL;
    for (int i = 0; i < mapc; i++) {
        if (maps[i].type < type)
            continue;
        if (maps[i].type > type)
            break;
        if (id >= maps[i].from && id <= (maps[i].from + maps[i].width)) {
            m = &maps[i];
            break;
        }
    }
    if (m == NULL) {
        return id;
    }
    return id + m->to - m->from ;
}

int main(int argc, char const *argv[] ) {
    FILE * input = fopen("day5/1/input.txt", "r");
    if (input == NULL) {
        fputs("Could not read file ./input.txt", stderr);
        exit(1);
    }
    int numberOfTypes = 7;
    char line[MAXLINE];
    Map * maps = malloc(sizeof(Map) * MAXDATASETS);
    Map * mapPos = maps;
    int mapc = 0;
    size_t * seeds = malloc(sizeof(size_t) * MAXSEEDS);
    size_t * seedPos = seeds;
    fgets(line, MAXLINE, input);
    char * pos = strchr(line, ':') + 1;
    while((*pos) != '\n') {
        if(*pos == ' ')
            pos++;
        if (isdigit(*pos)) {
            *seedPos++ = readNum(pos);
            while(isdigit(*pos))
                pos++;
        }
    }
    fgets(line, MAXLINE, input);
    for (int i = 0; i < numberOfTypes; i++) {
        fgets(line, MAXLINE, input);
        char * ret;
        while((ret = fgets(line, MAXLINE, input)) != NULL && *ret != '\n') {
            Map m = readMap(i, line);
            *mapPos++ = m;
            mapc++;
        }
    }
    // go through seeds
    size_t lowestMapping = ULONG_LONG_MAX;
    int seedId = 0;
    while(seedPos != seeds) {
        printf("Seed: %i. Types: ", seedId++);
        size_t id = *--seedPos;
        for (int i = 0; i < numberOfTypes; i++) {
            id = findCorrectMapping(id, i, maps, mapc);
            printf("%i ", i);
        }
        if (id < lowestMapping) {
            lowestMapping = id;
        }
        printf("\n");
    }
    printf("lowest position: %zu", lowestMapping);
    return 0;
}
