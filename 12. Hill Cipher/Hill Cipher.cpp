#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MATRIX_SIZE 2

// Key matrix
int key[MATRIX_SIZE][MATRIX_SIZE] = {
    {9, 4},
    {5, 7}
};

// Function to find the modular inverse of a number modulo 26
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return 1; // Default return value (should not happen for valid inputs)
}

// Function to find the determinant of the key matrix modulo 26
int findDeterminant(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    return determinant % 26;
}

// Function to find the inverse of the key matrix modulo 26
void findInverseMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inverseMatrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int determinant = findDeterminant(matrix);
    int invDet = modInverse(determinant, 26);

    inverseMatrix[0][0] = (matrix[1][1] * invDet) % 26;
    inverseMatrix[0][1] = (-matrix[0][1] * invDet) % 26;
    inverseMatrix[1][0] = (-matrix[1][0] * invDet) % 26;
    inverseMatrix[1][1] = (matrix[0][0] * invDet) % 26;

    // Make sure all values are positive
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            if (inverseMatrix[i][j] < 0) {
                inverseMatrix[i][j] += 26;
            }
        }
    }
}

// Function to encrypt a pair of letters using the Hill cipher
void encryptPair(char input[], char output[], int key[MATRIX_SIZE][MATRIX_SIZE]) {
    int x = input[0] - 'a';
    int y = input[1] - 'a';

    int encryptedX = (key[0][0] * x + key[0][1] * y) % 26;
    int encryptedY = (key[1][0] * x + key[1][1] * y) % 26;

    output[0] = encryptedX + 'a';
    output[1] = encryptedY + 'a';
}

// Function to decrypt a pair of letters using the Hill cipher
void decryptPair(char input[], char output[], int inverseKey[MATRIX_SIZE][MATRIX_SIZE]) {
    int x = input[0] - 'a';
    int y = input[1] - 'a';

    int decryptedX = (inverseKey[0][0] * x + inverseKey[0][1] * y) % 26;
    int decryptedY = (inverseKey[1][0] * x + inverseKey[1][1] * y) % 26;

    output[0] = decryptedX + 'a';
    output[1] = decryptedY + 'a';
}

// Function to process the input and remove non-alphabet characters
void preprocessInput(char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i] != '\0'; i++) {
        if (isalpha(input[i])) {
            output[j++] = tolower(input[i]);
        }
    }
    output[j] = '\0';
}

int main() {
    char plaintext[] = "meet me at the usual place at ten rather than eight oclock";
    char processedPlaintext[100];
    char ciphertext[100];
    char decryptedText[100];
    int inverseKey[MATRIX_SIZE][MATRIX_SIZE];

    // Preprocess the plaintext to remove spaces and non-alphabet characters
    preprocessInput(plaintext, processedPlaintext);

    // Add a padding character 'x' if the length of the plaintext is odd
    int len = strlen(processedPlaintext);
    if (len % 2 != 0) {
        processedPlaintext[len] = 'x';
        processedPlaintext[len + 1] = '\0';
    }

    // Encrypt the plaintext
    for (int i = 0; i < strlen(processedPlaintext); i += 2) {
        encryptPair(&processedPlaintext[i], &ciphertext[i], key);
    }
    ciphertext[strlen(processedPlaintext)] = '\0';

    // Find the inverse key matrix
    findInverseMatrix(key, inverseKey);

    // Decrypt the ciphertext
    for (int i = 0; i < strlen(ciphertext); i += 2) {
        decryptPair(&ciphertext[i], &decryptedText[i], inverseKey);
    }
    decryptedText[strlen(ciphertext)] = '\0';

    // Print the results
    printf("Plaintext: %s\n", plaintext);
    printf("Processed Plaintext: %s\n", processedPlaintext);
    printf("Ciphertext: %s\n", ciphertext);
    printf("Decrypted Text: %s\n", decryptedText);

    return 0;
}

