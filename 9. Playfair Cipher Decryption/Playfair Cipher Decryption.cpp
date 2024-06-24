#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Function to remove duplicates from the keyword and combine I and J
void prepareKeyword(char keyword[], char keyTable[SIZE][SIZE]) {
    int used[26] = {0};
    int row = 0, col = 0;

    // Fill key table with the keyword
    for (int i = 0; i < strlen(keyword); i++) {
        char c = toupper(keyword[i]);
        if (c == 'J') c = 'I'; // Combine I and J
        if (!used[c - 'A']) {
            keyTable[row][col++] = c;
            used[c - 'A'] = 1;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }

    // Fill key table with remaining letters
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue; // Skip J
        if (!used[c - 'A']) {
            keyTable[row][col++] = c;
            used[c - 'A'] = 1;
            if (col == SIZE) {
                col = 0;
                row++;
            }
        }
    }
}

// Function to find the position of a character in the key table
void findPosition(char keyTable[SIZE][SIZE], char c, int *row, int *col) {
    if (c == 'J') c = 'I'; // Combine I and J
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to decrypt a digraph using the Playfair cipher
void decryptDigraph(char keyTable[SIZE][SIZE], char digraph[], char decrypted[]) {
    int row1, col1, row2, col2;
    findPosition(keyTable, digraph[0], &row1, &col1);
    findPosition(keyTable, digraph[1], &row2, &col2);

    if (row1 == row2) {
        decrypted[0] = keyTable[row1][(col1 + SIZE - 1) % SIZE];
        decrypted[1] = keyTable[row2][(col2 + SIZE - 1) % SIZE];
    } else if (col1 == col2) {
        decrypted[0] = keyTable[(row1 + SIZE - 1) % SIZE][col1];
        decrypted[1] = keyTable[(row2 + SIZE - 1) % SIZE][col2];
    } else {
        decrypted[0] = keyTable[row1][col2];
        decrypted[1] = keyTable[row2][col1];
    }
}

// Function to decrypt the entire ciphertext
void decryptMessage(char keyTable[SIZE][SIZE], char ciphertext[], char decrypted[]) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i += 2) {
        char digraph[2] = {ciphertext[i], ciphertext[i + 1]};
        char decryptedDigraph[2];
        decryptDigraph(keyTable, digraph, decryptedDigraph);
        decrypted[i] = decryptedDigraph[0];
        decrypted[i + 1] = decryptedDigraph[1];
    }
    decrypted[len] = '\0';
}

int main() {
    char keyword[] = "PLAYFAIR";
    char keyTable[SIZE][SIZE];
    char ciphertext[] = "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPOBOTEIZONTXBYBNTGONEYC"
                        "UZWRGDSONSXBOUYWRHEBAAHYUSEDQ";
    char decrypted[strlen(ciphertext) + 1];

    // Prepare the key table
    prepareKeyword(keyword, keyTable);

    // Print the key table
    printf("Key Table:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", keyTable[i][j]);
        }
        printf("\n");
    }

    // Decrypt the message
    decryptMessage(keyTable, ciphertext, decrypted);

    // Print the decrypted message
    printf("Decrypted Message: %s\n", decrypted);

    return 0;
}

