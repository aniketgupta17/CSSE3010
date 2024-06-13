/**
  ******************************************************************************
  * @file    s2/main.c
  * @author  Aniket Gupta 
  * @date    07032024
  * @brief Create a circuit using the CD4019B AND/OR gate to control the colour and brightness of the RGB LED   
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "s4824063_mfs_pb.h"
#include "s4824063_mfs_trimpot.h"
#include "s4824063_rgb.h"
#include "s4824063_lta1000g.h"

/* Private defines -----------------------------------------------------------*/
#define PWM_MAX_DUTY_CYCLE 100 
#define PWM_MIN_DUTY_CYCLE 0
#define PWM_DUTY_CYCLE_STEP 10

/* Global variables ----------------------------------------------------------*/
int rgbColorIndex = 7; 
int trimpotTurns = 0; 
int brightnessLevel = 0;


const unsigned char colorSequence[8] = {0x03, 0x05, 0x06, 0x04, 0x02, 0x01, 0x00, 0x07};// Color sequence table


void updateRGBColor(); // Function to update the RGB color based on the color sequence table
void updateBrightnessLevel(); // Function to update the brightness level based on trimpot turns
int turns_count(int adc_value); // Function to count the number of turns of the trimpot

/* Main ----------------------------------------------------------------------*/
int main() {
    // Initialize RGB LED, MFS Trimpot, MFS Pushbutton, and LTA1000G LED Bar
    s4824063_reg_rgb_init();
    s4824063_reg_mfs_trimpot_init();
    s4824063_reg_mfs_pb_init(1);
    s4824063_reg_lta1000g_init();
    s4824063_reg_rgb_colour_set(0x07);

   while (1) {
    // Retrieve the current count of pushbutton presses from the MFS pushbutton module
    int pressCounter = s4824063_reg_mfs_pb_press_get(1);
    // Retrieve the current analog value from the trimpot connected to the MFS module
    int adc_value = s4824063_reg_mfs_trimpot_get();
    // Calculate the number of complete turns of the trimpot shaft based on the analog value
    int turns = turns_count(adc_value) % 11;

    // Check if the pushbutton has been pressed
    if (pressCounter > 0) {
        // Increment the RGB color index to change the color
        updateRGBColor();
        // Reset the pushbutton press counter to zero after processing
        s4824063_reg_mfs_pb_press_reset(1); 
    }

    // Adjust the brightness level of the RGB LED based on the number of turns of the trimpot
    updateBrightnessLevel(turns);

    // Write the current number of turns to the LTA1000G LED bar for visual feedback
    s4824063_reg_lta1000g_write(turns);
}
    return 0;

    
}
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Update RGB color based on the color sequence table.
  * @param   None
  * @retval  None
  */
void updateRGBColor() {
    // Increment the RGB color index circularly within the range of 0 to 7
    rgbColorIndex = (rgbColorIndex + 1) % 8;
    // Set the RGB color based on the updated color index
    s4824063_reg_rgb_colour_set(colorSequence[rgbColorIndex]);
}

/**
  * @brief   Update brightness level based on trimpot turns.
  * @param   trimpotTurns: The number of turns of the trimpot.
  * @retval  None
  */
void updateBrightnessLevel(int trimpotTurns) {
    // Calculate the brightness level by multiplying the number of trimpot turns by 10
    brightnessLevel = trimpotTurns * 10; // Each turn increases brightness by 10%
    // Ensure that the brightness level does not exceed the maximum PWM duty cycle
    if (brightnessLevel > PWM_MAX_DUTY_CYCLE) {
        // If the brightness level exceeds the maximum, reset it to 0
        brightnessLevel = 0;
    }
    // Update the brightness level of the RGB LED
    s4824063_reg_rgb_brightness_write(brightnessLevel);
}
