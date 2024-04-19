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
    printf("H:        ");
    print_gf128(H);

    //
    gf128 Y;
    gf128 EKY, EKY0;
    zero_gf128(EKY);
    copy_gf128(Y, input.IV);

    Y[3]++;
    printf("Y0:       ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y0): ");
    print_gf128(EKY);
    copy_gf128(EKY0, EKY);

    

    Y[3]++;
    printf("Y1:       ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y1): ");
    print_gf128(EKY);

    gf128 X;
    copy_gf128(X, input.P[0]);
    xor_gf128(X, EKY);              // Plaintext 1 XOR EKY_1
    copy_gf128(output.C[0], X);
    

    Y[3]++;
    printf("Y2:       ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y2): ");
    print_gf128(EKY);

    copy_gf128(X, input.P[1]);
    xor_gf128(X, EKY);              // Plaintext 2 XOR EKY_2
    copy_gf128(output.C[1], X);

    Y[3]++;
    printf("Y3:       ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y3): ");
    print_gf128(EKY);

    copy_gf128(X, input.P[2]);
    xor_gf128(X, EKY);              // Plaintext 3 XOR EKY_3
    copy_gf128(output.C[2], X);

    Y[3]++;
    printf("Y4:       ");
    print_gf128(Y);
    copy_gf128(state, Y);
    encryptNoBullshit();
    copy_gf128(EKY, state);
    printf("E(K, Y4): ");
    print_gf128(EKY);

    copy_gf128(X, input.P[3]);
    xor_gf128(X, EKY);              // Plaintext 4 XOR EKY_4
    copy_gf128(output.C[3], X);

    for (int i = 0; i < 4; i++) {
        printf("C%d:       ", i + 1);
        print_gf128(output.C[i]);
    }


    // GHash?
    gf128 lenAC;
    lenAC[0] = 0;
    lenAC[1] = (uint32_t) input.ALength;
    lenAC[2] = 0;
    lenAC[3] = (uint32_t) input.PLength;

    gf128 multH, multHTemp;
    gf128_mul(input.A[0], H, multH);
    xor_gf128(multH, output.C[0]);
    copy_gf128(multHTemp, multH);
    gf128_mul(multHTemp,H, multH);
    printf("X1:       ");
    print_gf128(multH);

    xor_gf128(multH, output.C[1]);
    copy_gf128(multHTemp, multH);
    gf128_mul(multHTemp,H, multH);
    printf("X2:       ");
    print_gf128(multH);

    xor_gf128(multH, output.C[2]);
    copy_gf128(multHTemp, multH);
    gf128_mul(multHTemp,H, multH);
    printf("X3:       ");
    print_gf128(multH);

    xor_gf128(multH, output.C[3]);
    copy_gf128(multHTemp, multH);
    gf128_mul(multHTemp,H, multH);
    printf("X4:       ");
    print_gf128(multH);

    printf("len ac:   ");
    print_gf128(lenAC);

    xor_gf128(multH, lenAC);
    copy_gf128(multHTemp, multH);

    gf128_mul(multHTemp, H, multH);
    printf("GHASH:    ");
    print_gf128(multH);

    xor_gf128(multH, EKY0);

    copy_gf128(output.T, multH);
    printf("T:        ");
    print_gf128(output.T);
    


    








}
void GHASH(void);


void encryptNoBullshit() {
    ExpandKey1D(Key1D, ExpandedKey1D);
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