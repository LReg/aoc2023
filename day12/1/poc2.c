#include <stdio.h>

// Funktion zur Berechnung des Binomialkoeffizienten
int binomialCoefficient(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    } else {
        // Verwende die rekursive Formel: C(n, k) = C(n-1, k-1) + C(n-1, k)
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }
}

void fillArrayWithCombinations(char array[], int size, char symbol1, char symbol2) {
    int totalCombinations = binomialCoefficient(size, 2);

    for (int i = 0; i < totalCombinations; ++i) {
        // Überprüfe, ob die Anzahl der gesetzten Bits gleich 2 ist
        if (__builtin_popcount(i) == 2) {
            int position = 0;

            // Fülle das Array mit den entsprechenden Symbolen
            for (int j = 0; j < size; ++j) {
                if ((i & (1 << j)) != 0) {
                    array[position++] = symbol1;
                } else {
                    array[position++] = symbol2;
                }
            }

            // Gib die aktuelle Kombination aus
            for (int k = 0; k < size; ++k) {
                printf("%c ", array[k]);
            }
            printf("\n");
        }
    }
}

int main() {
    // Größe des Arrays
    int size = 5;

    // Array erstellen
    char combinations[size];

    // Symbole
    char symbol1 = '#';
    char symbol2 = '.';

    // Funktion aufrufen, um das Array mit Kombinationen zu füllen
    fillArrayWithCombinations(combinations, size, symbol1, symbol2);

    return 0;
}