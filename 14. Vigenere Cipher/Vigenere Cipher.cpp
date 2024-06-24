#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Function to encrypt the plaintext using the one-time pad key stream
void encrypt(const char* plaintext, const int* key, char* ciphertext, int length) {
    for (int i = 0; i < length; i++) {
        if (plaintext[i] == ' ') {
            ciphertext[i] = ' '; // Maintain spaces in ciphertext
        } else {
            ciphertext[i] = ((plaintext[i] - 'a' + key[i]) % ALPHABET_SIZE) + 'a';
        }
    }
    ciphertext[length] = '\0'; // Null-terminate the string
}

// Function to decrypt the ciphertext using the key stream
void decrypt(const char* ciphertext, const int* key, char* plaintext, int length) {
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] == ' ') {
            plaintext[i] = ' '; // Maintain spaces in plaintext
        } else {
            plaintext[i] = ((ciphertext[i] - 'a' - key[i] + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
        }
    }
    plaintext[length] = '\0'; // Null-terminate the string
}

// Function to find the decryption key for a given ciphertext and desired plaintext
void find_decryption_key(const char* ciphertext, const char* desired_plaintext, int* key, int length) {
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] == ' ' || desired_plaintext[i] == ' ') {
            key[i] = 0; // Spaces do not require encryption
        } else {
            key[i] = (ciphertext[i] - desired_plaintext[i] + ALPHABET_SIZE) % ALPHABET_SIZE;
        }
    }
}

int main() {
    // Part (a)
    const char* plaintext = "send more money";
    int key_stream[] = {9, 0, 1, 7, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int length = strlen(plaintext);
    char ciphertext[length + 1];

    encrypt(plaintext, key_stream, ciphertext, length);

    printf("Encrypted text: %s\n", ciphertext);

    // Part (b)
    const char* new_plaintext = "cash not needed";
    int new_key_stream[length];

    find_decryption_key(ciphertext, new_plaintext, new_key_stream, length);

    printf("New key stream: ");
    for (int i = 0; i < length; i++) {
        if (new_plaintext[i] != ' ') {
            printf("%d ", new_key_stream[i]);
        }
    }
    printf("\n");

    return 0;
}

