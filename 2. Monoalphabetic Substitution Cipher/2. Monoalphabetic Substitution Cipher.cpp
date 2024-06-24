#include <stdio.h>
#include <ctype.h>

char encryptDecrypt(char ch, char key[], int mode) {
  // Check for uppercase and lowercase
  if (isupper(ch)) {
    ch = tolower(ch);
  }
  // Handle non-alphabetic characters
  if (!isalpha(ch)) {
    return ch;
  }
  int keyIndex = ch - 'a';
  int newIndex = (keyIndex + key[keyIndex] - 'a') % 26;
  return (mode == 1) ? newIndex + 'a' : (newIndex + 'A'); // Switch based on mode (1 for encrypt, 0 for decrypt)
}

int main() {
  char key[26];
  char message[100];
  int mode;

  // Get key
  printf("Enter key (26 unique characters): ");
  scanf("%s", key);

  // Get message
  printf("Enter message: ");
  scanf(" %[^\n]", message); // Capture entire line using [^\n]

  // Get mode (encrypt or decrypt)
  printf("Enter mode (1 for encrypt, 0 for decrypt): ");
  scanf("%d", &mode);

  // Process message
  for (int i = 0; message[i] != '\0'; i++) {
    message[i] = encryptDecrypt(message[i], key, mode);
  }

  printf("Processed message: %s\n", message);

  return 0;
}

