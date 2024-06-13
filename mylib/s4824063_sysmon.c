
#include "board.h"
#include "processor_hal.h"
#include "s4824063_sysmon.h"
#include "FreeRTOS.h"







void s4824063_sysmon_init(){

    SYSMON_CHAN0_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN0_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));  //clear bits
  	SYSMON_CHAN0_GPIO->MODER |= (0x01 << (SYSMON_CHAN0_PIN * 2));   //Set for push pull

  	SYSMON_CHAN0_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN0_PIN * 2));
	SYSMON_CHAN0_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN0_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN0_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN0_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN0_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN0_PIN * 2));   //Clear Bits
  	SYSMON_CHAN0_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN0_PIN * 2));  //Set for Pull down output
  
	SYSMON_CHAN1_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN1_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));  //clear bits
  	SYSMON_CHAN1_GPIO->MODER |= (0x01 << (SYSMON_CHAN1_PIN * 2));   //Set for push pull

  	SYSMON_CHAN1_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN1_PIN * 2));
	SYSMON_CHAN1_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN1_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN1_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN1_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN1_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN1_PIN * 2));   //Clear Bits
  	SYSMON_CHAN1_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN1_PIN * 2));  //Set for Pull down output

	SYSMON_CHAN2_GPIO_CLK();

  	//Initialise G9 as an output.
  	SYSMON_CHAN2_GPIO->MODER &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));  //clear bits
  	SYSMON_CHAN2_GPIO->MODER |= (0x01 << (SYSMON_CHAN2_PIN * 2));   //Set for push pull

  	SYSMON_CHAN2_GPIO->OSPEEDR &= ~(0x03<<(SYSMON_CHAN2_PIN * 2));
	SYSMON_CHAN2_GPIO->OSPEEDR |=   0x02<<(SYSMON_CHAN2_PIN * 2);  // Set for Fast speed

  	SYSMON_CHAN2_GPIO->OTYPER &= ~(0x01 << SYSMON_CHAN2_PIN);       //Clear Bit for Push/Pull utput

  	// Activate the Pull-up or Pull down resistor for the current IO
  	SYSMON_CHAN2_GPIO->PUPDR &= ~(0x03 << (SYSMON_CHAN2_PIN * 2));   //Clear Bits
  	SYSMON_CHAN2_GPIO->PUPDR |= ((0x01) << (SYSMON_CHAN2_PIN * 2));  //Set for Pull down output

}