#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gf2128.h"

typedef struct AES_GCM_In {
    uint32_t K[4];    // Key
    uint16_t KLength;

    uint32_t IV[4];   // Initialization Vector
    uint16_t IVLength;  // Usually 96 bits

    uint32_t P[4][4];    // Plain text
    uint16_t PLength;

    uint32_t A[4][4];    // AAD
    uint16_t ALength;
} AES_GCM_In;

typedef struct AES_GCM_Out {
    uint32_t C[4][4];    // Cipher Text
    uint16_t CLength;   // Equal to PLength

    uint32_t T[4];    // Tag
    uint16_t TLength;   // Between 64 and 128 bits, denoted as 't'
} AES_GCM_Out;

void EncryptionGCM(AES_GCM_In input, AES_GCM_Out output);
void GHASH(void);
void CreateHashKey(void);

void encryptNoBullshit(void);



