#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_LENGTH 1024

// English letter frequencies (in percentage)
const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 
    2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};

// Function to decrypt ciphertext with a given key
void decrypt(const char* ciphertext, char* plaintext, int key) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((ciphertext[i] - 'a' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'a';
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - key + ALPHABET_SIZE) % ALPHABET_SIZE) + 'A';
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain the same
        }
    }
    plaintext[length] = '\0';
}

// Function to compute the frequency score of a plaintext
double compute_frequency_score(const char* plaintext) {
    int length = strlen(plaintext);
    int letter_count[ALPHABET_SIZE] = {0};
    double score = 0.0;

    // Count letter frequencies in the plaintext
    for (int i = 0; i < length; i++) {
        if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            letter_count[plaintext[i] - 'a']++;
        } else if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            letter_count[plaintext[i] - 'A']++;
        }
    }

    // Calculate the frequency score
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        double frequency = (double)letter_count[i] / length * 100.0;
        score += abs(frequency - english_freq[i]);
    }

    return score;
}

// Struct to hold decrypted text and its score
typedef struct {
    char text[MAX_TEXT_LENGTH];
    double score;
} DecryptionResult;

// Function to compare two decryption results (for qsort)
int compare_results(const void* a, const void* b) {
    double score_a = ((DecryptionResult*)a)->score;
    double score_b = ((DecryptionResult*)b)->score;
    return (score_a > score_b) - (score_a < score_b);
}

int main() {
    char ciphertext[MAX_TEXT_LENGTH];
    int top_n;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    printf("Enter the number of top plaintexts to display: ");
    scanf("%d", &top_n);

    DecryptionResult results[ALPHABET_SIZE];

    // Try all possible keys and store the decrypted text and its frequency score
    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decrypt(ciphertext, results[key].text, key);
        results[key].score = compute_frequency_score(results[key].text);
    }

    // Sort the results based on their frequency score
    qsort(results, ALPHABET_SIZE, sizeof(DecryptionResult), compare_results);

    // Display the top N plaintexts
    printf("\nTop %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("Key %2d: %s (Score: %.2f)\n", i, results[i].text, results[i].score);
    }

    return 0;
}

