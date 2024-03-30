/*
 * AES_128.h
 *
 */

#ifndef AES_128_H_
#define AES_128_H_

#include "stdio.h"
#include "string.h"
#include "stdint.h"

//#include "xil_printf.h"
#ifndef xilprintf 
#define xil_printf printf
#endif


#define AES_PRINT_MAIN    (1<<0)	//change to 1<<0 to print
                                    //change to 0<<0 to disable printing detail
#define AES_PRINT_DETAILS (1<<1)	//change to 1<<1 to print details
                                    //change to 0<<1 to disable printing details

#define AES_PRINT (AES_PRINT_MAIN | AES_PRINT_DETAILS)

#define first(x)  ((uint32_t)(x) >> 24)
#define second(x) ((uint32_t)(x) >> 16 & 0xFF)
#define third(x)  ((uint32_t)(x) >> 8 & 0xFF)
#define fourth(x) ((uint32_t)(x) & 0xFF)

#define toFirst(x)  (((uint32_t)(x)) << 24)
#define toSecond(x) (((uint32_t)(x) & 0xFF) << 16)
#define toThird(x)  (((uint32_t)(x) & 0xFF) << 8)
#define toFourth(x) ((uint32_t)(x) & 0xFF)



/**********************************************************************
 * Functions for key expansion
 *********************************************************************/

void ExpandKey1D(uint32_t Key[4], uint32_t ExpandedKey[][4]);
void AddRoundKey1D(uint32_t Key[4], uint32_t StateArray[4]);
void ExpandKey (uint8_t Key[][4], uint8_t ExpandedKey[][4][4]);
void AddRoundKey (uint8_t Key[][4], uint8_t StateArray[][4]);

/**********************************************************************
 * Functions for AES encryption
 **********************************************************************/

void collapse_to_1D(uint8_t src[4][4], uint32_t dst[4]);
void SubBytesAndShiftRows1D(uint32_t *state);
void MixColumns1D(uint32_t *state);
void AddRoundKey1D(uint32_t Key[4], uint32_t StateArray[4]);
void expand_to_2D(uint32_t src[4], uint8_t dst[4][4]);
uint8_t affine_transform(uint8_t byte);
void SubBytesCalculated (uint8_t StateArray[]);

/**********************************************************************
 * Functions for AES decryption
 **********************************************************************/
 
void InvSubBytes (uint8_t StateArray[][4]);
void InvShiftRows (uint8_t StateArray[][4]);
void InvMixColumns (uint8_t StateArray[][4]);
 
/**********************************************************************
 * Miscellaneous Functions 
 **********************************************************************/

void AES_printf (uint8_t StateArray[][4]);



#endif /* AES_128_H_ */
