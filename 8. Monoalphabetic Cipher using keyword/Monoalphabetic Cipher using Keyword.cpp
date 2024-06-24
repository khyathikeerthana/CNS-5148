#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_LEN 26
#define MAX_LEN 1000

void generateCipherSequence(char keyword[], char cipher[]) {
    int used[ALPHABET_LEN] = {0};
    int keyword_len = strlen(keyword);
    int cipher_index = 0;

    // Add the keyword to the cipher
    for (int i = 0; i < keyword_len; i++) {
        char c = toupper(keyword[i]);
        if (!used[c - 'A']) {
            cipher[cipher_index++] = c;
            used[c - 'A'] = 1;
        }
    }

    // Add the remaining letters to the cipher
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!used[c - 'A']) {
            cipher[cipher_index++] = c;
        }
    }

    cipher[cipher_index] = '\0';
}

void encrypt(char plaintext[], char cipher[], char encrypted[]) {
    int plaintext_len = strlen(plaintext);
    for (int i = 0; i < plaintext_len; i++) {
        char c = toupper(plaintext[i]);
        if (isalpha(c)) {
            encrypted[i] = cipher[c - 'A'];
        } else {
            encrypted[i] = c;
        }
    }
    encrypted[plaintext_len] = '\0';
}

void decrypt(char ciphertext[], char cipher[], char decrypted[]) {
    int ciphertext_len = strlen(ciphertext);
    for (int i = 0; i < ciphertext_len; i++) {
        char c = toupper(ciphertext[i]);
        if (isalpha(c)) {
            for (int j = 0; j < ALPHABET_LEN; j++) {
                if (cipher[j] == c) {
                    decrypted[i] = 'A' + j;
                    break;
                }
            }
        } else {
            decrypted[i] = c;
        }
    }
    decrypted[ciphertext_len] = '\0';
}

int main() {
    char keyword[] = "CIPHER";
    char cipher[ALPHABET_LEN + 1];
    char plaintext[MAX_LEN];
    char encrypted[MAX_LEN];
    char decrypted[MAX_LEN];

    generateCipherSequence(keyword, cipher);

    printf("Keyword: %s\n", keyword);
    printf("Cipher sequence: %s\n", cipher);

    printf("Enter plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0';  // Remove trailing newline

    encrypt(plaintext, cipher, encrypted);
    printf("Encrypted: %s\n", encrypted);

    decrypt(encrypted, cipher, decrypted);
    printf("Decrypted: %s\n", decrypted);

    return 0;
}

