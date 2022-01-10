//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS_MOCK_SPI.C ######################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "tests_mock_spi.h"
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
// Mock for SPI1
unsigned char spi_buff [256] = { 0 };
unsigned char * p_spi_buff_tx;
char buff [256] = { 0 };

void SPI1_Callback (unsigned char * addr) __attribute__ ((weak));

// Module Functions ------------------------------------------------------------
void SPI1_Cs_On (void)
{
    PrintYellow("CS_ON buffer start\n");
    p_spi_buff_tx = spi_buff;
}


void SPI1_Cs_Off (void)
{
    char buff_in [20] = { 0 };
    PrintYellow("CS_OFF\n");
    int len = p_spi_buff_tx - spi_buff;
    sprintf(buff, "buff len: %d ", len);
    for (int i = 0; i < len; i++)
    {
        sprintf(buff_in, "0x%02x ", spi_buff[i]);
        strcat(buff, buff_in);
    }
    strcat(buff, "\n");
    PrintYellow(buff);
}


void SPI1_Config (void)
{
    printf("init spi\n");
    p_spi_buff_tx = spi_buff;
}


unsigned char SPI1_Send_Receive (unsigned char data)
{
    unsigned char answer = 0;
    SPI1_Callback(&data);
    answer = data;
    *p_spi_buff_tx = data;
    p_spi_buff_tx++;
    
    return answer;
}


void SPI1_Wait_Clk256 (void)
{
    
}


void SPI1_GetBuffer (unsigned char * buff, unsigned char * len)
{
    *len = p_spi_buff_tx - spi_buff;
    memcpy(buff, spi_buff, *len);
}


// reimplement this callback on your code
void SPI1_Callback (unsigned char * addr)
{
    
}



//--- end of file ---//


