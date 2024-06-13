/**
***************************************************************
* @file mylib/ s4824063_rgb.h
* @author s48240639
* @date 27/02/2024
* @brief RGB LED
* REFERENCE: csse3010_mylib_reg_rgb.pdf
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************

***************************************************************
*/

#ifndef s4824063_RGB_H
#define s4824063_RGB_H

#define s4824063_REG_RGB_BLACK() s4824063_reg_rgb_colour(0x07)
#define s4824063_REG_RGB_WHITE() s4824063_reg_rgb_colour(0x00)
#define s4824063_REG_RGB_RED() s4824063_reg_rgb_colour(0x03)
#define s4824063_REG_RGB_GREEN() s4824063_reg_rgb_colour(0x05)
#define s4824063_REG_RGB_BLUE() s4824063_reg_rgb_colour(0x06)
#define s4824063_REG_RGB_YELLOW() s4824063_reg_rgb_colour(0x01)
#define s4824063_REG_RGB_CYAN() s4824063_reg_rgb_colour(0x04)
#define s4824063_REG_RGB_MAGENTA() s4824063_reg_rgb_colour(0x02)

void s4824063_reg_rgb_init();
void s4824063_reg_rgb_brightness_write(int level);
int s4824063_reg_rgb_brightness_read();
void s4824063_reg_rgb_colour_set(unsigned char rgb_mask);
void s4824063_reg_rgb_colour_get();

#endif