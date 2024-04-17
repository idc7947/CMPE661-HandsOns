
#include "AES_128_1D.h"
#include "gf28.h"
extern unsigned char xTime02[256];
extern unsigned char xTime03[256];
extern unsigned char xTime09[256];
extern unsigned char xTime0b[256];
extern unsigned char xTime0d[256];
extern unsigned char xTime0e[256];

const uint8_t SBox[256] = {
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

const uint8_t InvSBox[256] = {
 // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,   //0
 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,   //1
 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,   //2
 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,   //3
 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,   //4
 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,   //5
 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,   //6
 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,   //7
 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,   //8
 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,   //9
 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,   //A
 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,   //B
 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,   //C
 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,   //D
 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,   //E
 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d }; //F





const uint8_t RCon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};



void ExpandKey1D(uint32_t Key[4], uint32_t ExpandedKey[][4]) {
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


void AddRoundKey1D(uint32_t Key[4], uint32_t StateArray[4]) {
    for (int i = 0; i < 4; i++) {
        StateArray[i] ^= Key[i];
    }
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

void AES_printf (uint8_t AES_StateArray[][4])
{
	int i;
	xil_printf("   W0  W1  W2  W3\r\n\n");
	for(i=0; i<4; i++)
		xil_printf("   %02x  %02x  %02x  %02x\r\n",
				AES_StateArray[i][0], AES_StateArray[i][1],
				AES_StateArray[i][2], AES_StateArray[i][3]);
	xil_printf("\n");
}

/**********************************************************************
 * Functions for AES decryption
 **********************************************************************/
 // use roundkey10 for first


 void InvSubBytes (uint8_t StateArray[][4])
{
	int i,j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			StateArray[i][j] = InvSBox[StateArray[i][j]];
		}
	}
}

void InvShiftRows (uint8_t StateArray[][4])
{
    uint8_t x;
    // Row#1 - rotate 1 column to the right
    x = StateArray[1][3];
    StateArray[1][3] = StateArray[1][2];
    StateArray[1][2] = StateArray[1][1];
    StateArray[1][1] = StateArray[1][0];
    StateArray[1][0] = x;
    
    // Row#2 - rotate 2 columns to the right
    x = StateArray[2][3];
    StateArray[2][3] = StateArray[2][1];
    StateArray[2][1] = x;
    x = StateArray[2][2];
    StateArray[2][2] = StateArray[2][0];
    StateArray[2][0] = x;
    
    // Row#3 - rotate 3 columns to the right (equivalent to 1 column to the left)
    x = StateArray[3][0];
    StateArray[3][0] = StateArray[3][1];
    StateArray[3][1] = StateArray[3][2];
    StateArray[3][2] = StateArray[3][3];
    StateArray[3][3] = x;
}

void InvMixColumns (uint8_t StateArray[][4])
{
    int i;
    uint8_t StateArrayTmp[4][4];

    for (i = 0; i < 4; i++) {
        StateArrayTmp[0][i] = 
            xTime0e[StateArray[0][i]] ^ xTime0b[StateArray[1][i]] ^
            xTime0d[StateArray[2][i]] ^ xTime09[StateArray[3][i]];
        StateArrayTmp[1][i] = 
            xTime09[StateArray[0][i]] ^ xTime0e[StateArray[1][i]] ^
            xTime0b[StateArray[2][i]] ^ xTime0d[StateArray[3][i]];
        StateArrayTmp[2][i] = 
            xTime0d[StateArray[0][i]] ^ xTime09[StateArray[1][i]] ^
            xTime0e[StateArray[2][i]] ^ xTime0b[StateArray[3][i]];
        StateArrayTmp[3][i] = 
            xTime0b[StateArray[0][i]] ^ xTime0d[StateArray[1][i]] ^
            xTime09[StateArray[2][i]] ^ xTime0e[StateArray[3][i]];
    }

	memcpy(StateArray, StateArrayTmp, 4 * 4 * sizeof(uint8_t));
}

void ExpandKey (uint8_t Key[][4], uint8_t ExpandedKey[][4][4])
{
	uint8_t TempKey[4][4];
	memset(&TempKey, 0, 4*4*sizeof (uint8_t));
	//bzero(TempKey, 4*4*sizeof (uint8_t));
	uint8_t TempKeyCol[4];
	memset(&TempKeyCol, 0, 4*sizeof (uint8_t));
	//bzero(TempKeyCol, 4*sizeof (uint8_t));
	int i,j;

	// Encryption Key copied to Expanded Key [0]
	memcpy(ExpandedKey[0], Key, 4 * 4 * sizeof(uint8_t));

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

void AddRoundKey (uint8_t Key[][4], uint8_t StateArray[][4])
{
	int i,j;
	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
			StateArray[i][j] ^= Key[i][j];
}

