
#include <stdio.h>
#include <string.h>
#include <strings.h>


//#include "xparameters.h"
//#include "platform.h"

#include "AES_128_1D.h"

//============================================================================
uint32_t state[4]; // 1-D state array
uint32_t Key1D[4];
uint8_t StateArray [4][4];
uint8_t ExpandedKey[11][4][4];
uint32_t ExpandedKey1D[11][4];
//    W0    W1    W2    W3
uint8_t Key[4][4]        = {  {0x2b, 0x28, 0xab, 0x09},
		{0x7e, 0xae, 0xf7, 0xcf},
		{0x15, 0xd2, 0x15, 0x4f},
		{0x16, 0xa6, 0x88, 0x3c} };

//    W0    W1    W2    W3
uint8_t PlainText[4][4]  = {  {0x32, 0x88, 0x31, 0xe0},
		{0x43, 0x5a, 0x31, 0x37},
		{0xf6, 0x30, 0x98, 0x07},
		{0xa8, 0x8d, 0xa2, 0x34} };
// To set all the bytes in a block of memory to a particular value, use memset().
// The function prototype is void * memset(void *dest, int c, size_t count);

void encrypt();
void encryptNoBullshit();
void decrypt();

int main()
{
	//init_platform();

	xil_printf("\n\n\n");
	xil_printf("-- Starting AES software test based on FIPS-197 (Appendix B)\r\n\n");

	//--------------------------------------------
	//-----Encrypt Function-----------------------
	//--------------------------------------------

	encryptNoBullshit();

	//--------------------------------------------
	//-----Decrypt Function-----------------------
	//--------------------------------------------

	//decrypt();

	//--------------------------------------------
	//-----Display Results------------------------
	//--------------------------------------------

	xil_printf("**********************************************\r\n");

	xil_printf("-- Exiting main() --\r\n");
	//cleanup_platform();

	return 0;
}

void encryptNoBullshit() {
    memcpy(StateArray, PlainText, 4 * 4 * sizeof(uint8_t));
    collapse_to_1D(Key, Key1D);
    collapse_to_1D(StateArray, state);
    ExpandKey1D(Key1D, ExpandedKey1D);
    AddRoundKey1D(ExpandedKey1D[0], state);
    
    for (int round = 1; round <= 10; round++) {
        SubBytesAndShiftRows1D(state); 
        if(round!=10) { 
            MixColumns1D(state);           
        }
        AddRoundKey1D(ExpandedKey1D[round], state); 
    }
    expand_to_2D(state, StateArray);
    AES_printf(StateArray);
}


void encrypt() {
	uint32_t state[4]; // 1-D state array
	uint32_t Key1D[4];

    long int x;
	for(x=0; x<1; x++){
		memcpy(StateArray, PlainText, 4 * 4 * sizeof(uint8_t));
    }
    collapse_to_1D(StateArray, state);
	// Same for the key
	collapse_to_1D(Key, Key1D);

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- Test Encryption Key \r\n\n");
    AES_printf(Key);
    xil_printf("---------------------\r\n\n");

    xil_printf("-- Test Plaintext \r\n\n");
    AES_printf(PlainText);
    xil_printf("---------------------\r\n\n");
#endif

    ExpandKey1D(Key1D, ExpandedKey1D);
	

    // Initial round key addition
    AddRoundKey1D(ExpandedKey1D[0], state);

	#if (AES_PRINT & AES_PRINT_DETAILS)
		xil_printf("-- Test State - End of Round 0 \r\n\n");
		expand_to_2D(state, StateArray);
		AES_printf(StateArray);
		xil_printf("---------------------\r\n\n");
#endif

    // Main rounds
    for (int round = 1; round <= 10; round++) {
        SubBytesAndShiftRows1D(state); // Perform SubBytes and ShiftRows
#if (AES_PRINT & AES_PRINT_DETAILS)
			xil_printf("-- Test State - Round %d after ShiftRows \r\n\n",round);
			expand_to_2D(state, StateArray);
			AES_printf(StateArray);
			xil_printf("---------------------\r\n\n");
#endif
        
        if(round!=10) { 
            MixColumns1D(state);           // Perform MixColumns
#if (AES_PRINT & AES_PRINT_DETAILS)
			xil_printf("-- Test State - Round %d after MixColumns \r\n\n",round);
			expand_to_2D(state, StateArray);
			AES_printf(StateArray);
			xil_printf("---------------------\r\n\n");
#endif
        }
        AddRoundKey1D(ExpandedKey1D[round], state); // AddRoundKey
    }

    expand_to_2D(state, StateArray);

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- Encryption Complete \r\n\n");
    AES_printf(StateArray); // Print the encrypted state
    xil_printf("---------------------\r\n\n");
#endif
}



void decrypt() {
	uint8_t Ciphertext[4][4];
	memcpy(Ciphertext, StateArray, 4 * 4 * sizeof(uint8_t));
#if (AES_PRINT & AES_PRINT_MAIN)
	xil_printf("-- Starting Decryption \r\n\n");
#endif

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- Starting Decryption \r\n\n");
#endif

    // Assuming ExpandedKey has been previously calculated and is available
    // If not, you'll need to call ExpandKey(Key, ExpandedKey); here as well

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- Starting Key Expansion for Decryption \r\n\n");
#endif

    // No need to expand the key again if already expanded for encryption

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- Starting Decryption \r\n\n");
#endif

    memcpy(StateArray, Ciphertext, 4 * 4 * sizeof(uint8_t));

    // Initial round
    AddRoundKey(ExpandedKey[10], StateArray);

    int i;

    // Rounds
    for(i = 9; i >= 0; i--){
        if (i != 9) {
#if (AES_PRINT & AES_PRINT_DETAILS)
            xil_printf("-- Test State - Start of Round %d \r\n\n", 10 - i);
            AES_printf(StateArray);
            xil_printf("---------------------\r\n\n");
#endif
            InvMixColumns(StateArray);
        }

#if (AES_PRINT & AES_PRINT_DETAILS)
        xil_printf("-- Test State - Round %d after InvMixColumns \r\n\n", 10 - i);
        AES_printf(StateArray);
        xil_printf("---------------------\r\n\n");
#endif

        InvShiftRows(StateArray);

#if (AES_PRINT & AES_PRINT_DETAILS)
        xil_printf("-- Test State - Round %d after InvShiftRows \r\n\n", 10 - i);
        AES_printf(StateArray);
        xil_printf("---------------------\r\n\n");
#endif

        InvSubBytes(StateArray);

#if (AES_PRINT & AES_PRINT_DETAILS)
        xil_printf("-- Test State - Round %d after InvSubBytes \r\n\n", 10 - i);
        AES_printf(StateArray);
        xil_printf("---------------------\r\n\n");
#endif

        AddRoundKey(ExpandedKey[i], StateArray);

#if (AES_PRINT & AES_PRINT_DETAILS)
        xil_printf("-- Test State - End of Round %d \r\n\n", 10 - i);
        AES_printf(StateArray);
        xil_printf("---------------------\r\n\n");
#endif
    }

#if (AES_PRINT & AES_PRINT_MAIN)
    xil_printf("-- AES key expansion and decryption test completed. \r\n\n");
    xil_printf("-- Test State - End \r\n\n");
    AES_printf(StateArray);
    xil_printf("---------------------\r\n\n");
#endif
}
