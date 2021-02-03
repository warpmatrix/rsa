#if !defined(RSA_H)
#define RSA_H

#include <gmp.h>
#include <stddef.h>

// the length of bigInt's bit
extern int bitLen;

typedef struct Key {
    mpz_t n, exp;
} Key;

void initKeys(Key *pubKey, Key *priKey);
void clearKeys(Key *pubKey, Key *priKey);

int getCiphLen(size_t msgSiz);
void initCiph(mpz_t *ciph, int ciphLen);
void clearCiph(mpz_t *ciph, int ciphLen);

mpz_t *encrypt(const void *msg, size_t msgSiz, const Key *key, mpz_t *ciph);
size_t decrypt(const mpz_t *ciph, int ciphLen, const Key *key, void *msg);

#endif  // RSA_H
