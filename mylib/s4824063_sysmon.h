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

#ifndef s4824063_SYSMON_H
#define s4824063_SYSMON_H

void s4824063_sysmon_init();
//System monitor pins
#define SYSMON_CHAN0_PIN	10
#define SYSMON_CHAN0_GPIO	GPIOD
#define SYSMON_CHAN0_GPIO_CLK()	__GPIOD_CLK_ENABLE()

#define SYSMON_CHAN1_PIN	7
#define SYSMON_CHAN1_GPIO	GPIOG
#define SYSMON_CHAN1_GPIO_CLK()	__GPIOG_CLK_ENABLE()

#define SYSMON_CHAN2_PIN	4
#define SYSMON_CHAN2_GPIO	GPIOG
#define SYSMON_CHAN2_GPIO_CLK()	__GPIOG_CLK_ENABLE()

#define SYSMON_CHAN0_SET()		SYSMON_CHAN0_GPIO->ODR|= (0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN0_CLR()		SYSMON_CHAN0_GPIO->ODR &= ~(0x01 << SYSMON_CHAN0_PIN);
#define SYSMON_CHAN1_SET()		SYSMON_CHAN1_GPIO->ODR |= (0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN1_CLR()		SYSMON_CHAN1_GPIO->ODR &= ~(0x01 << SYSMON_CHAN1_PIN);
#define SYSMON_CHAN2_SET()		SYSMON_CHAN2_GPIO->ODR |= (0x01 << SYSMON_CHAN2_PIN);
#define SYSMON_CHAN2_CLR()		SYSMON_CHAN2_GPIO->ODR &= ~(0x01 << SYSMON_CHAN2_PIN);

#endif