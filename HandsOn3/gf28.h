#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


// Define the irreducible polynomial modulus for GF(2^8), x^8 + x^4 + x^3 + x + 1
#define FofX 0x11B


uint32_t gf_add(uint32_t a, uint32_t b);
uint32_t gf_inv(uint32_t element);
uint32_t gf_div(uint32_t dividend, uint32_t divisor);
int gf_degree(uint32_t element);
uint32_t gf_mult(uint32_t a, uint32_t b);
void find_generators(void);