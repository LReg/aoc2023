#include <stdio.h>


void fillArrayWithHashes(char array[], int arr_length, int num_hashes, int index) {
    printf("ITERATION: %s\n", array);
    if (num_hashes == 0) {
        printf("DONE: %s\n", array);
        return;
    }

    int i;
    for (i = index; i < arr_length; i++) {
        array[i] = '#';
        fillArrayWithHashes(array, arr_length, num_hashes - 1, i + 1);
        array[i] = '.';
    }
}

int main() {
    int arr_length = 3;
    int num_hashes = 2;

    char array[100]; // Du kannst die Größe anpassen, je nach deinen Anforderungen
    int i;
    for (i = 0; i < arr_length; i++) {
        array[i] = '.';
    }

    fillArrayWithHashes(array, arr_length, num_hashes, 0);

    return 0;
}
