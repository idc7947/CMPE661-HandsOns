#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define GF128_NUM_WORDS 4 // GF(2^128) field elements are 4 words.

typedef uint32_t gf128[GF128_NUM_WORDS]; // GF(2^128) elements as arrays of uint32_t.
// THIS STRUCTURE^
// [0]   ..    [3]
// LSB   ..    MSB
// LSB = Bit 0 of [0]
// MSB = Bit 31 of [3]


void gf128_mul(gf128 a, gf128 b, gf128 y);

// String ops for input
void from_hex(const char *hex, gf128 a);

// Prints MSB to LSB order
void print_gf128(const gf128 a);
void print_gf128_binary(const gf128 a);

// Function to zero a GF(2^128) element.
void zero_gf128(gf128 z);

// Function to copy a GF(2^128) element.
void copy_gf128(gf128 dst, const gf128 src);

// Function to XOR two GF(2^128) elements.
void xor_gf128(gf128 z, const gf128 x);

// Shift right V 
void right_shift_gf128(gf128 V);

// Function to check if a bit is set in a GF(2^128) element.
int bit_is_set(const gf128 x, int bit);

