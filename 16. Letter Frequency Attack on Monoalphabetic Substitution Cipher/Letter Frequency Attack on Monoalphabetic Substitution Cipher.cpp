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

// Function to calculate the frequency of each letter in the ciphertext
void calculate_frequency(const char* text, double* freq) {
    int length = strlen(text);
    int count[ALPHABET_SIZE] = {0};

    // Count the occurrences of each letter
    for (int i = 0; i < length; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            count[text[i] - 'a']++;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            count[text[i] - 'A']++;
        }
    }

    // Calculate the frequency of each letter
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (double)count[i] / length * 100.0;
    }
}

// Function to compute the score of a plaintext based on its letter frequency
double compute_frequency_score(const double* freq) {
    double score = 0.0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += abs(freq[i] - english_freq[i]);
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

// Function to decrypt the ciphertext using a given substitution key
void decrypt(const char* ciphertext, const char* key, char* plaintext) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = key[ciphertext[i] - 'a'];
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = key[ciphertext[i] - 'A'];
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters remain the same
        }
    }
    plaintext[length] = '\0';
}

// Function to generate a key based on letter frequency analysis
void generate_key(const double* freq, char* key) {
    int indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        indices[i] = i;
    }

    // Sort the indices based on frequency
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (freq[indices[i]] < freq[indices[j]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    // Map the sorted frequencies to the English letter frequencies
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        key[indices[i]] = 'a' + i;
    }
    key[ALPHABET_SIZE] = '\0';
}

int main() {
    char ciphertext[MAX_TEXT_LENGTH];
    int top_n;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT_LENGTH, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0'; // Remove newline character

    printf("Enter the number of top plaintexts to display: ");
    scanf("%d", &top_n);

    double freq[ALPHABET_SIZE];
    calculate_frequency(ciphertext, freq);

    DecryptionResult results[ALPHABET_SIZE];
    char key[ALPHABET_SIZE + 1];
    
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        generate_key(freq, key);
        decrypt(ciphertext, key, results[i].text);
        results[i].score = compute_frequency_score(freq);
        
        // Rotate frequencies for the next attempt
        double temp = freq[0];
        for (int j = 0; j < ALPHABET_SIZE - 1; j++) {
            freq[j] = freq[j + 1];
        }
        freq[ALPHABET_SIZE - 1] = temp;
    }

    // Sort the results based on their frequency score
    qsort(results, ALPHABET_SIZE, sizeof(DecryptionResult), compare_results);

    // Display the top N plaintexts
    printf("\nTop %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n; i++) {
        printf("Plaintext %2d: %s (Score: %.2f)\n", i + 1, results[i].text, results[i].score);
    }

    return 0;
}

