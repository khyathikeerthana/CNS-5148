#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/des.h>

void print_hex(const char *label, const unsigned char *s, size_t len) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", s[i]);
    }
    printf("\n");
}

void handleErrors(void)
{
    printf("An error occurred\n");
    abort();
}

void encrypt(const unsigned char *plaintext, int plaintext_len, const unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext)
{
    DES_cblock des_key1, des_key2, des_key3;
    DES_key_schedule ks1, ks2, ks3;
    
    // Split the 24-byte key into three 8-byte keys
    memcpy(des_key1, key, 8);
    memcpy(des_key2, key + 8, 8);
    memcpy(des_key3, key + 16, 8);
    
    // Initialize keys
    if (DES_set_key_checked(&des_key1, &ks1) != 0 ||
        DES_set_key_checked(&des_key2, &ks2) != 0 ||
        DES_set_key_checked(&des_key3, &ks3) != 0) {
        handleErrors();
    }
    
    DES_cblock iv_temp;
    memcpy(iv_temp, iv, 8);
    
    // Encrypt the plaintext using CBC mode
    DES_ede3_cbc_encrypt(plaintext, ciphertext, plaintext_len, &ks1, &ks2, &ks3, &iv_temp, DES_ENCRYPT);
}

int main()
{
    const unsigned char plaintext[] = "Hello, world!";
    const unsigned char key[24] = "123456789012345678901234"; // 24-byte key for 3DES
    unsigned char iv[8] = "12345678"; // Initialization Vector (IV)
    unsigned char ciphertext[64]; // Ensure this is large enough for the encrypted output
    
    // Encrypt plaintext
    encrypt(plaintext, strlen((const char *)plaintext), key, iv, ciphertext);
    
    // Print results
    print_hex("Plaintext", plaintext, strlen((const char *)plaintext));
    print_hex("Ciphertext", ciphertext, strlen((const char *)plaintext));
    
    return 0;
}

