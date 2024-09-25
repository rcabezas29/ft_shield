#include <ft_shield.h>

unsigned char PASSWORD_HASH[SHA256_DIGEST_LENGTH] = {
    0xc7, 0x93, 0xf8, 0xa0, 0x9d, 0xf6, 0x46, 0x5e,
    0x9f, 0xaf, 0x26, 0x25, 0xa9, 0x78, 0xaf, 0x7e,
    0x61, 0x79, 0x28, 0xc0, 0x19, 0x11, 0x14, 0x1a,
    0x52, 0x50, 0xdb, 0xe1, 0x62, 0xb2, 0x44, 0x69
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

int check_password(unsigned char *hash1) {
    return memcmp(hash1, PASSWORD_HASH, SHA256_DIGEST_LENGTH) == 0;
}

void print_hash(unsigned char *hash, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}
