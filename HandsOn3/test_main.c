#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include "gf28.h"
#include "multiplication_tables.c"


#define first(x)  ((uint32_t)(x) >> 24)
#define second(x) ((uint32_t)(x) >> 16 & 0xFF)
#define third(x)  ((uint32_t)(x) >> 8 & 0xFF)
#define fourth(x) ((uint32_t)(x) & 0xFF)

#define toFirst(x)  (((uint32_t)(x)) << 24)
#define toSecond(x) (((uint32_t)(x) & 0xFF) << 16)
#define toThird(x)  (((uint32_t)(x) & 0xFF) << 8)
#define toFourth(x) ((uint32_t)(x) & 0xFF)

const uint32_t SBox[256] = {
 // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,   //0
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,   //1
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,   //2
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,   //3
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,   //4
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,   //5
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,   //6
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,   //7
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,   //8
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,   //9
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,   //A
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,   //B
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,   //C
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,   //D
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,   //E
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }; //F

 const uint32_t RCon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};


void ExpandKey (uint8_t Key[][4], uint8_t ExpandedKey[][4][4]);
void AddRoundKey (uint8_t Key[][4], uint8_t StateArray[][4]);


void SubBytes (uint8_t StateArray[][4]);
void ShiftRows (uint8_t StateArray[][4]);
void MixColumns (uint8_t StateArray[][4]);

void SubBytesCalculated (uint8_t StateArray[][4]);
uint8_t affine_transform(uint8_t bi);
 
void InvSubBytes (uint8_t StateArray[][4]);
void InvShiftRows (uint8_t StateArray[][4]);
void InvMixColumns (uint8_t StateArray[][4]);

void AES_printf (uint8_t StateArray[][4]);

void ExpandKey1D(uint32_t Key[4], uint32_t ExpandedKey[][4]);
void AddRoundKey1D(uint32_t Key[4], uint32_t StateArray[4]);


void collapse_to_1D(uint8_t src[4][4], uint32_t dst[4]);
void SubBytesAndShiftRows1D(uint32_t *state);
void MixColumns1D(uint32_t *state);
void expand_to_2D(uint32_t src[4], uint8_t dst[4][4]);

