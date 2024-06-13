/**
***************************************************************
* @file mylib/ s4824063_mfs_pb.h
* @author Aniket Gupta - S4824063
* @date 28/02/2024
* @brief MFS PUSH BUTTON
* REFERENCE: 
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4824063_reg_mfs_pb_init(int pb_select) - intialise MFS push button
* s4824063_reg_mfs_pb_isr(int pb_select) - MFS interrupt service routine
* s4824063_reg_mfs_pb_press_get(int pb_select) - Returns the value of pushbutton press counter
* s4824063_reg_mfs_pb_press_reset(int pb_select) - Reset the value of pushbutton event counter to 0
***************************************************************
*/
#include "FreeRTOS.h"
#include "semphr.h"
#ifndef S4824063_MFS_PB_H
#define S4824063_MFS_PB_H

#define S4824063_REG_MFA_PB_S1 (1 << 0)
extern SemaphoreHandle_t pb_semaphore;

void s4824063_reg_mfs_pb_init(int pb_select);
void s4824063_reg_mfs_pb_isr(int pb_select);
int s4824063_reg_mfs_pb_press_get(int pb_select);
void s4824063_reg_mfs_pb_press_reset(int pb_select);
void s4824063_reg_mfs_pb_semaphore_init(void);
#endif