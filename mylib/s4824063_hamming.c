/**   
 ******************************************************************************   
* @file mylib/ s4824063_hamming.c
* @author Aniket Gupta  - s48240639
* @date 23/03/2024
* @brief Hamming
* REFERENCE:   
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
unsigned short s4824063_lib_hamming_byte_encode( unsigned char value)
unsigned char s4824063_lib_hammin_byte decode(unsigned char value)
unsigned char hamming_hbyte_encode(unsigned char value)
int s4824063_lib_hamming_parity_error(unsigned char value)
 ******************************************************************************   
 */

#include <stdint.h>
#include "s4824063_hamming.h"


 /**
  * @brief   Encode a single byte using Hamming(8,4) code.
  * @param   in: The input byte to be encoded.
  * @retval  The encoded byte.
  */
unsigned char  hamming_hbyte_encode(unsigned char in) {
   unsigned char d0 , d1, d2, d3;
	unsigned char p0 = 0, h0, h1, h2;
	unsigned char z;
	uint16_t out;

	/* extract bits */
	d0 = !!(in & 0x1);
	d1 = !!(in & 0x2);
	d2 = !!(in & 0x4);
	d3 = !!(in  & 0x8);

	/* calculate hamming parity bits */
	h0 = d1 ^ d2 ^ d3;
	h1 = d0 ^ d2 ^ d3;
	h2 = d0 ^ d1 ^ d3;

	/* generate out byte without parity bit P0 */
	out = (h0 << 1) | (h1 << 2) | (h2 << 3) |
		(d0 << 4) | (d1 << 5) | (d2 << 6) | (d3 << 7);

	/* calculate even parity bit */
	for (z = 1; z<8; z++)
		p0 = p0 ^ !!(out & (1 << z));

	out |= p0;

	return(out);
}

/**
  * @brief   Encode a byte using Hamming(8,4) code for each nibble.
  * @param   value: The byte to be encoded.
  * @retval  The encoded byte with parity bits.
  */
unsigned short s4824063_lib_hamming_byte_encode(unsigned char value) {
    uint16_t out;

	/* first encode D0..D3 (first 4 bits),
	 * then D4..D7 (second 4 bits).
	 */
	out = hamming_hbyte_encode(value & 0xF) |
		(hamming_hbyte_encode(value >> 4) << 8);

	return(out);

}
/**
  * @brief   Check for a parity error in the received byte.
  * @param   value: The received byte with parity bit.
  * @retval  1 if a parity error is detected, 0 otherwise.
  */
int s4824063_lib_hamming_parity_error(unsigned char value) {
   uint8_t z;
	uint8_t messageParity = 0;
	uint8_t p0 = (value & (1 << 0)); // extracts the parity bit.

	// Calculate the parity of the message.
	for (z = 1; z < 8; z++) {
		messageParity = messageParity ^ !!(value & (1 << z));
	}

	// Check if the parity bit matches the actual parity of the message.
	if (p0 != messageParity) {
		// A parity error has occurred.
		return 1;
	} else {
		return 0;
	}
}

/**
  * @brief   Decode a byte encoded with Hamming(8,4) code.
  * @param   value: The encoded byte.
  * @retval  The decoded byte.
  */
unsigned char s4824063_lib_hamming_byte_decode(unsigned char value) {
    uint8_t d0, d1, d2, d3, h0, h1, h2, s2, s1, s0;
    unsigned char decodedByte = 0x00;

    // Extract Hamming bits and Data bits.
    h0 = !!(value & (1 << 1));
    h1 = !!(value & (1 << 2));
    h2 = !!(value & (1 << 3));
    d0 = !!(value & (1 << 4));
    d1 = !!(value & (1 << 5));
    d2 = !!(value & (1 << 6));
    d3 = !!(value & (1 << 7));

    // Calculate the Syndrome.
    s0 = h0 ^ d1 ^ d2 ^ d3;
    s1 = h1 ^ d0 ^ d2 ^ d3;
    s2 = h2 ^ d0 ^ d1 ^ d3;

    // Correct errors if any.
    if (!s0 && s1 && s2) {
        // d3 bit error
        d3 = !d3;
    } else if (s0 && !s1 && s2) {
        // d2 bit error
        d2 = !d2;
    } else if (s0 && s1 && !s2) {
        // d1 bit error
        d1 = !d1;
    } else if (s0 && s1 && s2) {
        // d0 bit error
        d0 = !d0;
    } else if (s0 && !s1 && !s2) {
        // h0 bit error
        h0 = !h0;
    } else if (!s0 && s1 && !s2) {
        // h1 bit error
        h1 = !h1;
    } else if (!s0 && !s1 && s2) {
        // h2 bit error
        h2 = !h2;
    }

    decodedByte =  (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3);

    return decodedByte;
}