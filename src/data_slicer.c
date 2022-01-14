//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DATA_SLICER.C ############################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "data_slicer.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants and Macros -----------------------------------
typedef enum {
    SL_STATUS_INIT,
    SL_STATUS_SYNC,
    SL_STATUS_DATA,
    SL_STATUS_DATA_END,
    SL_STATUS_END

} slicer_status_e;

#define SYNC_BYTE    0x01
// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
slicer_status_e slicer_rx_status = SL_STATUS_INIT;
slicer_status_e slicer_tx_status = SL_STATUS_INIT;
unsigned char sl_byte = 0;
unsigned char sl_cnt = 0;
unsigned char sl_end_cnt = 0;

unsigned char sended = 0;
unsigned char preamble_cnt = 0;
unsigned char data_end [3] = {'E', 'N', 'D'};


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void DataSlicerRxInit (void)
{
    slicer_rx_status = SL_STATUS_INIT;
    sl_byte = 0;
}


slicer_answer_e DataSlicerRx (unsigned char bit, unsigned char * data)
{
    slicer_answer_e answer = SLICER_WORKING;
    
    switch (slicer_rx_status)
    {
    case SL_STATUS_INIT:
        sl_byte |= bit;
        sl_cnt++;

        if ((sl_byte == 0x01) &&
            (sl_cnt >= 8))    //eight bits at least
        {
            slicer_rx_status = SL_STATUS_SYNC;
            sl_byte = 0;
            sl_cnt = 0;
            sl_end_cnt = 0;
        }
        else
            sl_byte <<= 1;
        
        break;

    case SL_STATUS_SYNC:
        sl_byte |= bit;
        sl_cnt++;

        if (sl_cnt == 8)
        {
            // check for end of tx
            if ((sl_end_cnt == 0) && (sl_byte == 'E'))
            {
                sl_end_cnt++;
            }
            else if (sl_end_cnt == 1)
            {
                if (sl_byte == 'N')
                    sl_end_cnt++;
                else if (sl_byte == 'E')
                {
                    // last E was a valid byte
                    *(data + 0) = 'E';
                    answer = SLICER_BYTE;
                }
                else
                {
                    // not an end send two bytes
                    *(data + 0) = 'E';
                    *(data + 1) = sl_byte;
                    sl_end_cnt = 0;
                    answer = SLICER_DOUBLE_BYTE;
                }
            }
            else if (sl_end_cnt == 2)
            {
                if (sl_byte == 'D')
                {
                    answer = SLICER_END_PCKT;
                    slicer_rx_status = SL_STATUS_END;
                }
                else
                {
                    // not an end send three bytes
                    *(data + 0) = 'E';
                    *(data + 1) = 'N';                    
                    *(data + 2) = sl_byte;
                    sl_end_cnt = 0;                    
                    answer = SLICER_TRIPLE_BYTE;
                }
            }
            else
            {
                *data = sl_byte;
                answer = SLICER_BYTE;
            }

            sl_byte = 0;
            sl_cnt = 0;

        }
        else
            sl_byte <<= 1;
        
        break;

    case SL_STATUS_DATA:
    case SL_STATUS_DATA_END:        
    case SL_STATUS_END:
        break;
    }

    return answer;
}


void DataSlicerTxInit (void)
{
    slicer_tx_status = SL_STATUS_INIT;
    preamble_cnt = 0;
    sl_cnt = 0;
}


slicer_answer_e DataSlicerTx (unsigned char * bit, unsigned char * data, unsigned char len)
{
    slicer_answer_e answer = SLICER_WORKING;
    
    switch (slicer_tx_status)
    {
    case SL_STATUS_INIT:
        // send preamble
        *bit = preamble_cnt % 2;
        preamble_cnt++;
        
        if (preamble_cnt == 16)
        {
            sl_cnt = 0;
            slicer_tx_status = SL_STATUS_SYNC;            
        }
        break;

    case SL_STATUS_SYNC:
        *bit = (SYNC_BYTE >> (7 - sl_cnt)) & 0x01;
        sl_cnt++;

        if (sl_cnt == 8)
        {
            sended = 0;
            sl_cnt = 0;
            slicer_tx_status = SL_STATUS_DATA;
        }
        break;

    case SL_STATUS_DATA:
        *bit = (*(data + sended) >> (7 - sl_cnt)) & 0x01;
        sl_cnt++;

        if (sl_cnt == 8)
        {
            sl_cnt = 0;
            sended++;
            if (sended == len)
            {
                // end of data
                sended = 0;
                slicer_tx_status = SL_STATUS_DATA_END;
            }
        }
        break;

    case SL_STATUS_DATA_END:
        *bit = (*(data_end + sended) >> (7 - sl_cnt)) & 0x01;
        sl_cnt++;

        if (sl_cnt == 8)
        {
            sl_cnt = 0;
            sended++;
            if (sended == 3)
            {
                // end of data and data_end
                slicer_tx_status = SL_STATUS_END;
                answer = SLICER_END_PCKT;
            }
            
        }
        break;

    case SL_STATUS_END:
        break;
        
    }

    return answer;
}

//--- end of file ---//
