//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_BASE_IO.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "sx1278_base_io.h"
#include "hard.h"
#include "stm32f10x.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void Sx_Rst_On (void)
{
    PB0_OFF;
}


void Sx_Rst_Off (void)
{
    PB0_ON;
}


void Sx_Cs_On (void)
{
    SPI_CS_ON;
}


void Sx_Cs_Off (void)
{
    SPI_CS_OFF;
}


unsigned char Sx_Dio0_Get (void)
{
    return PB1_IN;
}


void Sx_Dio1_Output (void)
{
    unsigned int temp;
    
    temp = GPIOB->CRH;    //PB10 output
    temp &= 0xFFFFF0FF;
    temp |= 0x00000100;
    GPIOB->CRH = temp;    
}


void Sx_Dio1_Input (void)
{
    unsigned int temp;
    
    temp = GPIOB->CRH;    //PB10 input
    temp &= 0xFFFFF0FF;
    temp |= 0x00000400;
    GPIOB->CRH = temp;    
}


unsigned char Sx_Dio1_Get (void)
{
    return PB10_IN;
}


void Sx_Dio2_Output (void)
{
    unsigned int temp;
    
    temp = GPIOB->CRH;    //PB11 output
    temp &= 0xFFFF0FFF;
    temp |= 0x00001000;
    GPIOB->CRH = temp;    
}


void Sx_Dio2_Input (void)
{
    unsigned int temp;
    
    temp = GPIOB->CRH;    //PB11 input
    temp &= 0xFFFF0FFF;
    temp |= 0x00004000;
    GPIOB->CRH = temp;    
}


unsigned char Sx_Dio2_Get (void)
{
    return PB11_IN;
}


void Sx_Dio2_Set (void)
{
    PB11_ON;
}


void Sx_Dio2_Reset (void)
{
    PB11_OFF;
}


//--- end of file ---//
