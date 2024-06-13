#include "board.h"
#include "processor_hal.h"
#include "s4824063_mfs_pb.h"
#include "s4824063_console.h"
#include "s4824063_sysmon.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"


void hardware_init(void);
void Task1_Task( void );
void Task2_Task( void );
void Task3_Task( void );

// Task Priorities (Idle Priority is the lowest priority)
#define TASK1_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TASK2_PRIORITY					( tskIDLE_PRIORITY + 2 )
#define TASK3_PRIORITY					( tskIDLE_PRIORITY + 2)

// Task Stack Allocations (must be a multiple of the minimal stack size)
#define TASK1_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TASK2_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )
#define TASK3_STACK_SIZE		( configMINIMAL_STACK_SIZE * 2 )


TaskHandle_t task2_handle = NULL;
TaskHandle_t task1_handle = NULL;
TaskHandle_t task3_handle = NULL;


int main( void ) {
   
  hardware_init();


	HAL_Init();		//Only HAL_Init() must be called before task creation.

	//
	//Only functions that create tasks must be called in main.
	//

	// Start the task to flash the LED.
   xTaskCreate( (void *) &Task3_Task, (const signed char *) "T3", TASK3_STACK_SIZE, NULL, TASK3_PRIORITY, &task3_handle );
    xTaskCreate( (void *) &Task1_Task, (const signed char *) "T1", TASK1_STACK_SIZE, NULL, TASK1_PRIORITY, &task1_handle );
	//xTaskCreate( (void *) &Task2_Task, (const signed char *) "T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, &task2_handle );

	/* Start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */

	vTaskStartScheduler();


  	return 0;
}
/*
 * Task 1.
 */
void Task1_Task( void ) {

	hardware_init() ;

	SYSMON_CHAN0_CLR();				//Clear LA Channel 0

	for (;;) {

		SYSMON_CHAN0_SET();			//Set LA Channel 0	
		BRD_LEDGreenToggle();		//Random instruction	

		vTaskDelay(3);			//Extra Delay for 3ms	
		SYSMON_CHAN0_CLR();			//Clear LA Channel 0	
        			//Extra Delay for 3ms	

	}

}

/*
 * Task 2.
 */
void Task2_Task( void ) {

 hardware_init() ;

	SYSMON_CHAN1_CLR();				//Clear LA Channel 1

	for (;;) {

		SYSMON_CHAN1_SET();			//Set LA Channel 1	
		BRD_LEDGreenToggle();		//Random instruction	

		vTaskDelay(3);				//Extra Delay for 3ms	
		SYSMON_CHAN1_CLR();			//Clear LA Channel 1	
      
	}
  
}

/*
 * Task3.
 */
void Task3_Task( void ) {
    hardware_init() ;

    SYSMON_CHAN2_CLR();				//Clear LA Channel 2
    for (;;) {
        if (pb_semaphore!=NULL)
        {
            if (xSemaphoreTake( pb_semaphore , 10 ) == pdTRUE )
            {
                if (s4824063_reg_mfs_pb_press_get(1) > 0){
                    if (task2_handle != NULL && task2_handle != xTaskGetCurrentTaskHandle()) {
                        // Delete Task2 if it exists and is not the current task
                        vTaskDelete(task2_handle);
                        task2_handle = NULL;
                    } else if (task2_handle == NULL) {
                        // Create Task2 if it doesn't exist
                        xTaskCreate((void *)&Task2_Task, (const signed char *)"T2", TASK2_STACK_SIZE, NULL, TASK2_PRIORITY, &task2_handle);
                    }
                    s4824063_reg_mfs_pb_press_reset(1);
                }
                xSemaphoreGive(pb_semaphore);
            }
        }

        SYSMON_CHAN2_SET();			//Set LA Channel 2	
        BRD_LEDGreenToggle();		//Random instruction	
        SYSMON_CHAN2_CLR();			//Clear LA Channel 2
    }
}



void hardware_init(void) {
    taskENTER_CRITICAL();
    BRD_LEDInit();				//Initialise Green LED
	BRD_LEDGreenOff();
  s4824063_reg_mfs_pb_semaphore_init();
    s4824063_sysmon_init();
   s4824063_reg_mfs_pb_init(1);

    taskEXIT_CRITICAL();
}