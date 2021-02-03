#include <stdio.h>

#include "rsa.h"

void printCiph(const mpz_t *ciph, int ciphLen);

int main(int argc, char const *argv[]) {
    char msg[] = "12345678";

    Key pub, pri;
    initKeys(&pub, &pri);
    int ciphLen = getCiphLen(sizeof(msg));
    mpz_t ciph[ciphLen];
    initCiph(ciph, ciphLen);

    encrypt(msg, sizeof(msg), &pri, ciph);
    printf("After encrypt:\n");
    printCiph(ciph, ciphLen);

    char deciph[sizeof(msg)];
    decrypt(ciph, ciphLen, &pub, &deciph);
    printf("After decrypt:\n");
    printf("%s\n", deciph);

    clearCiph(ciph, ciphLen);
    clearKeys(&pub, &pri);
    return 0;
}

void printCiph(const mpz_t *ciph, int ciphLen) {
    for (int i = 0; i < ciphLen; i++) {
        gmp_printf("ciph: %Zx\n", ciph[i]);
    }
    printf("\n");
}
