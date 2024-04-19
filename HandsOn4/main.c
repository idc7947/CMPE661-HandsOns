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
    //init_platform();
    //testMult();
    // Test Case 3
    AES_GCM_In TC_in;
    AES_GCM_Out TC_out;
    printf("---------------TEST CASE 3---------------\r\n");
    from_hex("feffe9928665731c6d6a8f9467308308", TC_in.K);
    TC_in.KLength = 128;
    from_hex("cafebabefacedbaddecaf88800000000", TC_in.IV);
    TC_in.IVLength = 96;
    uint32_t tempWord[4];
    from_hex("d9313225f88406e5a55909c5aff5269a", tempWord);
    memcpy(TC_in.P[0], tempWord, sizeof(gf128));
    from_hex("86a7a9531534f7da2e4c303d8a318a72", tempWord);
    memcpy(TC_in.P[1], tempWord, sizeof(gf128));
    from_hex("1c3c0c95956809532fcf0e2449a6b525", tempWord);
    memcpy(TC_in.P[2], tempWord, sizeof(gf128));
    from_hex("b16aedf5aa0de657ba637b391aafd255", tempWord);
    memcpy(TC_in.P[3], tempWord, sizeof(gf128));
    TC_in.PLength = 512;
    TC_in.ALength = 0;
    for (int i = 0; i < 4; i++) zero_gf128(TC_in.A[i]);
    EncryptionGCM(TC_in, TC_out);
    printf("-------------END TEST CASE 3-------------\r\n");


    // Test Case 4
    printf("---------------TEST CASE 4---------------\r\n");
    TC_in.P[3][3] = 0; // The only change besides A lol
    TC_in.ALength = 160;
    TC_in.PLength = 480;
    from_hex("feedfacedeadbeeffeedfacedeadbeef", tempWord);
    memcpy(TC_in.A[0], tempWord, sizeof(gf128));
    from_hex("abaddad2000000000000000000000000", tempWord);
    memcpy(TC_in.A[1], tempWord, sizeof(gf128));
    EncryptionGCM(TC_in, TC_out);
    printf("-------------END TEST CASE 4-------------\r\n");




    //cleanup_platform();
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



