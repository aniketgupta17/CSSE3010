/**   
******************************************************************************   
* @file mylib/ s4824063_mfs_pb.c
* @author s48240639
* @date 28/02/2024
* @brief MFS PUSH BUTTON
* REFERENCE: examples/getting-started/blink, examples/gpio/interrupt, templates/mylib
*
******************************************************************************   
*     EXTERNAL FUNCTIONS
******************************************************************************
* s4824063_reg_mfs_pb_init(int pb_select) - intialise MFS push button
* s4824063_reg_mfs_pb_isr(int pb_select) - MFS interrupt service routine
* s4824063_reg_mfs_pb_press_get(int pb_select) - Returns the value of pushbutton press counter
* s4824063_reg_mfs_pb_press_reset(int pb_select) - Reset the value of pushbutton event counter to 0
******************************************************************************   
*/

#include "s4824063_mfs_pb.h"
#include "s4824063_rgb.h"
#include "s4824063_lta1000g.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
 

// Define global array variable to hold current MFS pressed count
int MfPbPressCounter[3] = {0};
static uint32_t previous_time = 0;

SemaphoreHandle_t pb_semaphore;

void pb_callback(uint16_t GPIO_Pin);
// Initialize the specified pushbutton source
/**
  * @brief Initialize the specified MFS push button.
  * @param pb_select: The selection of the MFS push button.
  * @retval None
  */
void s4824063_reg_mfs_pb_init(int pb_select) {
    
    // Enable the GPIO clock
    __GPIOC_CLK_ENABLE();
 
    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 0 );    // Set fast speed for pin 0.
    GPIOC->PUPDR &= ~(0x03 << (0 * 2));                // Clear bits for no pull-up/pull-down for pin 0.
    GPIOC->MODER &= ~(0x03 << (0 * 2));   
    
    // Enable EXTI clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
    // Select trigger source (port C, pin 0) on EXTICR4.
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;

    EXTI->RTSR |= EXTI_RTSR_TR0;    // Enable rising edge trigger for pin 0.
    EXTI->FTSR &= ~EXTI_FTSR_TR0;   // Disable falling edge trigger for pin 0.
    EXTI->IMR |= EXTI_IMR_IM0;      // Enable interrupt for pin 0.

    // Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
    HAL_NVIC_SetPriority(EXTI0_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
    
}


void EXTI0_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI0_IRQn);

	// PR: Pending register
if ((EXTI->PR & EXTI_PR_PR0) == EXTI_PR_PR0) { // Check if switch signal is high
            EXTI->PR |= EXTI_PR_PR0;  //Clear interrupt flag.
            MfPbPressCounter[0]++; // Increment press counter
            pb_callback(0);
        }
}
/**
  * @brief MFS interrupt service routine for the push button.
  * @param pb_select: The selection of the MFS push button.
  * @retval None
  */
void s4824063_reg_mfs_pb_isr(int pb_select) {

    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    // Debounce the pushbutton press
    uint32_t current_time = HAL_GetTick();

    if ((current_time - previous_time) > 10) {
        EXTI0_IRQHandler();
    }
    previous_time = current_time;
    
}

void pb_callback(uint16_t GPIO_Pin) {
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 0) {
		
		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

		if (pb_semaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR( pb_semaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	}
	
}
/**
  * @brief Get the value of the push button press counter.
  * @param pb_select: The selection of the MFS push button.
  * @retval The value of the push button press counter.
  */
int s4824063_reg_mfs_pb_press_get(int pb_select) {

    return MfPbPressCounter[0];
}
/**
  * @brief Reset the value of the push button press counter to 0.
  * @param pb_select: The selection of the MFS push button.
  * @retval None
  */
void s4824063_reg_mfs_pb_press_reset(int pb_select) {

    MfPbPressCounter[0] = 0;
}

// Semaphore initialization
void s4824063_reg_mfs_pb_semaphore_init(void) {
    pb_semaphore = xSemaphoreCreateBinary();
}