#include "s4824063_txradio.h"
#include "s4824063_rcmcont.h"
#include "s4824063_console.h"
#include "s4824063_hamming.h"
#include "s4824063_board_pb.h"
#include "s4824063_mfs_led.h"
#include "board.h"
#include "processor_hal.h"
#include "nrf24l01plus.h"
#include "myconfig.h"
#include "debug_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "debug_log.h"

// Define the transmission type state
txType_t txtype;
// Define arrays for received and encoded packets
uint8_t recv_unencoded_packet[16] = {0};
uint8_t encoded_packet[32]; // Increase size to accommodate encoded packet



/**
 * @brief Initialize the transmission radio.
 * 
 * This function sets up the nRF24L01+ radio for transmission by configuring 
 * various registers and settings such as addresses, payload width, channel, 
 * and power settings.
 */
void s4824063_txradio_init(){
     
    BRD_delayInit();            // Enable uS Delay

    nrf24l01plus_spi_init();    // Enable SPI

    // Set CE low for idle state
    NRF_CE_LOW();

    // Power down nrf before powering up.
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_TX_ADDR, myradiotxaddr, 5);            // Writes TX_Address to nRF24L01
    nrf24l01plus_wb(NRF24L01P_WRITE_REG | NRF24L01P_RX_ADDR_P0, myradiotxaddr, 5);    //NRF24L01P_TX_ADR_WIDTH);

    nrf24l01plus_wr(NRF24L01P_EN_AA, 0x00);                                          // Disable Auto.Ack
    nrf24l01plus_wr(NRF24L01P_EN_RXADDR, 0x01);                                       // Enable Pipe0
    nrf24l01plus_wr(NRF24L01P_RX_PW_P0, NRF24L01P_TX_PLOAD_WIDTH);     // Select same RX payload width as TX Payload width

    nrf24l01plus_wr(NRF24L01P_RF_CH, MYRADIOCHAN);                                   // Select RF channel
    nrf24l01plus_wr(NRF24L01P_RF_SETUP, 0x06);                                         // TX_PWR:0dBm, Datarate:1Mbps

    nrf24l01plus_wr(NRF24L01P_CONFIG, 0x02); 
}


/**
 * @brief Task for handling the transmission radio.
 * 
 * This task initializes the transmission radio, creates a queue for receiving 
 * packets, and processes the packets by encoding them with Hamming encoding 
 * and transmitting them using the nRF24L01+ radio.
 */
void s4824063_tsk_txradio(){
    s4824063_txradio_init();
    packetQueue = xQueueCreate(50, sizeof(recv_unencoded_packet));// Create a queue for packets
    int next_state;
    for (;;)
    {
        next_state = INIT;
        switch (txtype)
        {
        case INIT:
            if (xQueueReceive(packetQueue, &recv_unencoded_packet, 2))
            {
                next_state = ENCODING;
                
            }
            break;
        case ENCODING:
         // Encode each byte of the received packet using Hamming encoding
            for (int i = 0; i < 16; i++){
            uint16_t encoded_byte = s4824063_lib_hamming_byte_encode(recv_unencoded_packet[i]);
            encoded_packet[i*2] = encoded_byte & 0xFF;
            encoded_packet[i*2 + 1] = (encoded_byte >> 8) & 0xFF;
            }
            next_state = TRANSMIT;
            break;
        case TRANSMIT:
        // Transmit the encoded packet if it is not NULL
            if (encoded_packet != NULL)
            {
                nrf24l01plus_send(encoded_packet);
                s4824063_REG_MFS_LED_D1_TOGGLE();
            
            }
            next_state = INIT;
            break;       
        }
        txtype = next_state;
    }
    
}