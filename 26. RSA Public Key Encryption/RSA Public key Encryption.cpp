#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to compute the greatest common divisor (GCD) using Euclidean algorithm
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to perform modular exponentiation
int mod_exp(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Extended Euclidean algorithm to find the modular inverse of e modulo phi_n
int extended_gcd(int a, int b, int* x, int* y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return a;
    }
    int x1, y1;
    int gcd = extended_gcd(b, a % b, &x1, &y1);
    *x = y1;
    *y = x1 - (a / b) * y1;
    return gcd;
}

// Function to compute the modular inverse of e modulo phi_n
int mod_inverse(int e, int phi_n) {
    int x, y;
    int g = extended_gcd(e, phi_n, &x, &y);
    if (g != 1) {
        printf("Inverse doesn't exist\n");
        return -1;
    } else {
        // Make x positive
        return (x % phi_n + phi_n) % phi_n;
    }
}

// Function to generate a random prime number
int generate_prime() {
    int primes[] = {101, 103, 107, 109, 113, 127, 131, 137, 139, 149};
    int size = sizeof(primes) / sizeof(primes[0]);
    srand(time(NULL));
    return primes[rand() % size];
}

int main() {
    // Step 1: Generate two large prime numbers p and q
    int p = generate_prime();
    int q = generate_prime();
    while (p == q) {
        q = generate_prime();
    }

    int n = p * q;
    int phi_n = (p - 1) * (q - 1);

    // Step 2: Choose a public key e such that 1 < e < phi(n) and gcd(e, phi(n)) = 1
    int e = 3;
    while (gcd(e, phi_n) != 1) {
        e += 2;
    }

    // Step 3: Compute the private key d such that (d * e) % phi(n) = 1
    int d = mod_inverse(e, phi_n);

    printf("Public Key (e, n): (%d, %d)\n", e, n);
    printf("Private Key (d, n): (%d, %d)\n", d, n);

    // Step 4: Encrypt a message
    int plaintext = 42; // Example plaintext
    int ciphertext = mod_exp(plaintext, e, n);
    printf("Encrypted message: %d\n", ciphertext);

    // Step 5: Decrypt the message
    int decryptedtext = mod_exp(ciphertext, d, n);
    printf("Decrypted message: %d\n", decryptedtext);

    return 0;
}

