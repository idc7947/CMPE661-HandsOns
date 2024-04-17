#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "gf2128.h"

typedef struct AES_GCM_In {
    uint8_t K[256];    // Key
    uint8_t KLength;

    uint8_t IV[256];   // Initialization Vector
    uint8_t IVLength;  // Usually 96 bits

    uint8_t P[256];    // Plain text
    uint8_t PLength;

    uint8_t A[256];    // AAD
    uint8_t ALength;
} AES_GCM_In;

typedef struct AES_GCM_Out {
    uint8_t C[256];    // Cipher Text
    uint8_t CLength;   // Equal to PLength

    uint8_t T[256];    // Tag
    uint8_t TLength;   // Between 64 and 128 bits, denoted as 't'
} AES_GCM_Out;

void EncryptionGCM(AES_GCM_In input, AES_GCM_Out output);
void GHASH(void);
void CreateHashKey(void);



