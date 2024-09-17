#include <ft_shield.h>

unsigned char PASSWORD_HASH[SHA256_DIGEST_LENGTH] = {
    0x1c, 0x8b, 0xfe, 0x8f, 0x80, 0x1d, 0x79, 0x74,
    0x5c, 0x46, 0x31, 0xd0, 0x9f, 0xff, 0x36, 0xc8,
    0x2a, 0xa3, 0x7f, 0xc4, 0xcc, 0xe4, 0xfc, 0x94,
    0x66, 0x83, 0xd7, 0xb3, 0x36, 0xb6, 0x30, 0x32
};

void hash_sha256(const char *str, unsigned char output[SHA256_DIGEST_LENGTH]) {
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    
    // Create and initialize the context
    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        perror("EVP_MD_CTX_new");
        return;
    }

    // Get the SHA256 digest method
    md = EVP_sha256();
    if (md == NULL) {
        perror("EVP_sha256");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Initialize the digest operation
    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1) {
        perror("EVP_DigestInit_ex");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Update the context with the input data
    if (EVP_DigestUpdate(mdctx, str, strlen(str)) != 1) {
        perror("EVP_DigestUpdate");
        EVP_MD_CTX_free(mdctx);
        return;
    }

    // Finalize the digest and get the result
    unsigned int length;
    if (EVP_DigestFinal_ex(mdctx, output, &length) != 1) {
        perror("EVP_DigestFinal_ex");
        EVP_MD_CTX_free(mdctx);
        return;
    }
    EVP_MD_CTX_free(mdctx);
}

int compare_hashes(unsigned char *hash1) {
    return memcmp(hash1, PASSWORD_HASH, SHA256_DIGEST_LENGTH) == 0;
}

// int main() {
//     char input[256];
//     printf("Enter password: ");
//     scanf("%255s", input);

//     unsigned char inputHash[SHA256_DIGEST_LENGTH];
//     hash_sha256(input, inputHash);

//     if (compare_hashes(inputHash)) {
//         printf("Password match.\n");
//     } else {
//         printf("Password does not match.\n");
//     }

//     return 0;
// }