#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "platform.h"
#include "xil_printf.h"

// Define the irreducible polynomial modulus for GF(2^8), x^8 + x^4 + x^3 + x + 1
#define FofX 0b000100011011

// Addition in GF(2^8) is just XOR
uint32_t gf_add(uint32_t a, uint32_t b) {
    return a ^ b;
}

// Multiplication in GF(2^8), with reduction by the irreducible polynomial
uint32_t gf_mult_28(uint32_t a, uint32_t b) {
    uint32_t product = 0;
    uint32_t power;
    uint32_t x8_reduce;  // You only need to add f(x) if the high term (in this case x^8) is present
    for (power = 0; power < 8; power++) {
        if (b & 0b00000001) {
			product ^= a;    // product += a(x) * (next term of b(x))
		}
        x8_reduce = a & 0b10000000; // we know we need to reduce because the x^7 term is present
		// shift values of a to the left, effectively multiplying a(x) by x
        a <<= 1;  // note: this effectively pre-multiplies the a variable if the next loop's b term is set
        if (x8_reduce) {
			a ^= FofX; // add f(x) if x8 present
		}
        b >>= 1; // Shift b right to eval next term in b(x)
    }
    return product;
}

// Function to verify correctness of operations using provided test cases
void verify_operations() {
    // Example test cases:
    // (0xcb * 0x02) mod 0x11b should be 0x8d
    // (0xcb * 0x03) mod 0x11b should be 0x46
    printf("Verification of multiplication: (0xcb * 0x02) mod 0x11b = %#x\r\n", gf_mult_28(0xcb, 0x02));
    printf("Verification of multiplication: (0xcb * 0x03) mod 0x11b = %#x\r\n", gf_mult_28(0xcb, 0x03));
}

/*
Function to find and print all generators in GF(2^8)
A generator is an element 'g' such that every non-zero element of the field can be expressed as g^k for some integer k
*/
void find_generators() {
	uint16_t gen_count = 0;
	// Outer loop will go through all elements of GF(2^8) except 0, as it can't be a generator
    for (uint32_t i = 1; i < 256; i++) {
		uint32_t powers[256] = {0}; 		// This array will keep track of all the "powers" we have seen (x^1 -> x^255)
        uint32_t a = i;							
        int is_generator = 1;
        for (int j = 0; j < 255; j++) { 	// Multiplying element 'i' by itself to generate all its powers
            a = gf_mult_28(a, i);			// a = i^j
            if (powers[a] != 0) {				// if its been seen before in the power array, we know it's not a generator
                is_generator = 0;
                break;
            }
            powers[a] = 1;						// if it isn't already known, label it as a generator
        }
        if (is_generator) {
            printf("0x%x is a generator\r\n", i);
			gen_count++;
        }
    }
	printf("%u generators!\r\n", gen_count); // Count should be 128
}

int main() {
    init_platform();

    verify_operations();
    find_generators();

    cleanup_platform();
    return 0;
}
