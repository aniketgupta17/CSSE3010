#ifndef S4824063_TXRADIO_H
#define S4824063_TXRADIO_H

 

#include "board.h"
#include "processor_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "nrf24l01plus.h" // periphery source code
#include "debug_log.h"

typedef enum {
    INIT,
    ENCODING,
    TRANSMIT
} txType_t;
extern txType_t txtype;

#define TASK_RADIO_PACKET_SIZE 16 // change accordingly
#define ENCODED_RADIO_PACKET_SIZE 32 // hamming encoded


/* State Enumerating -----------------------------------------*/
// SHOULD SEND VIA EVENT BITS?? 

/* RTOS Functions -----------------------------------------*/
 void s4824063_txradio_init();
void s4824063_tsk_txradio();
#endif