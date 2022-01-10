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


//--- end of file ---//
