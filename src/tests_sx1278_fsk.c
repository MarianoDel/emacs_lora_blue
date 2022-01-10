//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "sx1278_fsk.h"

//helper modules
#include "tests_ok.h"
#include "tests_utils.h"
// #include "tests_mock_spi.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
unsigned char sx_buff [256] = { 0 };


// Module Auxialiary Functions -------------------------------------------------
// void Wait_ms (unsigned short);
// void Sx_Rst_On (void);
// void Sx_Rst_Off (void);
// void Sx_Cs_On (void);
// void Sx_Cs_Off (void);



// Module Functions for testing ------------------------------------------------
void Test_SxFsk_Init (void);
void Test_SxFsk_Set_Freq (void);
void Test_SxFsk_Get_Freq (void);


// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    // Test_SxFsk_Init ();
    // Test_SxFsk_Set_Freq ();
    Test_SxFsk_Get_Freq ();
}


void Test_SxFsk_Init (void)
{
    printf("testing sx1278 init: ");
    unsigned char init = SxFskInit ();
    unsigned char rf_state = SxFsk_GetState();
    if ((init) && (rf_state == RF_IDLE))
        PrintOK();
    else
        PrintERR();
}


void Test_SxFsk_Set_Freq (void)
{
    // printf("testing sx1278 init: ");
    // unsigned char init = SxFskInit ();
    // unsigned char rf_state = SxFsk_GetState();
    // if ((init) && (rf_state == RF_IDLE))
    //     PrintOK();
    // else
    //     PrintERR();
    SxFskSetFreq(433000000);
}


void Test_SxFsk_Get_Freq (void)
{
    SxFskSetFreq(433000000);
    unsigned int freq = SxFskGetFreq();
    printf("freq getted: %d\n", freq);
}


// Local Mocked Functions ------------------------------------------------------
void SxBaseWrite (unsigned char addr, unsigned char data)
{
    sx_buff[addr] = data;
}


void SxBaseBurstWrite (unsigned char addr, unsigned char * pdata, unsigned char len)
{
    if ((addr + len) < 256)
    {
        memcpy(&sx_buff[addr], pdata, len);
    }
    else
        printf("read bigger than buffer!\n");
    
}


unsigned char SxBaseRead (unsigned char addr)
{
    if (addr & 0x80)
    {
        printf("read with write flag!\n");
        return 0;
    }
    else
        return sx_buff[addr];
}


void SxBaseBurstRead (unsigned char addr, unsigned char * pdata, unsigned char len)
{
    if (addr & 0x80)
    {
        printf("read with write flag!\n");
    }
    else if ((addr + len) < 256)
    {
        memcpy(pdata, &sx_buff[addr], len);
    }
    else
        printf("read bigger than buffer!\n");
    
}

void SxBaseRst (void)
{
}

unsigned char SxBaseCheckDeviceID (void)
{
    return 1;
}


// void Wait_ms (unsigned short delay)
// {
//     printf("waiting: %d ms\n", delay);
// }


// void Sx_Rst_On (void)
// {
//     printf("module Reset On\n");
// }


// void Sx_Rst_Off (void)
// {
//     printf("module Reset Off\n");
// }


// void Sx_Cs_On (void)
// {
//     SPI1_Cs_On();
// }


// void Sx_Cs_Off (void)
// {
//     SPI1_Cs_Off();
// }


//--- end of file ---//

