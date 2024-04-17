#include <stdio.h>
#include <string.h>
#include <strings.h>


//#include "xparameters.h"
//#include "platform.h"

#include "gf2128.h"

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
void testMult(void);


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



