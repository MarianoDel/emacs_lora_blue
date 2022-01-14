//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------
#include "data_slicer.h"

//helper modules
#include "tests_ok.h"

#include <stdio.h>
#include <string.h>


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Auxialiary Functions -------------------------------------------------



// Module Functions for testing ------------------------------------------------
void Test_Data_Slicer_Tx (void);
void Test_Data_Slicer_Rx (void);


// Module Functions ------------------------------------------------------------


int main(int argc, char *argv[])
{

    // Test_Data_Slicer_Tx ();
    Test_Data_Slicer_Rx ();
}


void Test_Data_Slicer_Tx (void)
{
    slicer_answer_e answer = SLICER_WORKING;
    // char buff_data [] = {"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789\n"};    //10 data 11 string
    char buff_data [] = {"THE"};    //3 data 4 string    
    unsigned char buff_sended [100] = { 0 };
    unsigned char bit = 0;
    unsigned char data_tx = 0;
    unsigned char cnt = 0;
    int total_bytes = 0;
    
    printf("testing slicer tx\n");
    DataSlicerTxInit();

    for (int i = 0; i < 1000; i++)
    {
        answer = DataSlicerTx (&bit, buff_data, sizeof(buff_data) - 1);

        if (answer == SLICER_WORKING)
        {
            data_tx |= bit;
            cnt++;
            
            if (cnt == 8)
            {
                printf("byte cnt: %d end clk: %d\n", cnt, i);
                cnt = 0;
                buff_sended[total_bytes] = data_tx;
                data_tx = 0;
                total_bytes++;                
            }
            else
                data_tx <<= 1;

        }

        if (answer == SLICER_END_PCKT)
        {
            // last bit to send
            data_tx |= bit;
            cnt++;
            printf("byte cnt: %d end clk: %d\n", cnt, i);
            buff_sended[total_bytes] = data_tx;
            data_tx = 0;
            total_bytes++;                
            
            printf("end of sending bytes cnt: %d total_clks: %d\n",
                   total_bytes,
                   i);

            break;
        }
    }

    printf("preamble: %d sync: %d data: %d data_end: %d total: %d\n",
           2,
           1,
           sizeof(buff_data) - 1,
           3,
           2 + 1 + sizeof(buff_data) - 1 + 3);
    
    printf("buffer sended hexa\n");
    for (int i = 0; i < total_bytes; i++)
        printf("0x%02x ", buff_sended[i]);

    printf("\n");

    printf("buffer sended ASCII\n");
    unsigned char ascii = 0;
    for (int i = 0; i < total_bytes; i++)
    {
        ascii = buff_sended[i];
        
        if ((ascii < 32) ||
            (ascii > 126))
            printf("0x%02x ", ascii);
        else
            printf("%c ", ascii);
    }

    printf("\n");
    
}


void Test_Data_Slicer_Rx (void)
{
    slicer_answer_e answer = SLICER_WORKING;
    // unsigned char buff_data [9] = {0x55, 0x55, 0x01, 'T', 'H', 'E', 'E', 'N', 'D'};    //9 data
    // unsigned char buff_data [10] = {0x55, 0x55, 0x01, 'T', 'H', 'E', 'O', 'E', 'N', 'D'};    //10 data
    // unsigned char buff_data [10] = {0x55, 0x55, 0x01, 'T', 'H', 'H', 'O', 'E', 'N', 'D'};    //10 data
    // unsigned char buff_data [9] = {0x55, 0x55, 0x01, 'E', 'N', 'N', 'E', 'N', 'D'};    //9 data
    unsigned char buff_data [9] = {0x55, 0x55, 0x01, 'E', 'N', 'E', 'E', 'N', 'D'};    //9 data        
    unsigned char buff_sended [100] = { 0 };
    unsigned char buff_rx [3] = { 0 };    
    unsigned char bit = 0;
    unsigned char data_tx = 0;
    unsigned char cnt = 0;
    int total_bytes = 0;
    int total_rx = 0;    
    
    printf("testing slicer rx\n");
    DataSlicerRxInit();

    // for (int i = 0; i < 1000; i++)
    for (int i = 0; i < 100; i++)        
    {
        bit = (buff_data[total_bytes] >> (7 - cnt)) & 0x01;
        cnt++;
        
        answer = DataSlicerRx (bit, buff_rx);

        if (cnt == 8)
        {
            total_bytes++;
            cnt = 0;
        }

        if (total_bytes == sizeof(buff_data))
        {
            printf("end of sending bytes cnt: %d total_clks: %d\n",
                   total_bytes,
                   i);
        }

        if (answer == SLICER_BYTE)
        {
            buff_sended[total_rx] = buff_rx[0];
            total_rx++;
            printf("end of byte: 0x%02x total rx: %d cnt: %d clk: %d\n",
                   buff_rx[0],
                   total_rx,
                   cnt,
                   i);
            
        }

        if (answer == SLICER_DOUBLE_BYTE)
        {
            buff_sended[total_rx] = buff_rx[0];
            total_rx++;
            buff_sended[total_rx] = buff_rx[1];
            total_rx++;
            printf("end of double byte: 0x%02x 0x%02x total rx: %d cnt: %d clk: %d\n",
                   buff_rx[0],
                   buff_rx[1],                   
                   total_rx,
                   cnt,
                   i);
            
        }

        if (answer == SLICER_TRIPLE_BYTE)
        {
            buff_sended[total_rx] = buff_rx[0];
            total_rx++;
            buff_sended[total_rx] = buff_rx[1];
            total_rx++;
            buff_sended[total_rx] = buff_rx[2];
            total_rx++;
            printf("end of triple byte: 0x%02x total rx: %d cnt: %d clk: %d\n",
                   buff_rx[0],
                   total_rx,
                   cnt,
                   i);
            
        }

        if (answer == SLICER_END_PCKT)
        {
            printf("rx bytes: %d byte cnt: %d end clk: %d\n", total_rx, cnt, i);
            break;
        }
    }

    printf("preamble: %d sync: %d data: %d data_end: %d total: %d\n",
           2,
           1,
           sizeof(buff_data) - 1,
           3,
           2 + 1 + sizeof(buff_data) - 1 + 3);
    
    printf("buffer sended hexa\n");
    for (int i = 0; i < total_rx; i++)
        printf("0x%02x ", buff_sended[i]);

    printf("\n");

    printf("buffer sended ASCII\n");
    unsigned char ascii = 0;
    for (int i = 0; i < total_rx; i++)
    {
        ascii = buff_sended[i];
        
        if ((ascii < 32) ||
            (ascii > 126))
            printf("0x%02x ", ascii);
        else
            printf("%c ", ascii);
    }

    printf("\n");
    
}




// Local Mocked Functions ------------------------------------------------------


//--- end of file ---//

