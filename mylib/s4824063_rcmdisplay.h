#ifndef s4824063_RCMDISPLAY_H
#define s4824063_RCMDISPLAY_H

#include "s4824063_console.h"
#include "FreeRTOS.h"

#include "semphr.h"
#include "queue.h"


extern QueueHandle_t rcmDisplayQueue;
void s4824063_rcmdisplay_task(void *pvParameters);
#endif