/**   
 ******************************************************************************   
* @file mylib/s4824063_rgb.c
* @author Aniket Gupta  - s4824063
* @date 06/03/2024
* @brief RGB LED
* REFERENCE:   csse3010_mylib_reg_rgb.pdf
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
* s4824063_reg_rgb_init()- initialise RGB LED (GPIO, PWM, etc)
*s4824063_reg rgb brightness write(int level)- Function for controlling the brightness level (0% to 100%), using the duty cycle of a PWM signal.
*s4824063_reg_rgb_colour_set(unsigned char rgb mask)- Set the primary colour of the RGB Led. The RGB bit mask is < R >< G >< B >. This function must only turn GPIO pins high or low.
*s4824063_reg_rgb_colour_get()- Return the current primary colour of the RGB Led as an RGB bitmask. 
 ******************************************************************************   
 */

#include "board.h"
#include "processor_hal.h"
#include "s4824063_rgb.h"

#define TIMER_COUNTER_FREQ  100  //CLOCK COUNTER Frequency
#define PWM_PULSE_WIDTH_SECONDS  0.05    // PERIOD OF PWM (1/Freq) - 100Hz is desired                
#define PWM_PULSE_WIDTH_TICKS    1000    // Number of ticks per period
#define PWM_PERCENT2TICKS_DUTYCYCLE(value) (uint16_t)(value*PWM_PULSE_WIDTH_TICKS/100) //Mapping a percentage to duty cycle
#define PWM_DUTYCYCLE_CHANGE(value) TIM1->CCR1=(uint16_t)value //Changes CCR1 Value accordingly

/**
  * @brief Initialize the RGB LED.
  * @param None
  * @retval None
  */
void s4824063_reg_rgb_init(){

    // Enable the GPIO Clock for Port E and F - output
    __GPIOE_CLK_ENABLE();
    __GPIOF_CLK_ENABLE();

    //Initialise Pins as outputs
    // Port E 
    GPIOE->MODER &= ~(0x03 << (11 * 2));  //clear bits
    GPIOE->MODER |= (0x01 << (11 * 2));   //Set for push pull
    GPIOE->OSPEEDR &= ~(0x03<<(11 * 2));
	GPIOE->OSPEEDR |= (0x02<<(11 * 2));  // Set for Fast speed
    GPIOE->OTYPER &= ~(0x01 << 11);       //Clear Bit for Push/Pull Output
    GPIOE->PUPDR &= ~(0x03 << (11 * 2));   //Clear Bits
    GPIOE->PUPDR |= ((0x01) << (11 * 2));  //Set for Pull down output

    // PORT F 
    GPIOF->MODER &= ~((0x03 << (14 * 2))|(0x03 << (15 * 2)) );  
    GPIOF->MODER |= ((0x01 << (14 * 2))|(0x01 << (15 * 2)));
    GPIOF->OSPEEDR &= ~((0x03<<(14 * 2))|(0x03<<(15 * 2)));
	GPIOF->OSPEEDR |= ((0x02<<(14 * 2))|(0x02<<(15 * 2)));  
    GPIOF->OTYPER &= ~((0x01 << 14)|(0x01 << 15));       
    GPIOF->PUPDR &= ~((0x03 << (14 * 2))|(0x03 << (15 * 2)));   
    GPIOF->PUPDR |= ((0x01) << (14 * 2)|((0x01) << (15 * 2)));


    // Brightness control GPIO(PWM) - PORT E PIN 9
    GPIOE->OSPEEDR |= (GPIO_SPEED_FAST << 2);		//Set fast speed.
	GPIOE->PUPDR &= ~(0x03 << (9 * 2));				//Clear bits for no push/pull
	GPIOE->MODER &= ~(0x03 << (9 * 2));				//Clear bits
	GPIOE->MODER |= (GPIO_MODE_AF_PP << (9 * 2));  	//Set Alternate Function Push Pull Mode
	
	GPIOE->AFR[1] &= ~((0x0F) << ((9-8) * 4));			//Clear Alternate Function for pin (lower ARF register)
	GPIOE->AFR[1] |= (GPIO_AF1_TIM1 << ((9-8) * 4));	//Set Alternate Function for pin (lower ARF register) 

    // Timer 1 clock enable
	__TIM1_CLK_ENABLE();
    	
    TIM1->PSC = ((SystemCoreClock/2) / TIMER_COUNTER_FREQ) - 1;
    // Counting direction: 0 = up-counting, 1 = down-counting (Timer Control Register 1)
    TIM1->CR1 &= ~TIM_CR1_DIR; 

    TIM1->ARR = PWM_PULSE_WIDTH_TICKS; 
    TIM1->CCR1 = PWM_PERCENT2TICKS_DUTYCYCLE(50);   //INITIALISATION VALUE - equivalent to 2.5ms p/w

    TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 	// Clear OC1M (Channel 1) 
	TIM1->CCMR1 |= (0x6 << 4); 		// Enable PWM Mode 1, upcounting, on Channel 1
	TIM1->CCMR1 |= (TIM_CCMR1_OC1PE);  // Enable output preload bit for channel 1

 	TIM1->CR1  |= (TIM_CR1_ARPE); 	// Set Auto-Reload Preload Enable 
	TIM1->CCER |= TIM_CCER_CC1E; 	// Set CC2E Bit
	TIM1->CCER &= ~TIM_CCER_CC1NE; 	// Clear CC2NE Bit for active high output
	
	
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 

	TIM1->CR1 |= TIM_CR1_CEN;	// Enable the counter   
}

/**
  * @brief Set the brightness level of the RGB LED.
  * @param level: The brightness level (0% to 100%).
  * @retval None
  */
void s4824063_reg_rgb_brightness_write(int level){
    int dutyCyclePercent = level / 2;

    PWM_DUTYCYCLE_CHANGE(PWM_PERCENT2TICKS_DUTYCYCLE(dutyCyclePercent));
}


/**
  * @brief Read the brightness level of the RGB LED.
  * @param None
  * @retval The current brightness level (0% to 100%).
  */
int s4824063_reg_rgb_brightness_read(){

    int regValue = TIM1->CCR1; 
    int brightnessPercent = regValue / 5; 

    return brightnessPercent;
}
/**
  * @brief Set the primary color of the RGB LED.
  * @param rgb_mask: The RGB bit mask (< R >< G >< B >).
  * @retval None
  */
void s4824063_reg_rgb_colour_set(unsigned char rgb_mask) {
    //Set Red
    switch (rgb_mask & 0x04) {
        case 0x04:
            GPIOE->ODR |= (0x01 << 11); // Turn red LED off
            break;
        default:
            GPIOE->ODR &= ~(0x01 << 11); // Turn red LED on (active low)
            break;
    }
    
    // Set Green
    switch (rgb_mask & 0x02) {
        case 0x02:
            GPIOF->ODR |= (0x01 << 14); // Turn green LED off
            break;
        default:
            GPIOF->ODR &= ~(0x01 << 14); // Turn green LED on (active low)
            break;
    }
    
    // Set Blue
    switch (rgb_mask & 0x01) {
        case 0x01:
            GPIOF->ODR |= (0x01 << 15); // Turn blue LED off
            break;
        default:
            GPIOF->ODR &= ~(0x01 << 15); // Turn blue LED on (active low)
            break;
    }
}
