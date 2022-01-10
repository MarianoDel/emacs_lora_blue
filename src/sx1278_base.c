//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_BASE.C ############################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "sx1278_base.h"
#include "sx1278_base_io.h"
#include "tim.h"
#include "spi.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void SxBaseRst (void)
{
    Sx_Rst_On ();
    Wait_ms(1);
    Sx_Rst_Off ();    
    Wait_ms(10);
}


unsigned char SxBaseCheckDeviceID (void)
{
    unsigned char getted = SxBaseRead(0x42);

    if (getted == 0x12)
        return 1;

    return 0;
}


void SxBaseWrite (unsigned char addr, unsigned char data)
{
    Sx_Cs_On ();
    SPI1_Send_Receive(addr | 0x80);
    SPI1_Send_Receive(data);
    SPI1_Wait_Clk256();
    Sx_Cs_Off ();
}


void SxBaseBurstWrite (unsigned char addr, unsigned char * pdata, unsigned char len)
{
    Sx_Cs_On ();
    SPI1_Send_Receive(addr | 0x80);
    for (unsigned char i = 0; i < len; i++)
    {
        SPI1_Send_Receive(*pdata);
        pdata++;
    }
    SPI1_Wait_Clk256();
    Sx_Cs_Off ();
}


unsigned char SxBaseRead (unsigned char addr)
{
    unsigned char getted;
    
    Sx_Cs_On ();
    SPI1_Send_Receive(addr);
    getted = SPI1_Send_Receive(0x00);
    SPI1_Wait_Clk256();
    Sx_Cs_Off ();

    return getted;
}


void SxBaseBurstRead (unsigned char addr, unsigned char * pdata, unsigned char len)
{
    Sx_Cs_On ();
    SPI1_Send_Receive(addr);
    for (unsigned int i = 0; i < len; i++)
    {
        *pdata = SPI1_Send_Receive(0x00);
        pdata++;
    }
    SPI1_Wait_Clk256();
    Sx_Cs_Off ();
}


//--- end of file ---//
