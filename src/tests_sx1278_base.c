//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "sx1278_base.h"

//helper modules
#include "tests_ok.h"
#include "tests_utils.h"
#include "tests_mock_spi.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------
void Wait_ms (unsigned short);
void Sx_Rst_On (void);
void Sx_Rst_Off (void);
void Sx_Cs_On (void);
void Sx_Cs_Off (void);



// Module Functions for testing ------------------------------------------------
void Test_SxBase_Reset (void);
void Test_SxBase_Device_ID (void);
void Test_SxBase_Write (void);
void Test_SxBase_Read (void);
void Test_SxBase_Burst_Write (void);
void Test_SxBase_Burst_Read (void);


// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    Test_SxBase_Reset ();
    Test_SxBase_Device_ID ();
    Test_SxBase_Write ();
    Test_SxBase_Read ();
    Test_SxBase_Burst_Write ();
    Test_SxBase_Burst_Read ();    
}


void Test_SxBase_Reset (void)
{
    printf("testing reset sx1278\n");
    SxBaseRst ();
}


void Test_SxBase_Device_ID (void)
{
    SPI1_Config ();
    unsigned char a = SxBaseCheckDeviceID ();
    printf("testing device id sx1278: ");
    if (a)
        PrintOK();
    else
        PrintERR();

}


void Test_SxBase_Write (void)
{
    unsigned char len = 0;
    unsigned char local_buff [10] = { 0 };
    
    SPI1_Config ();
    SxBaseWrite (0x22, 0x22);
    SPI1_GetBuffer (local_buff, &len);
    printf("testing write buffer: ");
    if ((local_buff[0] == (0x22 | 0x80)) &&
        (local_buff[1] == 0x22))
        PrintOK();
    else
        PrintERR();

}


void Test_SxBase_Burst_Write (void)
{
    unsigned char len = 0;
    unsigned char get_buff [10] = { 0 };
    unsigned char send_buff [10] = { 0 };    
    
    SPI1_Config ();
    
    send_buff[0] = 0x55;
    send_buff[1] = 0x55;
    send_buff[2] = 0x55;
    SxBaseBurstWrite(0x42, send_buff, 3);

    SPI1_GetBuffer(get_buff, &len);
    int equal = 1;
    for (int i = 0; i < 3; i++)
    {
        if (send_buff[i] != get_buff[i+1])
        {
            equal = 0;
            break;
        }
    }
    printf("testing burst writes total len %d: ", len);
    if ((len == 4) &&
        (equal))
        PrintOK();
    else
        PrintERR();
}


void Test_SxBase_Burst_Read (void)
{
    unsigned char len = 0;
    unsigned char get_buff [10] = { 0 };
    
    SPI1_Config ();
    
    SxBaseBurstRead(0x42, get_buff, 3);

    printf("buffer getted: ");
    for (int i = 0; i < 3; i++)
        printf("0x%02x ", get_buff[i]);

    printf("\ntesting burst read: ");
    if ((get_buff[0] == 0x12) &&
        (get_buff[1] == 0) &&
        (get_buff[2] == 0))
        PrintOK();
    else
        PrintERR();
}


void Test_SxBase_Read (void)
{
    SPI1_Config ();
    unsigned char answer = SxBaseRead (0x6d);

    printf("testing read: ");
    if (answer == 0x60)
        PrintOK();
    else
        PrintERR();

}


// Local Mocked Functions ------------------------------------------------------
void Wait_ms (unsigned short delay)
{
    printf("waiting: %d ms\n", delay);
}


void Sx_Rst_On (void)
{
    printf("module Reset On\n");
}


void Sx_Rst_Off (void)
{
    printf("module Reset Off\n");
}


void Sx_Cs_On (void)
{
    SPI1_Cs_On();
}


void Sx_Cs_Off (void)
{
    SPI1_Cs_Off();
}


unsigned char spi_cb_cnt = 0;
unsigned char spi_addr = 0;
void SPI1_Callback (unsigned char * addr)
{
    // printf("callback addr: %d", *addr);
    if (!(spi_cb_cnt %2))
    {
        spi_addr = *addr;
    }
    else
    {
        if (spi_addr == 0x42)
            *addr = 0x12;

        if (spi_addr == 0x6d)
            *addr = 0x60;
    }

    // printf(" data: %d\n", *addr);
    spi_cb_cnt++;
}
//--- end of file ---//

