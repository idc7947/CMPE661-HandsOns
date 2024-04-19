#include "AESGCM.h"
#include "AES_128_1D.h"

uint32_t PlainText[4];
uint32_t Key1D[4];
uint32_t ExpandedKey1D[11][4];
uint32_t state[4]; // 1-D state array
gf128 H, X, Y, EKY, EKY0, lenAC, multH, multHTemp;
int i, xcount;

void EncryptionGCM(AES_GCM_In input, AES_GCM_Out output){
    // Initialize lenAC
    lenAC[0] = 0;
    lenAC[1] = (uint32_t) input.ALength;
    lenAC[2] = 0;
    lenAC[3] = (uint32_t) input.PLength;

    // Print starting variables
    printf("K:        ");
    print_gf128(input.K);
    for (i = 0; i < 4; i++) {
        printf("P%d:       ", i);
        print_gf128(input.P[i]);
    }
    for (i = 0; i <= (input.ALength / 128); i++) {
        printf("A:        ");
        print_gf128(input.A[i]);
    }
    printf("IV:       ");
    print_gf128(input.IV);



    // Creates Hash Key
    copy_gf128(Key1D,input.K);
    zero_gf128(state);
    encryptNoBullshit();
    copy_gf128(H, state);
    printf("H:        ");
    print_gf128(H);

    // GCTR Init Y0 EKY0
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

    // If auth, make initial Xs
    xcount = 1;
    if (input.ALength) {
        gf128_mul(input.A[0], H, multH);
        printf("X%d:       ", xcount++);
        print_gf128(multH);
        for (i = 1; i <= (input.ALength / 128); i++) {
            xor_gf128(multH, input.A[i]);
            copy_gf128(multHTemp, multH);
            gf128_mul(multHTemp, H, multH);
            printf("X%d:     ", xcount++);
            print_gf128(multH);
        }
    }

    
    // GCTR
    for (i = 0; i < 4; i++) {
        Y[3]++;
        printf("Y%d:       ", i + 1);
        print_gf128(Y);
        copy_gf128(state, Y);
        encryptNoBullshit();
        copy_gf128(EKY, state);
        printf("E(K, Y%d): ", i + 1);
        print_gf128(EKY);
        copy_gf128(X, input.P[i]);
        xor_gf128(X, EKY);              
        copy_gf128(output.C[i], X);
        if ((i == 3) && (input.ALength)) output.C[i][3] = 0; // Cheeky...
    }



    // GHash continues from above if an Auth was present
    if (!(input.ALength)) gf128_mul(input.A[0], H, multH); // Do this if not done already
    for (i = 0; i < 4; i++) {
        xor_gf128(multH, output.C[i]);
        copy_gf128(multHTemp, multH);
        gf128_mul(multHTemp,H, multH);
        printf("X%d:       ", xcount++);
        print_gf128(multH);
    }

    printf("len ac:   ");
    print_gf128(lenAC);

    xor_gf128(multH, lenAC);
    copy_gf128(multHTemp, multH);

    gf128_mul(multHTemp, H, multH);
    printf("GHASH:    ");
    print_gf128(multH);

    for (i = 0; i < 4; i++) {
        printf("C%d:       ", i + 1);
        print_gf128(output.C[i]);
    }

    xor_gf128(multH, EKY0);

    copy_gf128(output.T, multH);
    printf("T:        ");
    print_gf128(output.T);

}


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