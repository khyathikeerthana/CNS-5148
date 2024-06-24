#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEN 1000

// Function to count the frequency of each character in the ciphertext
void frequencyAnalysis(char *ciphertext, int freq[]) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char c = ciphertext[i];
        if (c >= ' ' && c <= '~') {
            freq[c - ' ']++;
        }
    }
}

// Function to print the frequency of characters
void printFrequency(int freq[]) {
    for (int i = 0; i < 95; i++) {
        if (freq[i] > 0) {
            printf("%c: %d\n", i + ' ', freq[i]);
        }
    }
}

// Function to decrypt the ciphertext using the provided key
void decrypt(char *ciphertext, char *key) {
    char decrypted[MAX_LEN];
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        char c = ciphertext[i];
        if (c >= ' ' && c <= '~') {
            decrypted[i] = key[c - ' '];
        } else {
            decrypted[i] = c;
        }
    }
    decrypted[strlen(ciphertext)] = '\0';
    printf("Decrypted Message: %s\n", decrypted);
}

int main() {
    char ciphertext[] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48†8¶60))85;;]8*;:‡*8†83\n"
                        "(88)5*†;46(;88*96*?;8)*‡(;485);5*†2:*‡(;4956*2(5*—4)8¶8*\n"
                        ";4069285);)6†8)4‡‡;1(‡9;48081;8:8‡1;48†85;4)485†528806*81\n"
                        "(‡9;48;(88;4(‡?34;48)4‡;161;:188;‡?;";

    int freq[95] = {0};
    
    // Perform frequency analysis
    frequencyAnalysis(ciphertext, freq);
    
    // Print frequency of each character
    printf("Character frequencies:\n");
    printFrequency(freq);

    // Manual key for decryption (based on frequency analysis and educated guesses)
    // Adjust this key based on actual frequency analysis
    char key[95] = {
        '0', '1', 'e', '3', '4', 't', 'a', '7', '8', 'o', 'k', 'l', 'm', 'n', 'p', 'q', // 32-47
        'r', 's', 'd', 'u', 'v', 'i', 'w', 'x', 'y', 'z', 'h', 'f', 'g', 'b', 'c', 'j', // 48-63
        ' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', // 64-79
        '2', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', // 80-95
        '}', '~' // 96-97
    };

    // Decrypt the ciphertext using the key
    printf("\nDecrypted message:\n");
    decrypt(ciphertext, key);

    return 0;
}

