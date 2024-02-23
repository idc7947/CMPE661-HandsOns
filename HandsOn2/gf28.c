#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "platform.h"
//#include "xil_printf.h"

// Define the irreducible polynomial modulus for GF(2^8), x^8 + x^4 + x^3 + x + 1
#define FofX 0x11B
uint32_t gf_add(uint32_t a, uint32_t b);
uint32_t gf_inv(uint32_t element);
uint32_t gf_div(uint32_t dividend, uint32_t divisor);
int gf_degree(uint32_t element);
uint32_t gf_mult(uint32_t a, uint32_t b);
void find_generators(void);
uint32_t verify_operations(void);

// Addition in GF(2^8) is just XOR
uint32_t gf_add(uint32_t a, uint32_t b) {
    return a ^ b;
}

uint32_t gf_inv(uint32_t element) {
    if (element == 0) {
        // 0 doesn't have an inverse in a multiplicative group.
        return 0;
    }
    uint32_t n = FofX, s = 1, t = 0;
    int32_t shamt = gf_degree(element) - 8;
    uint32_t temp;

    while (element != 1) {
        //printf("divloop! %u\r\n", element);
        if (shamt < 0) {
            // swap element and n
            temp = element;
            element = n;
            n = temp;
            // swap s and t
            temp = s;
            s = t;
            t = temp;
            shamt = -(shamt);
        }
        element = gf_add(element, (n << shamt));
        element = element % 256;
        s = gf_add(s, (t << shamt));
        s = s % 256;
        shamt = gf_degree(element) - gf_degree(n);
    }
    /*
    uint32_t r0 = element, r1 = FofX;
    uint32_t s0 = 1, s1 = 0;
    uint32_t q, temp;

    while (r0 != 0) {
        printf("InvLoop! %u\r\n", r0);
        q = gf_div(r1, r0); // Replace direct division with the gf_div function.

        // Calculate the new remainder
        temp = r0;
        r0 = gf_add(r1, gf_mult(q, r0));
        r1 = temp;

        // Calculate the new coefficient for num
        temp = s0;
        s0 = gf_add(s1, gf_mult(q, s0));
        s1 = temp;
    }

    // When r0 reaches 0, the last non-zero remainder is in r1, which should be 1 if num is invertible
    // The multiplicative inverse is the last non-zero coefficient stored in s1
    if (r1 != 1) {
        // This means that num and MODULUS are not relatively prime, and num has no inverse.
        printf("r1 != 1\r\n");
    }
    */

    return s;
}

uint32_t gf_div(uint32_t dividend, uint32_t divisor) {
    if (divisor == 0) {
        exit(99);
    }
    
    uint32_t remainder = dividend;  // Start with the dividend as the remainder
    uint32_t deg_rem = gf_degree(remainder);
    uint32_t deg_div = gf_degree(divisor);
    uint32_t deg_diff = deg_rem - deg_div; 
    uint32_t result = 0;

    while(deg_rem >= deg_div) {
        remainder = gf_add(remainder, (divisor << deg_diff)); // Add and Sub are the same in GF
        deg_rem = gf_degree(remainder);
        result = gf_add(result, (1 << (deg_diff)));
        deg_diff = deg_rem - deg_div;
        if (remainder == 0) break;
    }
    return result;  // T
}

// Helper function to calculate the degree (the position of the highest bit set) of an element.
int gf_degree(uint32_t element) {
    element >>= 1;
    int degree = 0;  // Start with -1, which represents an element of 0.
    while (element) {
        degree++;
        element >>= 1;
    }
    return degree;
}


// Multiplication in GF(2^8), with reduction by the irreducible polynomial
uint32_t gf_mult(uint32_t a, uint32_t b) {
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
uint32_t verify_operations() {
    int fail = 0;
    for (uint32_t i = 1; i < 256; i++) { // Loop through all non-zero elements
        uint32_t inverse = gf_inv(i); // Calculate the multiplicative inverse
        if (inverse == 0) {
            printf("No inverse for %#04x\n", i);
            fail = 1; // Flag failure if no inverse exists
            continue;
        }
        uint32_t result = gf_mult(i, inverse); // Multiply the element by its inverse
        if (result != 1) {
            printf("Test fail for element %#04x: (element * inverse) != 1, result is %#04x\n", i, result);
            fail = 1; // Flag failure if the product is not 1
        } else {
            printf("Test pass for element %#04x: (element * inverse) == 1\n", i);
        }
    }
    if (fail) {
        printf("Some tests failed.\n");
    } else {
        printf("All tests passed! Every element has a correct multiplicative inverse.\n");
    }
    return fail; // Return status code indicating success or failure
}

void verify_div(void) {
    // Test every combination of dividend and divisor
    for (uint32_t dividend = 1; dividend < 256; dividend++) {
        uint32_t quot = gf_div(0xcb, dividend);
        printf("0xcb / %#04x = %#04x\r\n", dividend, quot);
    }
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
            a = gf_mult(a, i);			// a = i^j
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
    //init_platform();

    


    

    verify_operations();
    //find_generators();

    //cleanup_platform();
    return 0;
}
