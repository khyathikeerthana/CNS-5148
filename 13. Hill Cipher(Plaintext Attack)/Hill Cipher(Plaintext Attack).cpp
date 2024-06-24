#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MATRIX_SIZE 3

void getKeyMatrix(int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], char key[]) {
    int k = 0;
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            keyMatrix[i][j] = (key[k]) % 65;
            k++;
        }
    }
}

void encrypt(int cipherMatrix[], int keyMatrix[MATRIX_SIZE][MATRIX_SIZE], int messageVector[]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        cipherMatrix[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            cipherMatrix[i] += keyMatrix[i][j] * messageVector[j];
        }
        cipherMatrix[i] = cipherMatrix[i] % 26;
    }
}

void HillCipher(char message[], char key[]) {
    int keyMatrix[MATRIX_SIZE][MATRIX_SIZE];
    getKeyMatrix(keyMatrix, key);

    int messageVector[MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        messageVector[i] = (message[i]) % 65;
    }

    int cipherMatrix[MATRIX_SIZE];
    encrypt(cipherMatrix, keyMatrix, messageVector);

    char CipherText[MATRIX_SIZE + 1];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        CipherText[i] = cipherMatrix[i] + 65;
    }
    CipherText[MATRIX_SIZE] = '\0';

    printf("Ciphertext: %s\n", CipherText);
}

int main() {
    char message[MATRIX_SIZE + 1];
    char key[MATRIX_SIZE * MATRIX_SIZE + 1];

    printf("Enter a 3-letter message: ");
    scanf("%s", message);

    printf("Enter a 9-letter key: ");
    scanf("%s", key);

    HillCipher(message, key);

    return 0;
}

