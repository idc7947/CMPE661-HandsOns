#include "gf2128.h"


// Polynomial R in GF(2^128) (ignoring the x^128 term)
//const gf128 R = {0, 0, 0, 0xE1000000};
const gf128 R = {0xE1000000, 0, 0, 0};

// Function to zero a GF(2^128) element.
void zero_gf128(gf128 z) {
    for (int i = 0; i < GF128_NUM_WORDS; ++i) {
        z[i] = 0;
    }
}

// Function to copy a GF(2^128) element.
void copy_gf128(gf128 dst, const gf128 src) {
    for (int i = 0; i < GF128_NUM_WORDS; ++i) {
        dst[i] = src[i];
    }
}

// Function to XOR two GF(2^128) elements.
void xor_gf128(gf128 z, const gf128 x) {
    for (int i = 0; i < GF128_NUM_WORDS; ++i) {
        z[i] ^= x[i];
    }
}


// Function to check if a bit is set in a GF(2^128) element.
int bit_is_set(const gf128 x, int bit) {
    int word_index = bit / 32;        // Find which 32-bit word the bit is in
    int bit_index = 31 - (bit % 32);         // Find the bit position within the word
    return (x[word_index] >> (bit_index)) & 0x01;  // Check if the bit is set
}


void right_shift_gf128(gf128 V) {
    uint32_t temp, next_temp;

    // Start by capturing the bit that will be carried over to the next word
    next_temp = V[0]  & 0x01; // Save MSB of V[0] to be LSB of V[1]
    // Perform the shifts right to left to ensure correct carry handling
    V[0] = V[0] >> 1;

    temp = next_temp;  // Prepare the carry for the next shift
    next_temp = V[1] & 0x01;  // Save MSB of V[1] to be LSB of V[2]
    V[1] = (temp << 31) | (V[1] >> 1);

    temp = next_temp;  // Prepare the carry for the next shift
    next_temp = V[2] & 0x01;  // Save MSB of V[2] to be LSB of V[3]
    V[2] = (temp << 31) | (V[2] >> 1);

    V[3] = (next_temp << 31) | (V[3] >> 1);  // Apply the last carry
}


void gf128_mul(gf128 a, gf128 b, gf128 y) {
    gf128 V;
    uint8_t bit_set;
    memcpy(V, a, sizeof(gf128)); // V = X = a
    memset(y, 0, sizeof(gf128)); // Z = y = 0

    // Main loop
    for (int i = 0; i < 128; i++) {
        if (bit_is_set(b, i)) {
            xor_gf128(y, V);
            //printf("Power %d set in b, y = ", i);
            //print_gf128(y);
        }
        if (bit_is_set(V, 127)) { bit_set = 1;}
        else                    { bit_set = 0; }
        right_shift_gf128(V); // Right shift V
        if (bit_set) xor_gf128(V, R);
        
    }
}





void from_hex(const char *hex, gf128 a) {
    // Parse the hex string into the array, assuming the input hex string is exactly 32 characters long
    for (int i = 0; i < GF128_NUM_WORDS; i++) {
        sscanf(hex + 8 * i, "%8x", &a[i]);
    }
}



// Function to print a gf128 element as a hex string.
// Prints in reverse order to match the test vector format.
void print_gf128(const gf128 a) {
    for (int i = 0; i < GF128_NUM_WORDS ; i++) {
        printf("%08x", a[i]);
        printf(" [%d] ", i);
        
    }
    printf("\n");
}

void print_gf128_binary(const gf128 a) {
    for (int i = 0; i < GF128_NUM_WORDS ; i++) {
        for (int bit = 31; bit >= 0; --bit) {
            printf("%d", (a[i] >> bit) & 1);
        }
        printf(" [%d] ", i);
    }
    printf("\n");
}
