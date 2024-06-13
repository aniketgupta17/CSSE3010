#include "s4824063_rcmcont.h"
#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus.h"
#include "s4824063_mfs_led.h"
#include "s4824063_console.h"
#include "s4824063_rgb.h"
#include "s4824063_lta1000g.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
#include "debug_log.h"

#define MOVE_EVENT    (1 << 0)
#define ROTATE_EVENT  (1 << 1)
#define ZOOM_EVENT    (1 << 2)
#define JOIN_EVENT    (1 << 3)
#define EVENTS        (MOVE_EVENT | ROTATE_EVENT | ZOOM_EVENT | JOIN_EVENT)
uint8_t unencoded_packet[16] = {0};
uint8_t my_address[4] = {0x48,0x24,0x06,0x39};
QueueHandle_t packetQueue;
QueueHandle_t rcmDisplayQueue;


/**
 * @brief Finite State Machine for Remote Control Module Control.
 * 
 * This function implements a finite state machine for controlling the Remote Control Module (RCM).
 * It takes the current state of the RCM as input and performs actions accordingly.
 * 
 * @param current_state The current state of the RCM.
 */
void s4824063_fsm_rcmcont(CommandType_t current_state) {
    switch (current_state) {
        case IDLE:
            //debug_log("Idle State!\n\r");
            Packet(IDLE);
            break;
        case MOVE:
            //debug_log("Move State!\n\r");
            Packet(MOVE);
            debug_log("%d %d %d\n\r", rcmPayload.x, rcmPayload.y, rcmPayload.z);
        
            break;
        case ROTATE:
            //debug_log("Rotate State!\n\r");
            Packet(ROTATE);
            debug_log("%d\n\r", rcmPayload.angle);
            s4824063_reg_lta1000g_write(rcmPayload.angle);
            break;
        case ZOOM:
           // debug_log("Zoom State!\n\r");
            Packet(ZOOM);
            debug_log("%d\n\r", rcmPayload.zoomFactor);
            break;
    }
    xQueueSend(packetQueue, ( void * ) &unencoded_packet, 2);
}

/**
 * @brief Task for Remote Control Module Control.
 * 
 * This task listens for events related to remote control commands,
 * processes them using the finite state machine, and sends payloads to a display queue.
 */

void s4824063_rcmcont_task() {
    // RcmPayload_t rcmPayload;
    BRD_debuguart_init();
    rcmDisplayQueue = xQueueCreate(10, sizeof(RcmPayload_t));
    // s4824063_reg_mfs_led_init();
    commandEventGroup = xEventGroupCreate();
    
    for (;;) {
        EventBits_t bits = xEventGroupWaitBits(commandEventGroup, EVENTS, pdTRUE, pdFALSE, 3);
        
        if (bits & MOVE_EVENT) {
            s4824063_fsm_rcmcont(MOVE);
            xQueueSend(rcmDisplayQueue, &rcmPayload, 0);
            
        } else if (bits & ROTATE_EVENT) {
            s4824063_fsm_rcmcont(ROTATE);
            xQueueSend(rcmDisplayQueue, &rcmPayload, 0);

        } else if (bits & ZOOM_EVENT) {
            s4824063_fsm_rcmcont(ZOOM);
            xQueueSend(rcmDisplayQueue, &rcmPayload, 0);
        }else if (bits &JOIN_EVENT)
        {
            s4824063_fsm_rcmcont(IDLE);
        }
        
    }
}


/**
 * @brief Generate packet based on command type.
 * 
 * This function generates a packet based on the command type provided.
 * 
 * @param com The command type.
 */
void Packet(CommandType_t com){
    for (int i = 1; i < 16; i++)
    {
        unencoded_packet[i] = 0x00;
    }
     for (int i = 0; i < 4; i++) {
        unencoded_packet[i + 1] = my_address[i];
    }

    switch (com)
    {
    case IDLE:
     // Generate JOIN packet
      unencoded_packet[0] = 0x20;
      unencoded_packet[5] = 'J';
      unencoded_packet[6] = 'O';
      unencoded_packet[7] = 'I';
      unencoded_packet[8] = 'N';
      
    break;
    case MOVE:
    // Generate MOVE packet
        unencoded_packet[0] = 0x22;
        unencoded_packet[5] = 'X';
        unencoded_packet[6] = 'Y';
        unencoded_packet[7] = 'Z';
        // Encode payload values
        unencoded_packet[8] = rcmPayload.x/100 + '0';
        unencoded_packet[9] = (rcmPayload.x/10)%10 + '0';
        unencoded_packet[10] = rcmPayload.x%10 + '0';
        unencoded_packet[11] = rcmPayload.y/100 + '0';
        unencoded_packet[12] = (rcmPayload.y/10)%10 + '0';
        unencoded_packet[13] = rcmPayload.y%10 + '0';
        unencoded_packet[14] = rcmPayload.z/10 + '0';
        unencoded_packet[15] = rcmPayload.z%10 + '0';           
    break;
    case ROTATE:
       // Generate ROTATE packet
       unencoded_packet[0] = 0x23;
        unencoded_packet[5] = 'R';
        unencoded_packet[6] = 'O';
        unencoded_packet[7] = 'T';  
        // Encode angle value
        unencoded_packet[8] = rcmPayload.angle/100 + '0';
        unencoded_packet[9] = (rcmPayload.angle/10)%10 + '0';
        unencoded_packet[10] = rcmPayload.angle%10 + '0';           
    break;
    case ZOOM:
       // Generate ZOOM packet
        unencoded_packet[0] = 0x25;
        unencoded_packet[5] = 'Z';
        unencoded_packet[6] = 'O';
        unencoded_packet[7] = 'O';
        unencoded_packet[8] = 'M';
        // Encode zoom factor
        unencoded_packet[9] = rcmPayload.zoomFactor + '0';

    break;
    }
   
}