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

void ExpandKey (uint32_t Key[][4], uint32_t ExpandedKey[][4][4]);
void AddRoundKey (uint32_t Key[][4], uint32_t StateArray[][4]);

/**********************************************************************
 * Functions for AES encryption
 **********************************************************************/

void SubBytes (uint32_t StateArray[][4]);
void ShiftRows (uint32_t StateArray[][4]);
void MixColumns (uint32_t StateArray[][4]);

void SubBytesCalculated (uint32_t StateArray[][4]);

/**********************************************************************
 * Functions for AES decryption
 **********************************************************************/
 
void InvSubBytes (uint32_t StateArray[][4]);
void InvShiftRows (uint32_t StateArray[][4]);
void InvMixColumns (uint32_t StateArray[][4]);
 
/**********************************************************************
 * Miscellaneous Functions 
 **********************************************************************/

void AES_printf (uint32_t StateArray[][4]);

#endif /* AES_128_H_ */
