/**   
 ******************************************************************************   
* @file mylib/ s4824063_mfs_led.c
* @author Aniket Gupta  - s48240639
* @date 23/03/2024
* @brief MFS Led
* REFERENCE:   
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
@brief Initialize MFS LEDs.
 ******************************************************************************   
 */



/**
 * @brief Initialize MFS LEDs.
 * 
 * Initializes GPIO pins connected to the MFS LEDs and sets them as outputs.
 */
#include "s4824063_mfs_led.h"
#include "board.h" 
#include "processor_hal.h"

void s4824063_reg_mfs_led_init(void) {

    // Enable GPIO clock
    __GPIOD_CLK_ENABLE();
     __GPIOA_CLK_ENABLE();
     __GPIOB_CLK_ENABLE();


    //Initialise Pins as the outputs
    GPIOA->MODER &= ~((0x03 << (5 * 2)) | (0x03 << (6 * 2)) | (0x03 << (7 * 2)) );  // Clears the bits for Pins 
    GPIOA->MODER |= ((0x01 << (5 * 2)) | (0x01 << (6 * 2)) | (0x01 << (7 * 2)) );  // Set pins to output
   GPIOD->MODER &= ~(0x03 << (14 * 2));  // Clears the bits for Pins 
    GPIOD->MODER |= ((0x01 << (14 * 2)) );  // Set pins to output
    GPIOB->MODER &= ~((0x03 << (5 * 2))  );
    GPIOB->MODER |= ((0x01 << (5 * 2)));
   
    //Clear Bits in OTYPE register for Push/Pull Output
    GPIOA->OTYPER &= ~((0x01 << 5) | (0x01 << 6) | (0x01 << 7) ); 
    GPIOD->OTYPER &= ~((0x01 << 14) );
    GPIOB->OTYPER &= ~((0x01 << 5) );

    s4824063_REG_MFS_LED_D1_OFF();
     s4824063_REG_MFS_LED_D2_OFF();
      s4824063_REG_MFS_LED_D3_OFF();
       s4824063_REG_MFS_LED_D4_OFF();
  
}
void s4824063_REG_MFS_LED_D1_ON(void) {
    
 
     GPIOA->ODR &= ~(0x01 << 5); 
}

void s4824063_REG_MFS_LED_D1_OFF(void) {
       GPIOA->ODR |= (0x01 << 5);
}

void s4824063_REG_MFS_LED_D2_ON(void) {
    GPIOA->ODR &= ~(0x01 << 6); 
}

void s4824063_REG_MFS_LED_D2_OFF(void) {
   
   
    GPIOA->ODR |= (0x01 << 6); 
}

void s4824063_REG_MFS_LED_D3_ON(void) {
    
     GPIOA->ODR &= ~(0x01 << 7) ; 
}

void s4824063_REG_MFS_LED_D3_OFF(void) {
    
  
    GPIOA->ODR |= (0x01 << 7); 
}

void s4824063_REG_MFS_LED_D4_ON(void) {
      GPIOD->ODR &= ~(0x01 << 14); 
}

void s4824063_REG_MFS_LED_D4_OFF(void) {
   
 
     GPIOD->ODR |= (0x01 << 14); 
}

void s4824063_REG_MFS_LED_D1_TOGGLE(void){
    s4824063_REG_MFS_LED_D1_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D1_OFF();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D1_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D1_OFF();
            HAL_Delay(500);

}

void s4824063_REG_MFS_LED_D2_TOGGLE(void){
    s4824063_REG_MFS_LED_D2_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D2_OFF();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D2_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D2_OFF();
            HAL_Delay(500);

}