void PrintExpandedKey2D(uint8_t ExpandedKey[][4][4]) {
    for (int round = 0; round < 11; round++) {
        printf("Round %d Key:\n", round);
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                printf("%02x ", ExpandedKey[round][row][col]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void PrintExpandedKey1D(uint32_t ExpandedKey[][4]) {
    for (int round = 0; round < 11; round++) {
        printf("Round %d Key:\n", round);
        for (int col = 0; col < 4; col++) {
            // Assuming little endian, print bytes in reverse order for clarity
            for (int byte = 0; byte < 4; byte++) {
                printf("%02x ", (ExpandedKey[round][col] >> (8 * byte)) & 0xff);
            }
        }
        printf("\n");
    }
}

uint8_t StateArray [4][4];
uint32_t StateArray1D [4];
uint8_t ExpandedKey[11][4][4];
uint32_t ExpandedKey1D[11][4];
//    W0    W1    W2    W3
uint8_t Key[4][4]        = {  {0x2b, 0x28, 0xab, 0x09},
		{0x7e, 0xae, 0xf7, 0xcf},
		{0x15, 0xd2, 0x15, 0x4f},
		{0x16, 0xa6, 0x88, 0x3c} };
uint32_t Key1D[4];

//    W0    W1    W2    W3
uint8_t PlainText[4][4]  = {  {0x32, 0x88, 0x31, 0xe0},
		{0x43, 0x5a, 0x31, 0x37},
		{0xf6, 0x30, 0x98, 0x07},
		{0xa8, 0x8d, 0xa2, 0x34} };
// To set all the bytes in a block of memory to a particular value, use memset().
// The function prototype is void * memset(void *dest, int c, size_t count);



int main()
{   
    /*
    memcpy(StateArray, PlainText, 4 * 4 * sizeof(uint8_t));
    
    collapse_to_1D(Key, Key1D);
    collapse_to_1D(StateArray, StateArray1D);

    ExpandKey(Key, ExpandedKey);
    ExpandKey1D(Key1D, ExpandedKey1D);

    for (int i = 0; i < 10; i++) {
        AddRoundKey(ExpandedKey[i], StateArray);
        AddRoundKey1D(ExpandedKey1D[i], StateArray1D);
        AES_printf(StateArray);
        expand_to_2D(StateArray1D, StateArray);
        AES_printf(StateArray);
    }
    */
    for (uint8_t i = 0; i < 255; i++) {
        printf("LUT = 0x%02x, Affine = 0x%02x \r\n", SBox[i], affine_transform(i) );
    }
        //printf("0x%02x, ", gf_mult(0x03,255));

    /*
    SubBytesAndShiftRows1D(StateArray1D);

    SubBytes(StateArray);
    ShiftRows(StateArray);

    //MixColumns1D(StateArray1D);
    //MixColumns(StateArray);

    AES_printf(StateArray);
    expand_to_2D(StateArray1D, StateArray);
    AES_printf(StateArray);
*/
    
/*
    //AddRoundKey(Key, StateArray);
    //AddRoundKey1D(Key1D, StateArray1D);
    PrintExpandedKey2D(ExpandedKey);
    expand_to_2D(ExpandedKey1D, ExpandedKey);
    PrintExpandedKey2D(ExpandedKey);
*/
    //PrintExpandedKey1D(ExpandedKey1D);
    //PrintExpandedKey2D(ExpandedKey);



    
    //AES_printf(StateArray);
    //expand_to_2D(StateArray1D, StateArray);
    //AES_printf(StateArray);



    
	

	return 0;
}

void SubBytesAndShiftRows1D(uint32_t *state) {
    uint32_t temp[4];

    // Apply SubBytes and ShiftRows for each row
    for (int i = 0; i < 4; i++) {
        temp[i] = toFirst(SBox[first(state[i])]) |
                  toSecond(SBox[second(state[(i + 1) % 4])]) |
                  toThird(SBox[third(state[(i + 2) % 4])]) |
                  toFourth(SBox[fourth(state[(i + 3) % 4])]);
    }
    
    // Copy the temporary state back to the original state
    for (int i = 0; i < 4; i++) {
        state[i] = temp[i];
    }
}

void SubBytes (uint8_t StateArray[][4])
{
	int i,j;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			StateArray[i][j] = SBox[StateArray[i][j]];
}


void ShiftRows (uint8_t StateArray[][4])
{
	uint8_t x;
	// Row#1 - rotate 1 column to the left
	x = StateArray[1][0];
	StateArray[1][0] = StateArray[1][1];
	StateArray[1][1] = StateArray[1][2];
	StateArray[1][2] = StateArray[1][3];
	StateArray[1][3] = x;
	// Row#2 - rotate 2 column to the left
	x = StateArray[2][0];
	StateArray[2][0] = StateArray[2][2];
	StateArray[2][2] = x;
	x = StateArray[2][1];
	StateArray[2][1] = StateArray[2][3];
	StateArray[2][3] = x;
	// Row#3 - rotate 3 column to the left
	x = StateArray[3][3];
	StateArray[3][3] = StateArray[3][2];
	StateArray[3][2] = StateArray[3][1];
	StateArray[3][1] = StateArray[3][0];
	StateArray[3][0] = x;
}

void collapse_to_1D(uint8_t src[4][4], uint32_t dst[4]) {
    for (int i = 0; i < 4; i++) {
        dst[i] = 0;
        dst[i] |= src[0][i] << 24;
        dst[i] |= src[1][i] << 16;
        dst[i] |= src[2][i] << 8;
        dst[i] |= src[3][i];
    }
}

void expand_to_2D(uint32_t src[4], uint8_t dst[4][4]) {
    for (int i = 0; i < 4; i++) {
        dst[0][i] = (src[i] >> 24) & 0xFF;
        dst[1][i] = (src[i] >> 16) & 0xFF;
        dst[2][i] = (src[i] >> 8) & 0xFF;
        dst[3][i] = src[i] & 0xFF;
    }
}

uint8_t affine_transform(uint8_t bi) {
    uint8_t bo = gf_inv(bi); // Perform the multiplicative inverse.
    uint8_t bits[8];
    uint8_t bo_prime = 0;
    uint8_t c[8] = {1, 1, 0, 0, 0, 1, 1, 0}; // Constant vector c

    for (int i = 0; i < 8; ++i) {
        bits[i] = (bo >> i) & 0x01;
    }
    bo_prime |= bits[0] ^ bits[4] ^ bits[5] ^ bits[6] ^ bits[7] ^ c[0];
    bo_prime |= (bits[1] ^ bits[5] ^ bits[6] ^ bits[7] ^ bits[0] ^ c[1]) << 1;
    bo_prime |= (bits[2] ^ bits[6] ^ bits[7] ^ bits[0] ^ bits[1] ^ c[2]) << 2;
    bo_prime |= (bits[3] ^ bits[7] ^ bits[0] ^ bits[1] ^ bits[2] ^ c[3]) << 3;
    bo_prime |= (bits[4] ^ bits[0] ^ bits[1] ^ bits[2] ^ bits[3] ^ c[4]) << 4;
    bo_prime |= (bits[5] ^ bits[1] ^ bits[2] ^ bits[3] ^ bits[4] ^ c[5]) << 5;
    bo_prime |= (bits[6] ^ bits[2] ^ bits[3] ^ bits[4] ^ bits[5] ^ c[6]) << 6;
    bo_prime |= (bits[7] ^ bits[3] ^ bits[4] ^ bits[5] ^ bits[6] ^ c[7]) << 7;

    return bo_prime;
}


void AES_printf (uint8_t AES_StateArray[][4])
{
	int i;
	printf("   W0  W1  W2  W3\r\n\n");
	for(i=0; i<4; i++)
		printf("   %02x  %02x  %02x  %02x\r\n",
				AES_StateArray[i][0], AES_StateArray[i][1],
				AES_StateArray[i][2], AES_StateArray[i][3]);
	printf("\n");
}

void AddRoundKey (uint8_t Key[][4], uint8_t StateArray[][4])
{
	int i,j;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			StateArray[i][j] ^= Key[i][j];
}

void AddRoundKey1D(uint32_t Key[4], uint32_t StateArray[4]) {
    for (int i = 0; i < 4; i++) {
        StateArray[i] ^= Key[i];
    }
}

/**********************************************************************
 * Functions for key expansion
 *********************************************************************/
void ExpandKey (uint8_t Key[][4], uint8_t ExpandedKey[][4][4])
{
	uint32_t TempKey[4][4];
	memset(&TempKey, 0, 4*4*sizeof (uint32_t));
	//bzero(TempKey, 4*4*sizeof (uint32_t));
	uint32_t TempKeyCol[4];
	memset(&TempKeyCol, 0, 4*sizeof (uint32_t));
	//bzero(TempKeyCol, 4*sizeof (uint32_t));
	int i,j;

	// Encryption Key copied to Expanded Key [0]
	memcpy(ExpandedKey[0], Key, 4 * 4 * sizeof(uint32_t));

	for (i=1; i<11; i++){
		// W3 copied to TempKeyRow with rotation
		TempKeyCol[0]=ExpandedKey[i-1][1][3];
		TempKeyCol[1]=ExpandedKey[i-1][2][3];
		TempKeyCol[2]=ExpandedKey[i-1][3][3];
		TempKeyCol[3]=ExpandedKey[i-1][0][3];

		// sBox applied
		TempKeyCol[0]=SBox[ TempKeyCol[0] ];
		TempKeyCol[1]=SBox[ TempKeyCol[1] ];
		TempKeyCol[2]=SBox[ TempKeyCol[2] ];
		TempKeyCol[3]=SBox[ TempKeyCol[3] ];

		// Rcon applied
		TempKeyCol[0]^=RCon[i-1];

		// XOR
		for(j=0; j<4; j++){
			TempKeyCol[0] = TempKeyCol[0]^ExpandedKey[i-1][0][j];
			TempKeyCol[1] = TempKeyCol[1]^ExpandedKey[i-1][1][j];
			TempKeyCol[2] = TempKeyCol[2]^ExpandedKey[i-1][2][j];
			TempKeyCol[3] = TempKeyCol[3]^ExpandedKey[i-1][3][j];

			ExpandedKey[i][0][j] = TempKeyCol[0];
			ExpandedKey[i][1][j] = TempKeyCol[1];
			ExpandedKey[i][2][j] = TempKeyCol[2];
			ExpandedKey[i][3][j] = TempKeyCol[3];
		}
	}
}

void ExpandKey1D(uint32_t Key[4], uint32_t ExpandedKey[11][4]) {
    int i, j;
    uint32_t temp, rotated, subWord;

    // Copy the original key
    for (i = 0; i < 4; ++i) {
        ExpandedKey[0][i] = Key[i];
    }

    for (i = 1; i < 11; ++i) { // For each of the 11 sets of 4 words (for AES-128)
        // Start by dealing with the first word of each set specially
        temp = ExpandedKey[i-1][3]; // Last word from the previous block

        // Rotate the word
        rotated = (temp << 8) | (temp >> 24);

        // SubWord using SBox
        subWord = (uint32_t)(SBox[(rotated >> 24) & 0xFF] << 24) |
                  (uint32_t)(SBox[(rotated >> 16) & 0xFF] << 16) |
                  (uint32_t)(SBox[(rotated >> 8) & 0xFF] << 8) |
                  (uint32_t)(SBox[rotated & 0xFF]);

        // XOR with Rcon[i/Nk] (considering Nk=4 for AES-128)
        temp = subWord ^ (((uint32_t)RCon[i-1]) << 24);

        // Proceed to XOR with the corresponding word from the previous round
        ExpandedKey[i][0] = ExpandedKey[i-1][0] ^ temp;

        // For the rest of the words in this set, just XOR with the previous word
        for (j = 1; j < 4; ++j) {
            ExpandedKey[i][j] = ExpandedKey[i][j-1] ^ ExpandedKey[i-1][j];
        }
    }
}
/*
void ExpandKey1D(uint32_t Key[4], uint32_t ExpandedKey[][4]) {
    uint32_t temp;
    uint32_t rconIndex = 0;

    // Copy the original key to the first set of ExpandedKey
    for (int i = 0; i < 4; i++) {
        ExpandedKey[0][i] = Key[i];
    }

    // Generate the rest of the expanded key
    for (int i = 1; i < 11; i++) {
        // Take the last word of the previous block
        temp = ExpandedKey[i - 1][3];

        // Rotate the 32-bit word 8 bits to the left
        // Equivalent to rotating a row in the original 2-D array
        temp = (temp << 8) | (temp >> 24);

        // Apply S-box substitution to each byte of the temp word
        temp = (SBox[(temp >> 24) & 0xFF] << 24) |
               (SBox[(temp >> 16) & 0xFF] << 16) |
               (SBox[(temp >> 8) & 0xFF] << 8) |
               (SBox[temp & 0xFF]);

        // XOR the temp with the RCon value of this iteration
        temp ^= RCon[rconIndex++];
        // The original key of this round
        temp ^= ExpandedKey[i - 1][0];
        // The first word of the new key becomes the XOR result
        ExpandedKey[i][0] = temp;

        // The rest of the words are XORed with the corresponding words from the previous round
        for (int j = 1; j < 4; j++) {
            ExpandedKey[i][j] = ExpandedKey[i - 1][j] ^ ExpandedKey[i][j - 1];
        }
    }
}
*/

void MixColumns1D(uint32_t *state) {
    uint32_t temp[4];

    // MixColumns on each column
    for (int i = 0; i < 4; i++) {
        uint8_t s0 = first(state[i]);
        uint8_t s1 = second(state[i]);
        uint8_t s2 = third(state[i]);
        uint8_t s3 = fourth(state[i]);

        temp[i] = toFirst((unsigned char)(xTime02[s0] ^ xTime03[s1] ^ s2 ^ s3)) ^
                  toSecond((unsigned char)(s0 ^ xTime02[s1] ^ xTime03[s2] ^ s3)) ^
                  toThird((unsigned char)(s0 ^ s1 ^ xTime02[s2] ^ xTime03[s3])) ^
                  toFourth((unsigned char)(xTime03[s0] ^ s1 ^ s2 ^ xTime02[s3]));
    }

    // Copy the temporary state back to the original state
    for (int i = 0; i < 4; i++) {
        state[i] = temp[i];
    }
}

void MixColumns (uint8_t StateArray[][4])
{
	int i;
	uint8_t StateArrayTmp[4][4];

	for (i = 0; i < 4; i++) { // Iterate over each column
        StateArrayTmp[0][i] = 
            xTime02[StateArray[0][i]] ^ xTime03[StateArray[1][i]] ^
            StateArray[2][i] ^ StateArray[3][i];
        StateArrayTmp[1][i] = 
            StateArray[0][i] ^ xTime02[StateArray[1][i]] ^
            xTime03[StateArray[2][i]] ^ StateArray[3][i];
        StateArrayTmp[2][i] = 
            StateArray[0][i] ^ StateArray[1][i] ^
            xTime02[StateArray[2][i]] ^ xTime03[StateArray[3][i]];
        StateArrayTmp[3][i] = 
            xTime03[StateArray[0][i]] ^ StateArray[1][i] ^
            StateArray[2][i] ^ xTime02[StateArray[3][i]];
    }

	memcpy(StateArray, StateArrayTmp, 4 * 4 * sizeof(uint8_t));
}
