/**   
 ******************************************************************************   
* @file mylib/ s4824063_console.c
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





/*
@brief Convert ASCII hexadecimal value to binary hexadecimal.
 * @param value: The ASCII hexadecimal value to be converted.
 * @retval The binary hexadecimal equivalent of the ASCII value.
 */

#include "board.h"
#include "processor_hal.h"
#include "s4824063_console.h"
#include "s4824063_mfs_led.h"
#include "s4824063_board_pb.h"
#include "s4824063_txradio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "debug_log.h"
#include "s4824063_rgb.h"

#define CONSOLE_WIDTH 30
// Define the single payload variable
RcmPayload_t rcmPayload = {0, 0, 0, 0, 0};

QueueHandle_t consoleQueue;
EventGroupHandle_t commandEventGroup;

static int cursor_position = 0;
static int flag = 0;
char recvchar;

int s4824063_lib_console_ascii2hex(char value) {
   int hex_value = 0;
    if (value >= '0'&&value <= '9'){hex_value += value - '0';}
    else if (value >= 'A'&&value<='F'){hex_value += (value - 'A' + 10);}
    else if (value >= 'a'&&value<='f'){hex_value += (value - 'a' + 10);}
    else{return 0;}
    
    return hex_value;

}


/**
 * @brief Task to display characters on the console.
 * 
 * This task displays characters received through a queue on the console.
 */
void s4824063_lib_console_tsk_display(void)
{
    char display_str[2];
   
    for (;;)
    {
        if (xQueueReceive(consoleQueue, &display_str, 10))
        {
            // Display received characters
            debug_putc(display_str);
            cursor_position += 2;
            // Handle cursor position
            if (cursor_position >= CONSOLE_WIDTH)
            {
                cursor_position = cursor_position % CONSOLE_WIDTH;
                debug_putc("\r");
            }
        }
    }
}


 /**
 * @brief Task to process user input from the console.
 * 
 * This task processes user input from the console and sets event flags accordingly.
 */
void s4824063_lib_console_tsk_processInput(void) {
   s4824063_reg_board_pb_init(1);
   s4824063_reg_board_pb_semaphore_init();
   BRD_debuguart_init();
    for (;;) {
        commandEventGroup = xEventGroupCreate();
        recvchar = BRD_debuguart_getc(); 
        // Semaphore acquired
    if (xSemaphoreTake(board_semaphore, 0))
    { 
   
            xEventGroupSetBits(commandEventGroup,JOIN_EVENT);
            flag = 1;
           
    }
    if (flag == 1)
    { 
        // Process user input
        switch (recvchar) {
        case 'q':rcmPayload.x += 2;xEventGroupSetBits(commandEventGroup,MOVE_EVENT);break;
        case 'w': rcmPayload.x -= 2; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'e': rcmPayload.y += 2; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'r': rcmPayload.y -= 2; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 't': rcmPayload.z += 2; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); s4824063_reg_rgb_colour_set(0x06);break;
        case 'y': rcmPayload.z -= 2; xEventGroupSetBits(commandEventGroup,MOVE_EVENT);  s4824063_reg_rgb_colour_set(0x05); break;
        case 'a': rcmPayload.x += 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 's': rcmPayload.x -= 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'd': rcmPayload.y += 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'f': rcmPayload.y -= 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'g': rcmPayload.z += 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT);s4824063_reg_rgb_colour_set(0x06); break;
        case 'h': rcmPayload.z -= 10; xEventGroupSetBits(commandEventGroup,MOVE_EVENT);s4824063_reg_rgb_colour_set(0x05); break;
        case 'z': rcmPayload.x += 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'x': rcmPayload.x -= 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'c': rcmPayload.y += 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'v': rcmPayload.y -= 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT); break;
        case 'b': rcmPayload.z += 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT);s4824063_reg_rgb_colour_set(0x06); break;
        case 'n': rcmPayload.z -= 50; xEventGroupSetBits(commandEventGroup,MOVE_EVENT);s4824063_reg_rgb_colour_set(0x05); break;
        case '1': rcmPayload.zoomFactor += 1; xEventGroupSetBits(commandEventGroup,ZOOM_EVENT); break;
        case '2': rcmPayload.zoomFactor -= 1; xEventGroupSetBits(commandEventGroup,ZOOM_EVENT); break;
        case '3': rcmPayload.angle += 10; xEventGroupSetBits(commandEventGroup,ROTATE_EVENT); break;
        case '4': rcmPayload.angle -= 10; xEventGroupSetBits(commandEventGroup,ROTATE_EVENT); break;
        case '5': // Reset all values
            rcmPayload.x = 0;
            rcmPayload.y = 0;
            xEventGroupSetBits(commandEventGroup,MOVE_EVENT);
            break;
        case '7':
            if (task3 == NULL) {
                xTaskCreate((void *)&s4824063_tsk_txradio, (const signed char *)"T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, &task3);
            }
            break;
        case '8': 
            if (task3 != NULL) {
                vTaskDelete(task3);
                task3 = NULL;
            }; break;
        case '9': 
            display_sys();
            break;

        }

        if(rcmPayload.z <=0){
            s4824063_reg_rgb_colour_set(0x03);   
        }
                s4824063_REG_MFS_LED_D2_TOGGLE();
    }
    }
}


/**
 * @brief Display system information.
 * 
 * This function displays information about the running tasks, including their
 * name, ID, state, priority, and stack size.
 */

void display_sys(void){
    debug_log("\nNAME   ");
            debug_log("s4824063_lib_console_tsk_processInput    ");
            debug_log("s4824063_rcmcont_task    ");
            debug_log("s4824063_tsk_txradio    ");
            debug_log("s4824063_rcmdisplay_task    ");

            debug_log("\nID    ");
            debug_log("T1    ");
            debug_log("T2    ");
            debug_log("T3    ");
            debug_log("T4    ");

            debug_log("\nSTATE   ");
            debug_log("running   ");
            debug_log("running   ");
            if (task3 == NULL) {
                debug_log("paused   ");
            } else {
                debug_log("running   ");
            }
            debug_log("running   ");

            debug_log("\nPriority   ");
            debug_log("%d    ", TASK1_PRIORITY);
            debug_log("%d    ", TASK2_PRIORITY);
            debug_log("%d    ", TASK3_PRIORITY);
            debug_log("%d    ", TASK4_PRIORITY);

            debug_log("\nSTACK   ");
            debug_log("%d    ", TASK1_STACK_SIZE);
            debug_log("%d    ", TASK2_STACK_SIZE);
            debug_log("%d    ", TASK3_STACK_SIZE);
            debug_log("%d    ", TASK4_STACK_SIZE);

}