/**
  ******************************************************************************
  * @file    s1/main.c
  * @author  Aniket Gupta 
  * @date    28022024
  * @brief   Fibonacci LED Display with MFS Pushbutton Control with heartbeat signal
  ******************************************************************************
  */

#include "board.h"
#include "processor_hal.h" 
#include "s4824063_lta1000g.h"
#include "s4824063_mfs_pb.h"
#include "debug_log.h"


// variables to store Fibonacci values
int x = 0, y = 1, z; 
int counter = 0; //  counter value
volatile  uint32_t  upper_five_bits_inverted = 0; // track if upper five bits are inverted

/**
 * @brief Initialize hardware peripherals
 */
void hardware_init(void){
    __GPIOD_CLK_ENABLE();
// Configure pin PD10 on GPIO port D

// Clear bits in MODER register to configure PD10 as digital output
GPIOD->MODER &= ~(0x03 << (10 * 2));

// Set PD10 for push-pull output
GPIOD->MODER |= (0x01 << (10 * 2));

// Set PD10 for fast speed
GPIOD->OSPEEDR &= ~(0x03 << (10 * 2));
GPIOD->OSPEEDR |= (0x02 << (10 * 2));

// Clear bit for push-pull output
GPIOD->OTYPER &= ~(0x01 << 10);
}

/**
 * @brief Generate the next Fibonacci number
 * @return The next Fibonacci number in the sequence
 */
int Fibonacci() {
    z = x + y;
    x = y;
    y = z;
    return z;
}

/**
 * @brief Main function
 * @return 0 upon successful execution
 */
int main() {
    // Initialize LED bar
    HAL_Init();
    hardware_init();
    BRD_debuguart_init(); 
    s4824063_reg_lta1000g_init();
    s4824063_reg_mfs_pb_init(1);

    while (1) {
GPIOD->ODR ^= (1UL << 10);

        // Get next Fibonacci number
        int fib = Fibonacci();

        // Check if MFS S1 pushbutton is pressed
        if (s4824063_reg_mfs_pb_press_get(1) > 0) {
            // Toggle flag to invert upper five bits
            upper_five_bits_inverted = !upper_five_bits_inverted;
            
            
            // Reset pushbutton press counter
            s4824063_reg_mfs_pb_press_reset(1);
        }

        // If upper five bits should be inverted, invert counter value
        if (upper_five_bits_inverted) {
            fib ^= 0b1111100000; // Invert upper five bits
        }

        // Write Fibonacci number to LED bar
        s4824063_reg_lta1000g_write(fib);

        // Check if Fibonacci number is 610 (F15)
        if (fib >610) {
            // Reset Fibonacci sequence
            x = 0;
            y = 1;
        }

        // Delay for 1 second
        for (volatile uint32_t i = 0; i < 1000000; i++);
    }

    return 0;
}

 