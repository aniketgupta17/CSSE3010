/**   
 ******************************************************************************   
* @file mylib/s4824063_mfs_trimpot.c
* @author Aniket Gupta  - s4824063
* @date 06/03/2024
* @brief MFS Trimpot 
* REFERENCE: csse3010_mylib_reg_mfs_trimpot.pdf
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
* s4824063_reg_mfs_trimpot_init()-Initialise GPIO pins and ADC
*int_s4824063_reg_mfs_trimpot_get()-Function to read the current Trimpot input value 
 ******************************************************************************   
 */
/* Includes ------------------------------------------------------------------*/
 #include "s4824063_mfs_trimpot.h"
 #include "board.h"
#include "processor_hal.h"

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef AdcHandle;
ADC_ChannelConfTypeDef AdcChanConfig;

/**
  * @brief Initialize GPIO pins and ADC for the Trimpot.
  * @param None
  * @retval None
  */
// Define GPIO pins and ADC initialization function
void s4824063_reg_mfs_trimpot_init(void) {
    // Enable the GPIO Clock
    __GPIOA_CLK_ENABLE();

    // Initialize PA3 as an Analog input.
    GPIOA->MODER |= (0x03 << (3 * 2));            // Set bits for Analog input mode

    GPIOA->OSPEEDR &= ~(0x03<<(3 * 2));
    GPIOA->OSPEEDR |= 0x02<<(3 * 2);              // Fast speed

    GPIOA->PUPDR &= ~(0x03 << (3 * 2));           // Clear bits for no push/pull

    __ADC1_CLK_ENABLE();						//Enable ADC1 clock

	AdcHandle.Instance = (ADC_TypeDef *)(ADC1_BASE);						//Use ADC1 (For PA3)
	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;	//Set clock prescaler
	AdcHandle.Init.Resolution            = ADC_RESOLUTION12b;				//Set 12-bit data resolution
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;	//No Trigger
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;		//No Trigger
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;				//Right align data
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;

   //Initialise ADC
	HAL_ADC_Init(&AdcHandle);		

	// Configure ADC Channel, PA3 has Analog Channel 3 connected
	AdcChanConfig.Channel = ADC_CHANNEL_3;					
	AdcChanConfig.Rank         = 1;
	AdcChanConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	AdcChanConfig.Offset       = 0;

    //Initialise ADC Channel
	HAL_ADC_ConfigChannel(&AdcHandle, &AdcChanConfig);		
    HAL_ADC_Start(&AdcHandle); 

		
		while (HAL_ADC_PollForConversion(&AdcHandle, 10) != HAL_OK);
}

/**
  * @brief Read the current Trimpot input value (raw ADC value).
  * @param None
  * @retval The current Trimpot input value (raw ADC value).
  */

int s4824063_reg_mfs_trimpot_get(void) {

   unsigned int adc_value;
   adc_value = AdcHandle.Instance->DR;	
   return adc_value;
}
/**
  * @brief Calculate the number of turns based on the ADC value.
  * @param adc_value: The ADC value obtained from the Trimpot.
  * @retval The number of turns calculated based on the ADC value.
  */
int turns_count(int adc_value) {
    
    double percentage = (double)adc_value / 1638;
    
   
    int turns = (int)(percentage * 10);

    return turns;
}