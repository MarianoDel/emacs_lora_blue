//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "tests_mock_spi.h"

//helper modules
#include "tests_ok.h"
#include "tests_utils.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------


// Module Functions for testing ------------------------------------------------
void Test_Spi_Base_Funcs (void);
    

// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    Test_Spi_Base_Funcs ();
    
}


void Test_Spi_Base_Funcs (void)
{
    printf("start spi\n");
    SPI1_Config();
    printf("testing wait\n");
    SPI1_Wait_Clk256 ();

    printf("testing buffer\n");
    SPI1_Cs_On ();
    SPI1_Send_Receive (0x01);
    SPI1_Send_Receive (0x02);
    SPI1_Send_Receive (0x03);
    SPI1_Cs_Off ();

    printf("testing other buffer\n");
    SPI1_Cs_On ();
    SPI1_Send_Receive (0x04);
    SPI1_Send_Receive (0x05);
    SPI1_Send_Receive (0x06);
    SPI1_Cs_Off ();
}





//--- end of file ---//

