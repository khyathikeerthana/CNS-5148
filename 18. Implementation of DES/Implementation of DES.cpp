#include <stdio.h>
#include <stdint.h>

// Initial permutation table for the key
static const int PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1,  58, 50, 42, 34, 26, 18,
    10, 2,  59, 51, 43, 35, 27,
    19, 11, 3,  60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7,  62, 54, 46, 38, 30, 22,
    14, 6,  61, 53, 45, 37, 29,
    21, 13, 5,  28, 20, 12, 4
};

// Number of shifts for each round of key schedule
static const int shifts[] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

// Final permutation table for generating subkeys
static const int PC2[] = {
    14, 17, 11, 24, 1,  5,
    3,  28, 15, 6,  21, 10,
    23, 19, 12, 4,  26, 8,
    16, 7,  27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Function to perform left circular shift on a 28-bit value
uint32_t leftCircularShift(uint32_t value, int shift) {
    return (value << shift) | (value >> (28 - shift));
}

// Function to generate 48-bit subkey from 56-bit key
void generateSubkey(uint64_t key, int round, uint64_t *subkey) {
    uint32_t C = (uint32_t)(key >> 28); // First 28 bits (upper half)
    uint32_t D = (uint32_t)(key & 0x0FFFFFFF); // Last 28 bits (lower half)

    // Perform left shifts according to the round number
    int shift = shifts[round - 1];
    C = leftCircularShift(C, shift);
    D = leftCircularShift(D, shift);

    // Combine C and D back into 56-bit key
    uint64_t CD = ((uint64_t)C << 28) | D;

    // Apply PC2 permutation to generate 48-bit subkey
    *subkey = 0;
    for (int i = 0; i < 48; ++i) {
        int bitPosition = PC2[i] - 1;
        *subkey |= ((CD >> (56 - bitPosition)) & 1) << (47 - i);
    }
}

int main() {
    uint64_t initialKey = 0x123456789ABC; // Example initial 56-bit key

    // Apply PC1 permutation to initial key
    uint64_t permutedKey = 0;
    for (int i = 0; i < 56; ++i) {
        int bitPosition = PC1[i] - 1;
        permutedKey |= ((initialKey >> (56 - bitPosition)) & 1) << (55 - i);
    }

    // Generate and print subkeys for each round
    uint64_t subkey;
    for (int round = 1; round <= 16; ++round) {
        generateSubkey(permutedKey, round, &subkey);
        printf("Round %2d subkey: 0x%012lX\n", round, subkey);
    }

    return 0;
}

