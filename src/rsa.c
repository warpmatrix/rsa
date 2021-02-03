#include "rsa.h"

#include <stdint.h>
#include <time.h>

// bitLen should be 1024(default), 2048, 3072, etc.
int bitLen = 1024;

void genKeyConst(mpz_t n, mpz_t pubExp, mpz_t priExp);

void initKeys(Key *pubKey, Key *priKey) {
    mpz_inits(pubKey->n, pubKey->exp, priKey->n, priKey->exp, NULL);
    genKeyConst(priKey->n, pubKey->exp, priKey->exp);
    mpz_init_set(pubKey->n, priKey->n);
}

void clearKeys(Key *pubKey, Key *priKey) {
    mpz_clears(pubKey->n, pubKey->exp, priKey->n, priKey->exp, NULL);
}

mpz_t *encrypt(const void *msg, size_t msgSiz, const Key *key, mpz_t *ciph) {
    const uint8_t *msgPtr = msg;
    int ciphLen = getCiphLen(msgSiz);
    for (int i = 0; i < ciphLen; i++) {
        const uint8_t *ptr = msgPtr + i * (bitLen >> 3);
        mpz_t num;
        mpz_init(num);
        mpz_import(num, msgSiz, -1, sizeof(*ptr), 0, 0, ptr);
        mpz_powm(ciph[i], num, key->exp, key->n);
        mpz_clear(num);
    }
    return ciph;
}

size_t decrypt(const mpz_t *ciph, int ciphLen, const Key *key, void *msg) {
    uint8_t *msgPtr = msg;
    size_t size = 0;
    for (int i = 0; i < ciphLen; i++) {
        mpz_t num;
        mpz_init(num);
        mpz_powm(num, ciph[i], key->exp, key->n);
        uint8_t *ptr = msgPtr + i * (bitLen >> 3);
        size_t tmp;
        mpz_export(ptr, &tmp, -1, sizeof(*ptr), 0, 0, num);
        size += tmp;
        mpz_clear(num);
    }
    return size;
}

void genKeyConst(mpz_t n, mpz_t pubExp, mpz_t priExp) {
    mpz_set_ui(pubExp, 0x10001);
    gmp_randstate_t randSt;
    gmp_randinit_default(randSt);
    gmp_randseed_ui(randSt, time(NULL));

    mpz_t p, q;
    mpz_inits(p, q, NULL);
    do {
        mpz_rrandomb(p, randSt, bitLen / 2);
        mpz_rrandomb(q, randSt, bitLen / 2);
        mpz_nextprime(p, p);
        mpz_nextprime(q, q);

        mpz_t diff, abs;
        mpz_inits(diff, abs, NULL);
        mpz_sub(diff, p, q);
        mpz_abs(abs, diff);
        size_t log = mpz_sizeinbase(abs, 2);
        mpz_clears(diff, abs, NULL);
        if (!(log >= bitLen / 2 - 100 && log >= bitLen / 3 + 1)) continue;
        mpz_mul(n, p, q);
        if (!(mpz_sizeinbase(n, 2) == bitLen && mpz_popcount(n) > bitLen / 4)) continue;

        mpz_t phiN, tmpp, tmpq, tmpe;
        mpz_init(phiN);
        mpz_init_set(tmpp, p);
        mpz_init_set(tmpq, q);
        mpz_init_set(tmpe, pubExp);
        mpz_sub_ui(tmpp, tmpp, 1);
        mpz_sub_ui(tmpq, tmpq, 1);
        mpz_mul(phiN, tmpp, tmpq);
        mpz_invert(priExp, tmpe, phiN);
        mpz_clears(phiN, tmpp, tmpq, tmpe, NULL);
    } while (!(mpz_sizeinbase(priExp, 2) > bitLen / 2));
    gmp_randclear(randSt);
    mpz_clears(p, q, NULL);
}

inline int getCiphLen(size_t msgSiz) {
    // msgSiz * 8 / bitLen + 1
    return (msgSiz << 3) / bitLen + 1;
}

void initCiph(mpz_t *ciph, int ciphLen) {
    for (int i = 0; i < ciphLen; i++) {
        mpz_init(ciph[i]);
    }
}

void clearCiph(mpz_t *ciph, int ciphLen) {
    for (int i = 0; i < ciphLen; i++) {
        mpz_clear(ciph[i]);
    }
}
