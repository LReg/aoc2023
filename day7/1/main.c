// https://github.com/LReg/clib
#include "../../../clib/lib.c"
#define MAXLINE 1000
#define MAXDATASETS 5000
#define NUMBEROFCARDS 5
unsigned short convertStrength(char c) {
    switch(c) {
        case 'T': return 11;
        case 'J': return 12;
        case 'Q': return 13;
        case 'K': return 14;
        case 'A': return 15;
        default: return 0;
    }
}
typedef enum Picture {
    FIVE = 7,
    FOUR = 6,
    FULLHOUSE = 5,
    THREE = 4,
    TWOTWO = 3,
    TWO = 2,
    HIGHCARD = 1,
} Picture;

Picture toPicture(unsigned short timesSame) {
    switch(timesSame) {
        case 5: return FIVE;
        case 4: return FOUR;
        case 3: return THREE;
        case 2: return TWO;
        default: return HIGHCARD;
    }
}

typedef struct Hand {
    unsigned short cards[5];
    unsigned int bid;
    Picture picture;
} Hand;

void printHand(Hand h) {
    printf("Cards: %u|%u|%u|%u|%u,\t Bid: %u,\t Picture: %i\n", h.cards[0], h.cards[1], h.cards[2], h.cards[3], h.cards[4], h.bid, h.picture);
}

Hand analysePicture(Hand h) {
    Picture highestPicture = HIGHCARD;
    unsigned short cardOfPic = -1;
    for(int i = 0; i < NUMBEROFCARDS; i++) {
        unsigned short timesSame = 1;
        for (int j = i + 1; j < NUMBEROFCARDS; j++) {
            if (h.cards[i] == h.cards[j])
                timesSame++;
        }
        Picture p = toPicture(timesSame);
        if (p > highestPicture) {
            highestPicture = p;
            cardOfPic = h.cards[i];
        }
    }
    if (highestPicture == TWO) {
        unsigned short timesSame = 1;
        for (int i = 0; i < NUMBEROFCARDS; i++)
            if (h.cards[i] != cardOfPic)
                for (int j = i + 1; j < NUMBEROFCARDS; j++)
                    if (h.cards[i] == h.cards[j])
                        timesSame++;
        if (timesSame >= 2)
            highestPicture = TWOTWO;
    }
    else if (highestPicture == THREE) {
        unsigned short timesSame = 1;
        for (int i = 0; i < NUMBEROFCARDS; i++)
            if (h.cards[i] != cardOfPic)
                for (int j = i + 1; j < NUMBEROFCARDS; j++)
                    if (h.cards[i] == h.cards[j])
                        timesSame++;
        if (timesSame >= 2)
            highestPicture = FULLHOUSE;
    }
    h.picture = highestPicture;
    return h;
}
int compareHands(Hand h1, Hand h2) {
    if (h1.picture != h2.picture)
        // h1 higher -> return positive
        return (h1.picture - h2.picture) * 16;
    for (int i = 0; i < NUMBEROFCARDS; i++)
        if (h1.cards[i] != h2.cards[i])
            return h1.cards[i] - h2.cards[i];
    return 0;
}

int main() {
    FILE* file = assertOpenFile("day7/1/input.txt", "r");
    char line[MAXLINE];
    Hand hands[MAXDATASETS];
    Hand * handsPos = hands;
    unsigned int handsc = 0;
    while(fgets(line, MAXLINE, file) != NULL) {
        Hand h;
        for(int i = 0; i < NUMBEROFCARDS; i++) {
            h.cards[i] = isdigit(line[i]) ? line[i] - '0' : convertStrength(line[i]);
        }
        h = analysePicture(h);
        char * bid = strchr(line, ' ') + 1;
        h.bid = atoi(bid);
        *handsPos++ = h;
        handsc++;
    }
    for (int i = 0; i <= handsc; i++) {
        for (int j = i + 1; j < handsc; j++) {
            if (compareHands(hands[i], hands[j]) > 0) {
                Hand h = hands[i];
                hands[i] = hands[j];
                hands[j] = h;
            }
        }
    }

    size_t sum = 0;
    for (int i = 0; i < handsc; i++) {
        //printf("%i * %i\n", hands[i].bid, i+1);
        sum += hands[i].bid * (i+1);
    }
    printf("SUM:%zu\n", sum);
    /*for (int i = 0; i < handsc; i++) {
        printHand(hands[i]);
    }*/
    return 0;
}