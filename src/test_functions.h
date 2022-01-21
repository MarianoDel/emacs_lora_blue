//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.H #########################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _TEST_FUNTIONS_H_
#define _TEST_FUNTIONS_H_


// Module Exported Types Constants and Macros ----------------------------------


// Module Exported Functions ---------------------------------------------------
void TF_Led (void);
void TF_Timer1_OnePulseMode (void);
void TF_Usart1_Tx_Int (void);
void TF_Usart1_Tx_Rx_Int (void);
void TF_Usart2_Tx_Int (void);
void TF_Usart2_Tx_Rx_Int (void);
void TF_Usart3_Tx_Int (void);
void TF_Usart3_Tx_Rx_Int (void);
void TF_Spi1_Send_Single (void);
void TF_Spi1_Send_Receive_Usart2 (void);
void TF_Spi_Chip_Select (void);
void TF_Spi1_Send_Single_Chip_Select (void);
void TF_PB0 (void);
void TF_Sx_Init (void);
void TF_Sx_Fsk_Transmit (void);
void SxFskSetFreq (unsigned int freq);
unsigned int SxFskGetFreq (void);
void TF_Sx_Fsk_Transmit_Zero_Dev (void);
void TF_Sx_Fsk_Transmit_Modulation (void);
void TF_Sx_Fsk_Transmit_Modulation_Preamble (void);
void TF_Sx_Fsk_Transmit_Modulation2 (void);
void TF_Sx_Fsk_Transmit_Modulation_Sliced (void);
void TF_Sx_Fsk_Transmit_Packet_for_Slicer (void);
void TF_Sx_Fsk_Transmit_Packet (void);

void TF_Sx_Fsk_Receive_Rssi (void);
void TF_Sx_Fsk_Receive_Preamble (void);
void TF_Sx_Fsk_Receive_Data_Sliced (void);
void TF_Sx_Fsk_Receive_Data_Sliced_OnlyData (void);
void TF_Sx_Fsk_Receive_Packet_Data (void);

void TF_Timeouts (void);

// Lora tests
void TF_Sx_Lora_Transmit_Packet (void);

#endif    /* _TEST_FUNTIONS_H_ */

//--- end of file ---//

