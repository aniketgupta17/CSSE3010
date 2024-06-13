#include "board.h"
#include "processor_hal.h"
#include "s4824063_console.h"
#include "s4824063_rcmcont.h"
#include "s4824063_mfs_led.h"
#include "s4824063_rcmdisplay.h"
#include "s4824063_txradio.h"
#include "s4824063_board_pb.h"
#include "s4824063_mfs_pb.h"
#include "s4824063_sysmon.h"
#include "event_groups.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug_log.h"

void hardware_init();
#define TASK1_PRIORITY (tskIDLE_PRIORITY + 1)
#define TASK2_PRIORITY (tskIDLE_PRIORITY + 2)
#define TASK3_PRIORITY (tskIDLE_PRIORITY + 3)
#define TASK4_PRIORITY (tskIDLE_PRIORITY + 1)

#define TASK1_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK2_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK3_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
#define TASK4_STACK_SIZE (configMINIMAL_STACK_SIZE * 6)
static 	int flag =0;
TaskHandle_t task1;
TaskHandle_t task2;
TaskHandle_t task3;
TaskHandle_t task4;

int main(void){
    HAL_Init();
    BRD_debuguart_init();
    s4824063_reg_mfs_led_init();
    hardware_init();
    commandEventGroup = xEventGroupCreate();
    // Create tasks
    xTaskCreate((void *)&s4824063_lib_console_tsk_processInput, (const signed char *)"T1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, &task1);
    xTaskCreate((void *)&s4824063_rcmcont_task, (const signed char *)"T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, &task2);
    xTaskCreate((void *)&s4824063_tsk_txradio, (const signed char *)"T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, &task3);
	xTaskCreate((void *)&s4824063_rcmdisplay_task, (const signed char *)"T4", TASK4_STACK_SIZE, NULL, TASK4_PRIORITY, &task4);

    // Start the scheduler
    vTaskStartScheduler();
 
    return 0;
} 

void hardware_init(){
	taskENTER_CRITICAL();
   s4824063_txradio_init();
   s4824063_reg_mfs_pb_init(1);
   s4824063_reg_board_pb_init(1);
   s4824063_reg_mfs_led_init();
s4824063_reg_rgb_init();
s4824063_reg_lta1000g_init();

   taskEXIT_CRITICAL();
}
