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

/**
 * Debug levels
 */

#define AES_PRINT_MAIN    (1<<0)	//change to 1<<0 to print
                                    //change to 0<<0 to disable printing detail
#define AES_PRINT_DETAILS (1<<1)	//change to 1<<1 to print details
                                    //change to 0<<1 to disable printing details

#define AES_PRINT (AES_PRINT_MAIN | AES_PRINT_DETAILS)

/**********************************************************************
 * Functions for key expansion
 *********************************************************************/

void ExpandKey (uint8_t Key[][4], uint8_t ExpandedKey[][4][4]);
void AddRoundKey (uint8_t Key[][4], uint8_t StateArray[][4]);

/**********************************************************************
 * Functions for AES encryption
 **********************************************************************/

void SubBytes (uint8_t StateArray[][4]);
void ShiftRows (uint8_t StateArray[][4]);
void MixColumns (uint8_t StateArray[][4]);
uint8_t affine_transform(uint8_t byte);
void SubBytesCalculated (uint8_t StateArray[][4]);

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
