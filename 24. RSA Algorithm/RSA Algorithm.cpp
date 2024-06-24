#include <stdio.h>

// Function to perform trial and error to find factors of n
void find_factors(int n, int* p, int* q) {
    for (int i = 2; i < n; i++) {
        if (n % i == 0) {
            *p = i;
            *q = n / i;
            return;
        }
    }
}

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
    int e = 31;
    int n = 3599;
    int p = 0, q = 0;

    // Step 1: Find p and q
    find_factors(n, &p, &q);
    printf("Factors of %d are p = %d and q = %d\n", n, p, q);

    // Step 2: Compute phi(n)
    int phi_n = (p - 1) * (q - 1);
    printf("phi(n) = %d\n", phi_n);

    // Step 3: Compute the private key d
    int d = mod_inverse(e, phi_n);
    printf("Private key d = %d\n", d);

    return 0;
}

