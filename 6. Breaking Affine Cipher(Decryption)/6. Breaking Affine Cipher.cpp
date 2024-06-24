#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function to find modular multiplicative inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 0) {
            return -1;
        }
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Function to decrypt affine cipher
void decryptAffine(char *ciphertext, int a, int b, int m) {
    int a_inv = modInverse(a, m);
    if (a_inv == -1) {
        printf("No modular multiplicative inverse for a = %d\n", a);
        return;
    }
    
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char c = toupper(ciphertext[i]);
        if (c >= 'A' && c <= 'Z') {
            int y = c - 'A';
            int x = (a_inv * (y - b + m)) % m;
            if (x < 0) x += m;  // Ensure positive result
            printf("%c", (char)(x + 'A'));
        } else {
            printf("%c", ciphertext[i]);
        }
    }
    printf("\n");
}

int main() {
    char ciphertext[1000];
    int m = 26;

    // Input the ciphertext
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);

    // Mapping B to E and U to T
    int x1 = 'E' - 'A'; // 4
    int y1 = 'B' - 'A'; // 1
    int x2 = 'T' - 'A'; // 19
    int y2 = 'U' - 'A'; // 20

    // Solving the system of equations
    // y1 = (a * x1 + b) % m
    // y2 = (a * x2 + b) % m

    int a = ((y1 - y2) * modInverse(x1 - x2 + m, m)) % m;
    if (a < 0) a += m;
    int b = (y1 - a * x1) % m;
    if (b < 0) b += m;

    printf("Determined keys: a = %d, b = %d\n", a, b);

    printf("Decrypted message: ");
    decryptAffine(ciphertext, a, b, m);

    return 0;
}

