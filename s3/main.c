#include "board.h"
#include "processor_hal.h"
#include "s4824063_mfs_pb.h"
#include "s4824063_lta1000g.h"
#include "s4824063_console.h"
#include "s4824063_mfs_led.h"
#include "s4824063_hamming.h"

#define DEBUG
#ifdef DEBUG
#include "debug_log.h"
#endif

#define S0  0  // IDLE State
#define S1  1  // Encode State
#define S2  2  // Decode State

typedef struct {
    int nextState;
    char input1;
    char input2;
    const char* message;
} Transition;

// Define transitions for each state
Transition transitions[] = {
    {S0, '+', '+', " We are in the idle State again!\n\r"},
    {S1, '#', '#', " We are in the Encode State!\n\r"},
    {S2, '$', '$', " We are in the Decode State!\n\r"},
    {S0, '!', '!', " We are in the Idle State!\n\r"},
    {S1, '&', '&', " We are in the Encode State again!\n\r"},
    {S2, '*', '*', " We are in the Decode State!\n\r"},
    {S0, '%', '%', " We are in the Idle State!\n\r"},
    {S1, '@', '@', " We are in the Encode State!\n\r"},
    {S2, '?', '?', " We are in the Idle State!\n\r"}
};

int flag = 0;
int currentState = S0;
int encodeStateLoggedOnce = 0;
int decodeStateLoggedOnce = 0;
int idleStateLoggedOnce = 0;


/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Process the state transition based on current state and input characters.
  * @param   current_state: Current state of the finite state machine.
  * @param   input1: First input character.
  * @param   input2: Second input character.
  * @retval  The next state of the finite state machine.
  */
int fsm_processing(int current_state, char input1, char input2) {
    int next_state = current_state;
    int i;
// Iterate over the transition table to find a matching transition
    for (i = 0; i < sizeof(transitions) / sizeof(Transition); i++) {
         // Check if the input characters match the transition conditions
        if (input1 == transitions[i].input1 && input2 == transitions[i].input2) {
            next_state = transitions[i].nextState;
            // Log transition message if certain conditions are met
            if (next_state == S1 && encodeStateLoggedOnce == 0) {
                debug_log("%s", transitions[i].message);
                encodeStateLoggedOnce = 1;
            }
            if (next_state == S2 && decodeStateLoggedOnce == 0) {
                debug_log("%s", transitions[i].message);
                decodeStateLoggedOnce = 1;
            }
            if (next_state == S0 && idleStateLoggedOnce == 0) {
                debug_log("%s", transitions[i].message);
                idleStateLoggedOnce = 1;
            }
            break;
        }
    }

    return next_state;
}

int pb_get;


/**
  * @brief   Process state-specific operations based on the current state.
  * @param   current_state: Current state of the finite state machine.
  * @param   in1: First input character.
  * @param   in2: Second input character.
  * @retval  None
  */
