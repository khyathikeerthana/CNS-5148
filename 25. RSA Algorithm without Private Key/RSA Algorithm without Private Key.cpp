#include <stdio.h>

// Function to compute the greatest common divisor (GCD) using Euclidean algorithm
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
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

int main() {
    int n = 3599; // Given n
    int e = 31;   // Given public key e
    int m = 13;   // Assume we know a plaintext block m which has a common factor with n

    // Step 1: Find the common factor p
    int p = gcd(m, n);
    if (p == 1 || p == n) {
        printf("No useful common factor found.\n");
        return -1;
    }

    // Step 2: Compute q
    int q = n / p;
    printf("Factors of %d are p = %d and q = %d\n", n, p, q);

    // Step 3: Compute phi(n)
    int phi_n = (p - 1) * (q - 1);
    printf("phi(n) = %d\n", phi_n);

    // Step 4: Compute the private key d
    int d = mod_inverse(e, phi_n);
    if (d != -1) {
        printf("Private key d = %d\n", d);
    }

    return 0;
}

