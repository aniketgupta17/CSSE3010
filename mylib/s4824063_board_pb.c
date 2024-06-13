/**   
******************************************************************************   
* @file mylib/ s4824063_board_pb.c
* @author s48240639
* @date 28/02/2024
* @brief board PUSH BUTTON
* REFERENCE: examples/getting-started/blink, examples/gpio/interrupt, templates/mylib
*
******************************************************************************   
*     EXTERNAL FUNCTIONS
******************************************************************************
* s4824063_reg_board_pb_init(int pb_select) - intialise board push button
* s4824063_reg_board_pb_isr(int pb_select) - board interrupt service routine
* s4824063_reg_board_pb_press_get(int pb_select) - Returns the value of pushbutton press counter
* s4824063_reg_board_pb_press_reset(int pb_select) - Reset the value of pushbutton event counter to 0
******************************************************************************   
*/

#include "s4824063_board_pb.h"
#include "s4824063_rgb.h"
#include "s4824063_lta1000g.h"
#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
 

// Define global array variable to hold current board pressed count
int PbPressCounter[3] = {0};
static uint32_t previous_time = 0;

SemaphoreHandle_t board_semaphore;

void board_callback(uint16_t GPIO_Pin);
// Initialize the specified pushbutton source
/**
  * @brief Initialize the specified board push button.
  * @param pb_select: The selection of the board push button.
  * @retval None
  */
void s4824063_reg_board_pb_init(int pb_select) {
    
    // Enable the GPIO clock
    __GPIOC_CLK_ENABLE();
 
    GPIOC->OSPEEDR |= (GPIO_SPEED_FAST << 13 );    // Set fast speed for pin 0.
    GPIOC->PUPDR &= ~(0x03 << (13 * 2));                // Clear bits for no pull-up/pull-down for pin 0.
    GPIOC->MODER &= ~(0x03 << (13 * 2));   
    
    // Enable EXTI clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    
    // Select trigger source (port C, pin 0) on EXTICR4.
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

    EXTI->RTSR |= EXTI_RTSR_TR13;    // Enable rising edge trigger for pin 0.
    EXTI->FTSR &= ~EXTI_FTSR_TR13;   // Disable falling edge trigger for pin 0.
    EXTI->IMR |= EXTI_IMR_IM13;      // Enable interrupt for pin 0.

    // Enable priority (10) and interrupt callback. Do not set a priority lower than 5.
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
}


void EXTI15_10_IRQHandler(void) {

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);

	// PR: Pending register
if ((EXTI->PR & EXTI_PR_PR13) == EXTI_PR_PR13) { // Check if switch signal is high
            EXTI->PR |= EXTI_PR_PR13;  //Clear interrupt flag.
            PbPressCounter[0]++; // Increment press counter
            board_callback(0);
        }
}
/**
  * @brief board interrupt service routine for the push button.
  * @param pb_select: The selection of the board push button.
  * @retval None
  */
void s4824063_reg_board_pb_isr(int pb_select) {

    NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    // Debounce the pushbutton press
    uint32_t current_time = HAL_GetTick();

    if ((current_time - previous_time) > 10) {
        EXTI15_10_IRQHandler();
    }
    previous_time = current_time;
    
}

void board_callback(uint16_t GPIO_Pin) {
	BaseType_t xHigherPriorityTaskWoken;

	if (GPIO_Pin == 0) {
		
		// Is it time for another Task() to run?
		xHigherPriorityTaskWoken = pdFALSE;

		if (board_semaphore != NULL) {	// Check if semaphore exists 
			xSemaphoreGiveFromISR( board_semaphore, &xHigherPriorityTaskWoken );		// Give PB Semaphore from ISR
		}

		// Perform context switching, if required.
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	}
	
}
/**
  * @brief Get the value of the push button press counter.
  * @param pb_select: The selection of the board push button.
  * @retval The value of the push button press counter.
  */
int s4824063_reg_board_pb_press_get(int pb_select) {

    return PbPressCounter[0];
}
/**
  * @brief Reset the value of the push button press counter to 0.
  * @param pb_select: The selection of the board push button.
  * @retval None
  */
void s4824063_reg_board_pb_press_reset(int pb_select) {

    PbPressCounter[0] = 0;
}

// Semaphore initialization
void s4824063_reg_board_pb_semaphore_init(void) {
    board_semaphore = xSemaphoreCreateBinary();
}