#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

// Function to find the position of a character in the Playfair matrix
void findPosition(char matrix[SIZE][SIZE], char c, int *row, int *col) {
    if (c == 'J') c = 'I'; // Combine I and J
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (matrix[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

// Function to decrypt a digraph using the Playfair cipher
void decryptDigraph(char matrix[SIZE][SIZE], char digraph[], char decrypted[]) {
    int row1, col1, row2, col2;
    findPosition(matrix, digraph[0], &row1, &col1);
    findPosition(matrix, digraph[1], &row2, &col2);

    if (row1 == row2) {
        decrypted[0] = matrix[row1][(col1 + SIZE - 1) % SIZE];
        decrypted[1] = matrix[row2][(col2 + SIZE - 1) % SIZE];
    } else if (col1 == col2) {
        decrypted[0] = matrix[(row1 + SIZE - 1) % SIZE][col1];
        decrypted[1] = matrix[(row2 + SIZE - 1) % SIZE][col2];
    } else {
        decrypted[0] = matrix[row1][col2];
        decrypted[1] = matrix[row2][col1];
    }
}

// Function to decrypt the entire ciphertext
void decryptMessage(char matrix[SIZE][SIZE], char ciphertext[], char decrypted[]) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i += 2) {
        char digraph[2] = {toupper(ciphertext[i]), toupper(ciphertext[i + 1])};
        char decryptedDigraph[2];
        decryptDigraph(matrix, digraph, decryptedDigraph);
        decrypted[i] = decryptedDigraph[0];
        decrypted[i + 1] = decryptedDigraph[1];
    }
    decrypted[len] = '\0';
}

int main() {
    // Define the Playfair matrix
    char matrix[SIZE][SIZE] = {
        {'M', 'F', 'H', 'I', 'K'},
        {'U', 'N', 'O', 'P', 'Q'},
        {'Z', 'V', 'W', 'X', 'Y'},
        {'E', 'L', 'A', 'R', 'G'},
        {'D', 'S', 'T', 'B', 'C'}
    };

    char ciphertext[1000]; // Assuming maximum length of ciphertext
    char decrypted[1000];

    // Input the ciphertext from the output terminal
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  // Remove newline if present

    // Decrypt the message
    decryptMessage(matrix, ciphertext, decrypted);

    // Print the decrypted message
    printf("Decrypted Message: %s\n", decrypted);

    return 0;
}

