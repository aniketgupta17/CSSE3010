/**   
 ******************************************************************************   
* @file mylib/s4824063_mfs_trimpot.h
* @author Aniket Gupta  - s4824063
* @date 06/03/2024
* @brief MFS Trimpot 
* REFERENCE:csse3010_mylib_reg_mfs_trimpot.pdf
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
* s4824063_reg_mfs_trimpot_init()-Initialise GPIO pins and ADC
*int_s4824063_reg_mfs_trimpot_get()-Function to read the current Trimpot input value 
 ******************************************************************************   
 */


#ifndef S4824063_MFS_TRIMPOT_H
#define S4824063_MFS_TRIMPOT_H

// Configuration Parameters
#define S4824063_REG_MFS_TRIMPOT_ZERO_CAL_OFFSET

// Function Prototypes
void s4824063_reg_mfs_trimpot_init(void);
int s4824063_reg_mfs_trimpot_get(void);

#endif 