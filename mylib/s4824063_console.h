/**   
 ******************************************************************************   
* @file mylib/ s4824063_console.h
* @author Aniket Gupta  - s48240639
* @date 23/03/2024
* @brief Console
* REFERENCE:   
 *
 ******************************************************************************   
 *     EXTERNAL FUNCTIONS
 ******************************************************************************
 int s4824063_lib_console_ascii2hex(char value)-External function to convert an ASCII hexadecimal value value to a (returned) binary hexadecimal e.g. ’C’ to 0xC.
 ******************************************************************************   
 */

#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"

#ifndef S4824063_CONSOLE_H
#define S4824063_CONSOLE_H
// Combined Payload Structure
#define MOVE_EVENT (1 << 0)
#define ROTATE_EVENT (1 << 1)
#define ZOOM_EVENT (1 << 2)
#define JOIN_EVENT (1 << 3)

#define TASK1_PRIORITY (tskIDLE_PRIORITY + 1)
#define TASK2_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK3_PRIORITY (tskIDLE_PRIORITY + 3)
#define TASK4_PRIORITY (tskIDLE_PRIORITY + 2)

#define TASK1_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK2_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK3_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK4_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)

typedef struct {
    uint16_t x;
    int16_t y;
    int16_t z;
    int16_t angle;
    int16_t zoomFactor;
} RcmPayload_t;

extern QueueHandle_t consoleQueue; // External reference to the queue
extern EventGroupHandle_t commandEventGroup;
extern RcmPayload_t rcmPayload;

extern TaskHandle_t task1;
extern TaskHandle_t task2;
extern TaskHandle_t task3;
extern TaskHandle_t task4;
extern TaskHandle_t task5;

int s4824063_lib_console_ascii2hex(char value);
void s4824063_lib_console_tsk_display(void);
void s4824063_lib_console_tsk_processInput(void);

#endif