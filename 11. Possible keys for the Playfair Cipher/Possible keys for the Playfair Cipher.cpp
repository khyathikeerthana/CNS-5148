#include <stdio.h>
#include <math.h>

int main() {
    int alphabet_size = 26; // Total letters in the English alphabet
    long long factorial = 1;

    // Calculate factorial of 25 (since we choose 25 out of 26 letters)
    for (int i = 1; i <= alphabet_size - 1; ++i) {
        factorial *= i;
    }

    // Total number of ways to choose 25 letters from 26 (I/J combined) and arrange them
    long long total_keys = alphabet_size * factorial;

    // Calculate approximate power of 2 representing the number of keys
    double approximate_power_of_2 = log2(total_keys);

    // Output the results
    printf("Total possible keys for the Playfair cipher: %lld\n", total_keys);
    printf("Approximate power of 2 representing the number of keys: %.2f\n", approximate_power_of_2);

    return 0;
}

