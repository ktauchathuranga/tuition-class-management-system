#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>

void sha256(const char *str, char outputBuffer[65]) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int len = EVP_MAX_MD_SIZE;
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, str, strlen(str));
    EVP_DigestFinal_ex(mdctx, hash, &len);
    EVP_MD_CTX_free(mdctx);
    
    for(int i = 0; i < len; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int login() {
    char stored_username_hash[] = "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"; // a
    char stored_password_hash[] = "3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d"; // b

    char username[100];
    char password[100];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    char entered_username_hash[65];
    char entered_password_hash[65];
    sha256(username, entered_username_hash);
    sha256(password, entered_password_hash);

    if (strcmp(stored_username_hash, entered_username_hash) == 0 &&
        strcmp(stored_password_hash, entered_password_hash) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Login failed. Please check your username and password.\n");
        return 0;
    }

}
