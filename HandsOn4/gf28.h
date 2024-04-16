#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// Define the irreducible polynomial modulus for GF(2^8), x^8 + x^4 + x^3 + x + 1
#define FofX 0x11B


uint8_t gf_add(uint8_t a, uint8_t b);
uint8_t gf_inv(uint8_t element);
uint8_t gf_div(uint8_t dividend, uint8_t divisor);
int gf_degree(uint8_t element);
uint8_t gf_mult(uint8_t a, uint8_t b);
void find_generators(void);