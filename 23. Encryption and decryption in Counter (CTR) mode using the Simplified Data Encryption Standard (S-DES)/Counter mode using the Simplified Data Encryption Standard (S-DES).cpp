#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 8  // Define block size (in bits)
#define BYTE_SIZE 8

// Initial permutation (IP) and its inverse (IP-1)
const int IP[] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_inv[] = {4, 1, 3, 5, 7, 2, 8, 6};

// Permutation P10 and P8 for key generation
const int P10[] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[] = {6, 3, 7, 4, 8, 5, 10, 9};

// Expansion permutation (EP)
const int EP[] = {4, 1, 2, 3, 2, 3, 4, 1};

// Permutation P4
const int P4[] = {2, 4, 3, 1};

// S-boxes
const int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};
const int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Utility function to print binary data
void print_binary(const char* label, unsigned char* data, int length) {
    printf("%s: ", label);
    for (int i = 0; i < length; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (data[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

// Function to permute input block
void permute(const unsigned char* input, unsigned char* output, const int* permutation, int n) {
    for (int i = 0; i < n; i++) {
        int pos = permutation[i] - 1;
        int byte_pos = pos / BYTE_SIZE;
        int bit_pos = pos % BYTE_SIZE;
        int bit = (input[byte_pos] >> (BYTE_SIZE - 1 - bit_pos)) & 1;
        int output_byte_pos = i / BYTE_SIZE;
        int output_bit_pos = i % BYTE_SIZE;
        output[output_byte_pos] |= bit << (BYTE_SIZE - 1 - output_bit_pos);
    }
}

// Function to generate subkeys
void generate_subkeys(const unsigned char* key, unsigned char* K1, unsigned char* K2) {
    unsigned char permuted_key[2] = {0, 0};
    permute(key, permuted_key, P10, 10);

    // Split and left shift
    unsigned char LS1 = ((permuted_key[0] << 1) & 0xFE) | ((permuted_key[0] >> 4) & 0x01);
    unsigned char LS2 = ((permuted_key[1] << 1) & 0xFE) | ((permuted_key[0] >> 7) & 0x01);

    unsigned char combined_LS1_LS2 = (LS1 << 4) | (LS2 >> 4);
    permute(&combined_LS1_LS2, K1, P8, 8);

    // Split and left shift again
    LS1 = ((LS1 << 2) & 0xFC) | ((LS1 >> 3) & 0x03);
    LS2 = ((LS2 << 2) & 0xFC) | ((LS2 >> 3) & 0x03);

    combined_LS1_LS2 = (LS1 << 4) | (LS2 >> 4);
    permute(&combined_LS1_LS2, K2, P8, 8);
}

// S-box lookup
unsigned char sbox_lookup(unsigned char input, const int sbox[4][4]) {
    int row = ((input >> 3) & 0x02) | (input & 0x01);
    int col = (input >> 1) & 0x03;
    return sbox[row][col];
}

// Feistel function F
void feistel_function(const unsigned char* half_block, const unsigned char* subkey, unsigned char* output) {
    unsigned char expanded_half_block = 0;
    permute(half_block, &expanded_half_block, EP, 8);

    unsigned char temp = expanded_half_block ^ *subkey;
    unsigned char left_half = sbox_lookup((temp >> 4) & 0x0F, S0);
    unsigned char right_half = sbox_lookup(temp & 0x0F, S1);

    unsigned char combined_sbox_output = (left_half << 2) | right_half;
    permute(&combined_sbox_output, output, P4, 4);
}

// S-DES encryption function
void sdes_encrypt(const unsigned char* plaintext, unsigned char* ciphertext, const unsigned char* K1, const unsigned char* K2) {
    unsigned char permuted_block[1] = {0};
    permute(plaintext, permuted_block, IP, 8);

    unsigned char left_half = permuted_block[0] >> 4;
    unsigned char right_half = permuted_block[0] & 0x0F;

    unsigned char feistel_output[1] = {0};
    feistel_function(&right_half, K1, feistel_output);
    unsigned char temp = left_half ^ feistel_output[0];
    left_half = right_half;
    right_half = temp;

    feistel_function(&right_half, K2, feistel_output);
    temp = left_half ^ feistel_output[0];
    left_half = right_half;
    right_half = temp;

    unsigned char pre_output = (left_half << 4) | right_half;
    permute(&pre_output, ciphertext, IP_inv, 8);
}

// S-DES decryption function
void sdes_decrypt(const unsigned char* ciphertext, unsigned char* plaintext, const unsigned char* K1, const unsigned char* K2) {
    unsigned char permuted_block[1] = {0};
    permute(ciphertext, permuted_block, IP, 8);

    unsigned char left_half = permuted_block[0] >> 4;
    unsigned char right_half = permuted_block[0] & 0x0F;

    unsigned char feistel_output[1] = {0};
    feistel_function(&right_half, K2, feistel_output);
    unsigned char temp = left_half ^ feistel_output[0];
    left_half = right_half;
    right_half = temp;

    feistel_function(&right_half, K1, feistel_output);
    temp = left_half ^ feistel_output[0];
    left_half = right_half;
    right_half = temp;

    unsigned char pre_output = (left_half << 4) | right_half;
    permute(&pre_output, plaintext, IP_inv, 8);
}

// Function to increment counter
void increment_counter(unsigned char* counter) {
    counter[0]++;
}

// Function to encrypt plaintext using CTR mode with S-DES
void ctr_encrypt(const unsigned char* plaintext, unsigned char* ciphertext, const unsigned char* key, unsigned char* counter, int length) {
    unsigned char K1[1] = {0}, K2[1] = {0};
    generate_subkeys(key, K1, K2);

    for (int i = 0; i < length; i++) {
        unsigned char counter_output[1] = {0};
        sdes_encrypt(counter, counter_output, K1, K2);
        ciphertext[i] = plaintext[i] ^ counter_output[0];
        increment_counter(counter);
    }
}

// Function to decrypt ciphertext using CTR mode with S-DES
void ctr_decrypt(const unsigned char* ciphertext, unsigned char* plaintext, const unsigned char* key, unsigned char* counter, int length) {
    // CTR mode decryption is identical to encryption
    ctr_encrypt(ciphertext, plaintext, key, counter, length);
}

int main() {
    unsigned char counter[1] = {0x00};  // Binary: 0000 0000
    unsigned char plaintext[3] = {0x01, 0x02, 0x04};  // Binary: 0000 0001 0000 0010 0000 0100
    unsigned char key[1] = {0x7D};  // Binary: 01111 11101

    unsigned char ciphertext[3] = {0};
    unsigned char decryptedtext[3] = {0};

    // Encrypt plaintext
    ctr_encrypt(plaintext, ciphertext, key, counter, 3);
    print_binary("Ciphertext", ciphertext, 3);

    // Reset counter for decryption
    counter[0] = 0x00;

    // Decrypt ciphertext
    ctr_decrypt(ciphertext, decryptedtext, key, counter, 3);
    print_binary("Decrypted text", decryptedtext, 3);

    return 0;
}

