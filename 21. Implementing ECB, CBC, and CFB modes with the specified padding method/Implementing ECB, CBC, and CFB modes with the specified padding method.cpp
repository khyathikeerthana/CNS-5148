#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 8  // Define block size (in bytes)

// Function to XOR a block of plaintext with a key
void xor_encrypt_decrypt(char* block, const char* key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] ^= key[i];
    }
}

// Function to pad plaintext to a multiple of BLOCK_SIZE
void pad_plaintext(const char* plaintext, char* padded_plaintext, int length, int padded_length) {
    memcpy(padded_plaintext, plaintext, length);
    padded_plaintext[length] = 0x80;  // Add 1 bit followed by zero bits
    for (int i = length + 1; i < padded_length; i++) {
        padded_plaintext[i] = 0;
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

// Function to encrypt plaintext using CBC mode
void cbc_encrypt(const char* plaintext, char* ciphertext, const char* key, const char* iv, int length) {
    char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < length; i += BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ prev_block[j];
        }
        xor_encrypt_decrypt(ciphertext + i, key);
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

// Function to decrypt ciphertext using CBC mode
void cbc_decrypt(const char* ciphertext, char* plaintext, const char* key, const char* iv, int length) {
    char prev_block[BLOCK_SIZE];
    char temp_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(temp_block, ciphertext + i, BLOCK_SIZE);
        xor_encrypt_decrypt(plaintext + i, key);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] ^= prev_block[j];
        }
        memcpy(prev_block, temp_block, BLOCK_SIZE);
    }
}

// Function to encrypt plaintext using CFB mode
void cfb_encrypt(const char* plaintext, char* ciphertext, const char* key, const char* iv, int length) {
    char prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < length; i += BLOCK_SIZE) {
        xor_encrypt_decrypt(prev_block, key);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ prev_block[j];
        }
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

// Function to decrypt ciphertext using CFB mode
void cfb_decrypt(const char* ciphertext, char* plaintext, const char* key, const char* iv, int length) {
    char prev_block[BLOCK_SIZE];
    char temp_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);

    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(temp_block, ciphertext + i, BLOCK_SIZE);
        xor_encrypt_decrypt(prev_block, key);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            plaintext[i + j] = ciphertext[i + j] ^ prev_block[j];
        }
        memcpy(prev_block, temp_block, BLOCK_SIZE);
    }
}

int main() {
    const char key[BLOCK_SIZE] = "mysecret";  // Key must be BLOCK_SIZE bytes
    const char iv[BLOCK_SIZE] = "initvect";   // Initialization vector must be BLOCK_SIZE bytes
    const char plaintext[] = "This is a secret message!";
    int length = strlen(plaintext);
    int padded_length = ((length + BLOCK_SIZE) / BLOCK_SIZE) * BLOCK_SIZE;  // Ensure padding is always added

    char* padded_plaintext = (char*)malloc(padded_length);
    char* ciphertext = (char*)malloc(padded_length);
    char* decryptedtext = (char*)malloc(padded_length + 1);

    pad_plaintext(plaintext, padded_plaintext, length, padded_length);

    // Encrypt and decrypt using ECB mode
    ecb_encrypt(padded_plaintext, ciphertext, key, padded_length);
    printf("ECB Ciphertext: ");
    for (int i = 0; i < padded_length; i++) {
        printf("%02x ", (unsigned char)ciphertext[i]);
    }
    printf("\n");

    ecb_decrypt(ciphertext, decryptedtext, key, padded_length);
    decryptedtext[padded_length] = '\0';  // Null-terminate the decrypted text
    printf("ECB Decrypted text: %s\n\n", decryptedtext);

    // Encrypt and decrypt using CBC mode
    cbc_encrypt(padded_plaintext, ciphertext, key, iv, padded_length);
    printf("CBC Ciphertext: ");
    for (int i = 0; i < padded_length; i++) {
        printf("%02x ", (unsigned char)ciphertext[i]);
    }
    printf("\n");

    cbc_decrypt(ciphertext, decryptedtext, key, iv, padded_length);
    decryptedtext[padded_length] = '\0';  // Null-terminate the decrypted text
    printf("CBC Decrypted text: %s\n\n", decryptedtext);

    // Encrypt and decrypt using CFB mode
    cfb_encrypt(padded_plaintext, ciphertext, key, iv, padded_length);
    printf("CFB Ciphertext: ");
    for (int i = 0; i < padded_length; i++) {
        printf("%02x ", (unsigned char)ciphertext[i]);
    }
    printf("\n");

    cfb_decrypt(ciphertext, decryptedtext, key, iv, padded_length);
    decryptedtext[padded_length] = '\0';  // Null-terminate the decrypted text
    printf("CFB Decrypted text: %s\n\n", decryptedtext);

    free(padded_plaintext);
    free(ciphertext);
    free(decryptedtext);

    return 0;
}

