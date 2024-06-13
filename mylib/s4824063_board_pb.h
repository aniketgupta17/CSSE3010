/**
***************************************************************
* @file mylib/ s4824063_board_pb.h
* @author Aniket Gupta - S4824063
* @date 28/02/2024
* @brief board PUSH BUTTON
* REFERENCE: 
***************************************************************
* EXTERNAL FUNCTIONS
***************************************************************
* s4824063_reg_board_pb_init(int pb_select) - intialise board push button
* s4824063_reg_board_pb_isr(int pb_select) - board interrupt service routine
* s4824063_reg_board_pb_press_get(int pb_select) - Returns the value of pushbutton press counter
* s4824063_reg_board_pb_press_reset(int pb_select) - Reset the value of pushbutton event counter to 0
***************************************************************
*/
#include "FreeRTOS.h"
#include "semphr.h"
#ifndef S4824063_board_PB_H
#define S4824063_board_PB_H

#define S4824063_REG_MFA_PB_S1 (1 << 0)
extern SemaphoreHandle_t board_semaphore;

void s4824063_reg_board_pb_init(int pb_select);
void s4824063_reg_board_pb_isr(int pb_select);
int s4824063_reg_board_pb_press_get(int pb_select);
void s4824063_reg_board_pb_press_reset(int pb_select);
void s4824063_reg_board_pb_semaphore_init(void);
#endif