void state_processing(int current_state, char in1, char in2) {
    int i;
    uint16_t Word = 0;
    uint8_t encode1 = 0;
    uint8_t input1 = 0;
    uint8_t input2 = 0;
    uint8_t encoded2 = 0;
    uint8_t decode = 0;
    int inputs = 0;
    int displayUpperByte; // Flag to track which byte to display

 // Validate input characters and convert them to hexadecimal if valid
    if (((in1 >= '0' && in1 <= '9') || (in1 >= 'A' && in1 <= 'F') || (in1 >= 'a' && in1 <= 'f')) &&
        ((in2 >= '0' && in2 <= '9') || (in2 >= 'A' && in2 <= 'F') || (in2 >= 'a' && in2 <= 'f'))) {
        input1 = s4824063_lib_console_ascii2hex(in1);
        input2 = s4824063_lib_console_ascii2hex(in2);
        inputs = 2;
    }

 // Handle state transitions and perform encoding or decoding processes
if ((in1 != '#' || in2 != '#') && current_state == S1) {
    // Encoding process for State S1
    encode1 = hamming_hbyte_encode(input1);
    encoded2 = hamming_hbyte_encode(input2);
    Word = (encoded2 << 8) | encode1;
    if (flag == 0 ) {
        #ifdef DEBUG
        debug_log("encode1 %x\n\r", encode1);
        debug_log("encoded2 %x\n\r", encoded2);
        #endif
  
        flag = 1;
    }
} else if ((in1 != '*' || in2 != '*') && current_state == S2) {
    // Decoding process for State S2
    input1 = input1 << 4;
    input1 |= input2 << 0;
    decode = s4824063_lib_hamming_byte_decode(input1);
    if (flag == 0) {
        debug_log("decode %x\n\r", decode);
        flag = 1;
    }
}

// Log encoded words for every input in State S1
    if (current_state == S1) {
        encodeStateLoggedOnce = 1; // Set the flag for the next input
    }

    // Perform state-specific operations
    if (current_state == S1) {
        // Display encoded bytes on LED Bar and toggle LED D4
        if (pb_get % 2 == 1) {
            s4824063_reg_lta1000g_write(encode1);
            s4824063_REG_MFS_LED_D3_ON(); // Indicate upper byte display
        } else {
            s4824063_reg_lta1000g_write(encoded2);
            s4824063_REG_MFS_LED_D3_OFF(); // Indicate lower byte display
        }

        // Toggle the MFS D4 LED when a byte is received from the console
        if (inputs == 2) {
            MFS_LED_D4_TOGGLE();
            inputs = 0;
        }
    } else if (current_state == S2) {
        
        // Display decoded bytes on LED Bar and set error LEDs
        if (pb_get % 2 == 1) {
            s4824063_reg_lta1000g_write(decode >> 4); // Display upper byte
            s4824063_REG_MFS_LED_D3_ON(); // Indicate upper byte display
        } else {
            s4824063_reg_lta1000g_write(decode & 0x00FF); // Display lower byte
            s4824063_REG_MFS_LED_D3_OFF(); // Indicate lower byte display
        }

        // Set the MFS D1 LED for 1-bit error
        if (s4824063_lib_hamming_parity_error(input1)) {
            s4824063_REG_MFS_LED_D1_ON();
        } else {
            s4824063_REG_MFS_LED_D1_OFF();
        }

        // Set the MFS D2 LED for 2-bit error
        if (s4824063_lib_hamming_byte_decode(input1) != decode) {
            s4824063_REG_MFS_LED_D2_ON();
        } else {
            s4824063_REG_MFS_LED_D2_OFF();
        }
        if (inputs == 2) {
            MFS_LED_D4_TOGGLE();
            inputs = 0;
        }
    }
}

void MFS_LED_D4_TOGGLE(void){
    s4824063_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D4_ON();
            HAL_Delay(500);
            s4824063_REG_MFS_LED_D4_OFF();
            HAL_Delay(500);
}

/**
  * @brief   Main function of the application.
  * @param   None
  * @retval  None
  */

void main(void) {
    s4824063_reg_mfs_led_init();
    s4824063_reg_lta1000g_init();
    s4824063_reg_mfs_pb_init(1); // Initialize pushbutton
   
    int inputs = 0;
    uint32_t prev_tick;
    prev_tick = 0;
    char char1, char2;
    int flag2 = 0;

    HAL_Init(); // Initialise Board

#ifdef DEBUG
    BRD_debuguart_init(); // Initialise UART for debug log output
#endif

    /* Main processing loop */
    while (1) {
        if ((HAL_GetTick() - prev_tick) >= 50) {
            prev_tick = HAL_GetTick();
            pb_get = s4824063_reg_mfs_pb_press_get(1); // Check pushbutton state
            // Read input characters if inputs are not pending
            if (inputs == 0) {
                char1 = BRD_debuguart_getc();
                if (char1 != '\0') {
                    inputs = 1;
                    #ifdef DEBUG
                    debug_log("char1 %c\n\r", char1);
                    #endif
                }
            }

            // Read the second character if the first one has been read
            if (inputs == 1) {
    char2 = BRD_debuguart_getc();
    if (char2 != '\0') {
        inputs = 0;
        #ifdef DEBUG
        debug_log("char2 %c\n\r", char2);
        #endif 
        flag = 0;
        encodeStateLoggedOnce = 0; // Reset flags for the next state
        decodeStateLoggedOnce = 0;
        idleStateLoggedOnce = 0;
    }
}
        }
        // Process input characters if both are received
        if ((char1 != '\0') && (char2 != '\0')) {
            currentState = fsm_processing(currentState, char1, char2);
            
            if (flag2 == 0) {
                debug_log("current state = %c\n\r", currentState + '0');
                flag2 = 1;
            }
        }
        // Process state if input characters are received and current state is valid
        if (((currentState == S1) || (currentState == S2)) && (inputs == 0)) {
            state_processing(currentState, char1, char2);
        } 
    }
}