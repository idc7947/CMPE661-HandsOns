#include <stdio.h>
#include <string.h>
#include <strings.h>


//#include "xparameters.h"
//#include "platform.h"

#include "gf2128.h"
#include "AESGCM.h"
#include "AES_128_1D.h"

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

uint8_t example[4][4]  = {  
       {0x01, 0x89, 0x76, 0xFE},
		{0x23, 0xAB, 0x54, 0xDC},
		{0x45, 0xCD, 0x32, 0xBA},
		{0x67, 0xEF, 0x10, 0x98} 
};
// To set all the bytes in a block of memory to a particular value, use memset().
// The function prototype is void * memset(void *dest, int c, size_t count);
void testMult(void);
void testColumns(void);


int main()
{
/*
	//init_platform();

	xil_printf("\n\n\n");
	xil_printf("-- Starting AES software test based on FIPS-197 (Appendix B)\r\n\n");

	encryptNoBullshit();

	//decrypt();

	xil_printf("**********************************************\r\n");

	xil_printf("-- Exiting main() --\r\n");
	//cleanup_platform();
*/
   // Test Case 3
   AES_GCM_In TC3_in;
   AES_GCM_Out TC3_out;
   from_hex("feffe9928665731c6d6a8f9467308308", TC3_in.K);
   TC3_in.KLength = 128;
   uint32_t IV_in[4] = {0xcafebabe, 0xfacedbad, 0xdecaf888, 0x0000000};
   TC3_in.IV[0] = IV_in[0];
   TC3_in.IV[1] = IV_in[1];
   TC3_in.IV[2] = IV_in[2];
   TC3_in.IV[3] = IV_in[3];
   TC3_in.IVLength = 96;
   uint32_t P_in[16];
   from_hex_512("d9313225f88406e5a55909c5aff5269a86a7a9531534f7da2e4c303d8a318a721c3c0c95956809532fcf0e2449a6b525b16aedf5aa0de657ba637b391aafd255", P_in);
   for (int i = 0; i < 16; i++) {
    TC3_in.P[i] = P_in[i];
   }
   TC3_in.PLength = 512;
   TC3_in.ALength = 0;

   EncryptionGCM(TC3_in, TC3_out);

    //gcc main.c gf2128.c AESGCM.c AES_128_1D.c gf28.c multiplication_tables.c -o test
	return 0;
}


void testMult(void) {
    gf128 x, y, z, expected;
    // Test Case 1
    from_hex("0388dace60b6a392f328c2b971b2fe78", x);
    from_hex("66e94bd4ef8a2c3b884cfa59ca342b2e", y);
    from_hex("5e2ec746917062882c85b0685353deb7", expected);
    gf128_mul(x, y, z);
    printf("Z1:          ");
    print_gf128(z);
    printf("Expected Z1: ");
    print_gf128(expected);

    // Test Case 2
    from_hex("5e2ec746917062882c85b0685353de37", x);
    from_hex("66e94bd4ef8a2c3b884cfa59ca342b2e", y);
    from_hex("f38cbb1ad69223dcc3457ae5b6b0f885", expected);
    gf128_mul(x, y, z);
    printf("Z2:          ");
    print_gf128(z);
    printf("Expected Z2: ");
    print_gf128(expected);

    // Test Case 3
    from_hex("ba471e049da20e40495e28e58ca8c555", x);
    from_hex("b83b533708bf535d0aa6e52980d53b78", y);
    from_hex("b714c9048389afd9f9bc5c1d4378e052", expected);
    gf128_mul(x, y, z);
    printf("Z3:          ");
    print_gf128(z);
    printf("Expected Z3: ");
    print_gf128(expected);
}

void testColumns(void) {
    gf128 example1D;
   uint8_t example2D[4][4];

   collapse_to_1D(example, example1D);
   print_gf128(example1D);
   zero_gf128(example1D);
   from_hex("0123456789ABCDEF76543210FEDCBA98", example1D);
   print_gf128(example1D);
   expand_to_2D(example1D, example2D);
   AES_printf(example2D);
   AES_printf(example);
}



