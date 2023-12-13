#include <stdio.h>

int binomialCoefficient(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    } else {
        // Verwende die rekursive Formel: C(n, k) = C(n-1, k-1) + C(n-1, k)
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }
}

void fillArrayWithCombinations(char array[], int size, char symbol1, char symbol2, int amountOfSymbols) {
    int totalCombinations = binomialCoefficient(size, amountOfSymbols);

    for (int i = 0; i < totalCombinations; ++i) {
        for (int j = 0; j <= size - amountOfSymbols; ++j) {
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
    fillArrayWithCombinations(combinations, size, symbol1, symbol2, 2);

    return 0;
}