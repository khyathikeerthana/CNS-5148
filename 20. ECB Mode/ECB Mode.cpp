#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 8  // Define block size (in bytes)

// Function to XOR a block of plaintext with a key
void xor_encrypt_decrypt(char* block, const char* key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key[i];
    }
}

// Function to encrypt plaintext using ECB mode
void ecb_encrypt(const char* plaintext, char* ciphertext, const char* key, int length) {
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(ciphertext + i, plaintext + i, BLOCK_SIZE);
        xor_encrypt_decrypt(ciphertext + i, key);
    }
}

// Function to decrypt ciphertext using ECB mode
void ecb_decrypt(const char* ciphertext, char* plaintext, const char* key, int length) {
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(plaintext + i, ciphertext + i, BLOCK_SIZE);
        xor_encrypt_decrypt(plaintext + i, key);
    }
}

// Function to pad plaintext to a multiple of BLOCK_SIZE
void pad_plaintext(const char* plaintext, char* padded_plaintext, int length, int padded_length) {
    memcpy(padded_plaintext, plaintext, length);
    for (int i = length; i < padded_length; i++) {
        padded_plaintext[i] = 0;  // Pad with zero bytes
    }
}

int main() {
    const char key[BLOCK_SIZE] = "mysecret";  // Key must be BLOCK_SIZE bytes
    const char plaintext[] = "This is a secret message!";
    int length = strlen(plaintext);
    int padded_length = ((length + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;  // Padding for block size

    char* padded_plaintext = (char*)malloc(padded_length);
    char* ciphertext = (char*)malloc(padded_length);
    char* decryptedtext = (char*)malloc(padded_length + 1);

    pad_plaintext(plaintext, padded_plaintext, length, padded_length);

    // Encrypt
    ecb_encrypt(padded_plaintext, ciphertext, key, padded_length);
    printf("Ciphertext: ");
    for (int i = 0; i < padded_length; i++) {
        printf("%02x ", (unsigned char)ciphertext[i]);
    }
    printf("\n");

    // Decrypt
    ecb_decrypt(ciphertext, decryptedtext, key, padded_length);
    decryptedtext[padded_length] = '\0';  // Null-terminate the decrypted text
    printf("Decrypted text: %s\n", decryptedtext);

    free(padded_plaintext);
    free(ciphertext);
    free(decryptedtext);

    return 0;
}

