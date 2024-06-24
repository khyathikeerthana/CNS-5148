#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to calculate the GCD of two numbers
int CalcGCD(int a, int b) {
    if (b == 0)
        return a;
    else
        return CalcGCD(b, a % b);
}

// Function to check if a number is coprime with 26
int IsCoprime(int a) {
    return CalcGCD(a, 26) == 1;
}

// Function to encrypt a message using the Affine Cipher
void EncryptMessage(char *message, int a, int b) {
    int i;
    char *encrypted = (char *)malloc(strlen(message) + 1);

    for (i = 0; i < strlen(message); i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            encrypted[i] = (char)(((a * (message[i] - 'A')) + b) % 26 + 'A');
        } else if (message[i] >= 'a' && message[i] <= 'z') {
            encrypted[i] = (char)(((a * (message[i] - 'a')) + b) % 26 + 'a');
        } else {
            encrypted[i] = message[i];
        }
    }
    encrypted[i] = '\0';

    printf("Encrypted Message: %s\n", encrypted);

    free(encrypted);
}

// Function to decrypt a message using the Affine Cipher
void DecryptMessage(char *message, int a, int b) {
    int i;
    char *decrypted = (char *)malloc(strlen(message) + 1);

    for (i = 0; i < strlen(message); i++) {
        if (message[i] >= 'A' && message[i] <= 'Z') {
            decrypted[i] = (char)(((26 + (a * (message[i] - 'A' - b))) % 26) + 'A');
        } else if (message[i] >= 'a' && message[i] <= 'z') {
            decrypted[i] = (char)(((26 + (a * (message[i] - 'a' - b))) % 26) + 'a');
        } else {
            decrypted[i] = message[i];
        }
    }
    decrypted[i] = '\0';

    printf("Decrypted Message: %s\n", decrypted);

    free(decrypted);
}

int main() {
    char message[100];
    int a, b;

    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);

    printf("Enter a (must be coprime with 26): ");
    scanf("%d", &a);

    while (!IsCoprime(a)) {
        printf("a must be coprime with 26. Enter a new value: ");
        scanf("%d", &a);
    }

    printf("Enter b: ");
    scanf("%d", &b);

    EncryptMessage(message, a, b);
    DecryptMessage(message, a, b);

    return 0;
}
