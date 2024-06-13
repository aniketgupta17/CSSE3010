/**   
 ******************************************************************************   
* @file mylib/ s4824063_hamming.h
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
#ifndef S4824063_HAMMING_H
#define S4824063_HAMMING_H

unsigned short s4824063_lib_hamming_byte_encode(unsigned char value);
unsigned char s4824063_lib_hamming_byte_decode(unsigned char in );
unsigned char hamming_hbyte_encode(unsigned char value);
int s4824063_lib_hamming_parity_error(unsigned char value);

#endif