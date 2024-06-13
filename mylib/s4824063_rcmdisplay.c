#include "s4824063_rcmdisplay.h"
#include "s4824063_console.h"
#include "s4824063_rgb.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "debug_log.h"

#define SCREEN_WIDTH 100
#define SCREEN_HEIGHT 100
#define RCM_HEAD_SYMBOL '+'

static int prev_x = -1, prev_y = -1;  // Initialize with invalid values

/**
 * @brief Task for Remote Control Module Display.
 * 
 * This task receives payloads from the RCM display queue and updates the display accordingly.
 * 
 * @param pvParameters Pointer to task parameters (not used in this function).
 */
void s4824063_rcmdisplay_task(void *pvParameters) {
    rcmDisplayQueue = xQueueCreate(10, sizeof(RcmPayload_t));
    RcmPayload_t rcmPayload;

    for (;;) {
        if (xQueueReceive(rcmDisplayQueue, &rcmPayload, portMAX_DELAY) == pdTRUE) {
            // Check if RCM head position has changed
            if (rcmPayload.x != prev_x || rcmPayload.y != prev_y) {
                // Clear the screen
                debug_log("\e[2J");

                // Position the cursor at the origin (bottom-left corner)
                debug_log("\e[%d;%dH", SCREEN_HEIGHT, 1);

                // Draw the outline/boundary of the grid
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    debug_log(".");
                }
                debug_log("\n\r");

                for (int y = 1; y < SCREEN_HEIGHT - 1; y++) {
                    debug_log(".");
                    for (int x = 1; x < SCREEN_WIDTH - 1; x++) {
                        if (x == rcmPayload.x && SCREEN_HEIGHT - y == rcmPayload.y) {
                            debug_log("%c", RCM_HEAD_SYMBOL);
                        } else {
                            debug_log(" ");
                        }
                    }
                    debug_log(".\n\r");
                }

                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    debug_log(".");
                }
                debug_log("\n\r");

                // Update previous RCM head position
                prev_x = rcmPayload.x;
                prev_y = rcmPayload.y;
            }
           
        }
    }
}