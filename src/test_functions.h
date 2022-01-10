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


#endif    /* _TEST_FUNTIONS_H_ */

//--- end of file ---//

