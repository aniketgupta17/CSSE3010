/**
***************************************************************
* @file mylib/ s4824063_lta1000g.h
* @author MyName - s48240639
* @date 27/02/2024
* @brief LED Light Bar
* REFERENCE: 
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4824063_reg_lta1000g_init() - intialises the LED bar
* lta1000g_seg_set(int segment, unsigned char value) - sets the  LED bar value
* s4824063_reg_lta1000g_write(unsigned short value) - Writes the LED Bar segments to  high or low
* s4824063_reg_lta1000g_invert(unsigned short value, unsigned short mask) - Inverts the LED Bar segments
***************************************************************
*/

#ifndef s4824063_LTA000G_H
#define s4824063_LTA000G_H

void s4824063_reg_lta1000g_init();
void lta1000g_seg_set(int segment, unsigned char value);
void s4824063_reg_lta1000g_write(unsigned short value);
void s4824063_reg_lta1000g_invert(unsigned short value, unsigned short mask);

#endif