#include "AESGCM.h"
#include "AES_128_1D.h"

uint32_t PlainText[4];
uint32_t Key1D[4];
uint32_t ExpandedKey1D[11][4];
uint32_t state[4]; // 1-D state array

void EncryptionGCM(AES_GCM_In input, AES_GCM_Out output){
    ExpandKey1D(Key1D, ExpandedKey1D);
    // Creates Hash Key
    gf128 H;
    copy_gf128(Key1D,input.K);
    zero_gf128(state);
    encryptNoBullshit();
    copy_gf128(H, state);
    printf("H = ");
    print_gf128(H);

    //
    gf128 Y;
    gf128 EKY;
    zero_gf128(EKY);
    copy_gf128(Y, input.IV);

    Y[3]++;
    printf("Y0 = ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y0) = ");
    print_gf128(EKY);
    

    Y[3]++;
    printf("Y1 = ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y1) = ");
    print_gf128(EKY);

    Y[3]++;
    printf("Y2 = ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y2) = ");
    print_gf128(EKY);

    Y[3]++;
    printf("Y3 = ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y3) = ");
    print_gf128(EKY);

    Y[3]++;
    printf("Y4 = ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y4) = ");
    print_gf128(EKY);


}
void GHASH(void);


void encryptNoBullshit() {
    AddRoundKey1D(ExpandedKey1D[0], state);
    
    for (int round = 1; round <= 10; round++) {
        SubBytesAndShiftRows1D(state); 
        if(round!=10) { 
            MixColumns1D(state);           
        }
        AddRoundKey1D(ExpandedKey1D[round], state); 
    }
    //print_gf128(state);
}