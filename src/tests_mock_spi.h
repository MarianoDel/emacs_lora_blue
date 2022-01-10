//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MOCK_SPI.H ######################
//---------------------------------------------

// Module Exported Functions ---------------------------------------------------
void SPI1_Cs_On (void);
void SPI1_Cs_Off (void);
void SPI1_Config (void);
unsigned char SPI1_Send_Receive (unsigned char data);
void SPI1_Wait_Clk256 (void);
void SPI1_GetBuffer (unsigned char * buff, unsigned char * len);


//--- end of file ---//


