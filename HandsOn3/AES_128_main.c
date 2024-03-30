
#include <stdio.h>
#include <string.h>
#include <strings.h>


//#include "xparameters.h"
//#include "platform.h"

#include "AES_128.h"

//============================================================================

uint8_t StateArray [4][4];
uint8_t ExpandedKey[11][4][4];
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

	encryptNoBullshit();

	//decrypt();

	xil_printf("**********************************************\r\n");

	xil_printf("-- Exiting main() --\r\n");
	//cleanup_platform();

	return 0;
}

void encryptNoBullshit() {
	ExpandKey(Key, ExpandedKey);
	memcpy(StateArray, PlainText, 4 * 4 * sizeof(uint8_t));
	AddRoundKey(ExpandedKey[0], StateArray);
	for(int i=1; i<=10; i++){
		#ifdef SBOX_LUT
		SubBytes(StateArray);
		#else
		SubBytesCalculated(StateArray);
		#endif
		ShiftRows(StateArray);
		if(i!=10){
			MixColumns(StateArray);
		}
		AddRoundKey(ExpandedKey[i], StateArray);
	}
	AES_printf(StateArray);
}

void encrypt(){
#if (AES_PRINT & AES_PRINT_MAIN)
	xil_printf("-- Test Encryption Key \r\n\n");
	AES_printf(Key);
	xil_printf("---------------------\r\n\n");

	xil_printf("-- Test Plaintext \r\n\n");
	AES_printf(PlainText);
	xil_printf("---------------------\r\n\n");
#endif

#if (AES_PRINT & AES_PRINT_MAIN)
	xil_printf("-- Starting Key Expansion \r\n\n");
#endif

	ExpandKey(Key, ExpandedKey);

#if (AES_PRINT & AES_PRINT_MAIN)
	xil_printf("-- Starting Encryption \r\n\n");
#endif

	long int x;
	for(x=0; x<1; x++){
		memcpy(StateArray, PlainText, 4 * 4 * sizeof(uint8_t));

#if (AES_PRINT & AES_PRINT_DETAILS)
		xil_printf("-- Test State - Start of Round 0 \r\n\n");
		AES_printf(StateArray);
		xil_printf("---------------------\r\n\n");
#endif

		AddRoundKey(ExpandedKey[0], StateArray);
#if (AES_PRINT & AES_PRINT_DETAILS)
		xil_printf("-- Test State - End of Round 0 \r\n\n");
		AES_printf(StateArray);
		xil_printf("---------------------\r\n\n");
#endif

		int i;

		// Rounds
		for(i=1; i<=10; i++){
			SubBytes(StateArray);
//			/*
#if (AES_PRINT & AES_PRINT_DETAILS)
			xil_printf("-- Test State - Round %d after SubBytes \r\n\n",i);
			AES_printf(StateArray);
			xil_printf("---------------------\r\n\n");
#endif
//*/
			ShiftRows(StateArray);
#if (AES_PRINT & AES_PRINT_DETAILS)
			xil_printf("-- Test State - Round %d after ShiftRows \r\n\n",i);
			AES_printf(StateArray);
			xil_printf("---------------------\r\n\n");
#endif


			if(i!=10){
				MixColumns(StateArray);
				 
#if (AES_PRINT & AES_PRINT_DETAILS)
				xil_printf("-- Test State - Round %d after MixColumns \r\n\n",i);
				AES_printf(StateArray);
				xil_printf("---------------------\r\n\n");
#endif

			}

			AddRoundKey(ExpandedKey[i], StateArray);
			
#if (AES_PRINT & AES_PRINT_DETAILS)
			xil_printf("-- Test State - End of Round %d \r\n\n",i);
			AES_printf(StateArray);
			xil_printf("---------------------\r\n\n");
#endif

		}
	}
#if (AES_PRINT & AES_PRINT_MAIN)
	xil_printf("-- AES key expansion and encryption test completed. \r\n\n");
	xil_printf("-- Test State - End \r\n\n");
	AES_printf(StateArray);
	xil_printf("--------------------\r\n\n");
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
