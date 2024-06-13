#ifndef s4824063_RCMCONT_H
#define s4824063_RCMCONT_H

#define IDLE_STATE 0
#define ROTATE_STATE 1
#define MOVE_STATE 2
#define ZOOM_STATE 3
#include "s4824063_console.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "semphr.h"
#include "queue.h"



typedef enum {
    IDLE,
    MOVE,
    ROTATE,
    ZOOM
} CommandType_t;
extern QueueHandle_t packetQueue; 
void s4824063_rcmcont_task();

#endif