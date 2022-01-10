//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_FSK.C #############################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "sx1278_base.h"
#include "sx1278_fsk.h"
#include "tim.h"
#include "spi.h"
#include "hard.h"

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------

// Default settings
// tFskSettings FskSettings = 
// {
//     870000000,      // RFFrequency
//     9600,           // Bitrate
//     50000,          // Fdev
//     20,             // Power
//     100000,         // RxBw
//     150000,         // RxBwAfc
//     true,           // CrcOn
//     true,           // AfcOn    
//     255             // PayloadLength (set payload size to the maximum for variable mode, else set the exact payload length)
// };

// /*!
//  * SX1278 FSK registers variable
//  */
// tSX1278* SX1278;

// /*!
//  * Local RF buffer for communication support
//  */
// static uint8_t RFBuffer[RF_BUFFER_SIZE];

// /*!
//  * Chunk size of data write in buffer 
//  */
// static uint8_t DataChunkSize = 32;


// /*!
//  * RF state machine variable
//  */
// static uint8_t RFState = RF_STATE_IDLE;

// /*!
//  * Rx management support variables
//  */

// /*!
//  * PacketTimeout holds the RF packet timeout
//  * SyncSize = [0..8]
//  * VariableSize = [0;1]
//  * AddressSize = [0;1]
//  * PayloadSize = [0..RF_BUFFER_SIZE]
//  * CrcSize = [0;2]
//  * PacketTimeout = ( ( 8 * ( VariableSize + AddressSize + PayloadSize + CrcSize ) / BR ) * 1000.0 ) + 1
//  * Computed timeout is in miliseconds
//  */
// static uint32_t PacketTimeout;

// /*!
//  * Preamble2SyncTimeout
//  * Preamble2SyncTimeout = ( ( 8 * ( PremableSize + SyncSize ) / RFBitrate ) * 1000.0 ) + 1
//  * Computed timeout is in miliseconds
//  */
// static uint32_t Preamble2SyncTimeout;

// static bool PreambleDetected = false;
// static bool SyncWordDetected = false;
// static bool PacketDetected = false;
// static uint16_t RxPacketSize = 0;
// static uint8_t RxBytesRead = 0;
// static uint8_t TxBytesSent = 0;
// static double RxPacketRssiValue;
// static uint32_t RxPacketAfcValue;
// static uint8_t RxGain = 1;
// static uint32_t RxTimeoutTimer = 0;
// static uint32_t Preamble2SyncTimer = 0;

// /*!
//  * Tx management support variables
//  */
// static uint16_t TxPacketSize = 0;
// static uint32_t TxTimeoutTimer = 0;
RfState_e RFState = RF_NO_MODULE;

unsigned char SxFsk_GetState (void)
{
    return RFState;
}

unsigned char SxFskInit (void)
{
    RFState = RF_NO_MODULE;
    SxBaseRst();
    if (!SxBaseCheckDeviceID())
        return 0;

    // Set the device in FSK mode and Sleep Mode
    // SxBaseWrite(REG_OPMODE, RF_OPMODE_MODULATIONTYPE_FSK | RF_OPMODE_SLEEP);    // defaults are good

    // set bitrate
    // SxBaseWrite(REG_BITRATEMSB, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good
    // SxBaseWrite(REG_BITRATELSB, RF_PARAMP_MODULATIONSHAPING_01);

    // set freq deviation
    // SxBaseWrite(REG_FDEVMSB, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good
    // SxBaseWrite(REG_FDEVLSB, RF_PARAMP_MODULATIONSHAPING_01);

    // set freq
    // SxFskSetFreq(433000000);    // defaults are good
    // SxBaseWrite(REG_FRFMSB, RF_PARAMP_MODULATIONSHAPING_01);
    // SxBaseWrite(REG_FRFMID, RF_PARAMP_MODULATIONSHAPING_01);
    // SxBaseWrite(REG_FRFLSB, RF_PARAMP_MODULATIONSHAPING_01);

    // transmitter settings
    // set power output
    // SxBaseWrite(REG_PACONFIG, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good
    SxBaseWrite(REG_PACONFIG, RF_PACONFIG_PASELECT_PABOOST | 0x4f);    // defaults are good    

    // set ramp and modulation shaping Gaussian BT = 1
    // SxBaseWrite(REG_PARAMP, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good

    // set overload current
    // SxBaseWrite(REG_OCP, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good

    // receiver settings    
    // set lna
    SxBaseWrite(REG_LNA, RF_LNA_GAIN_G1);

    // rx config
    SxBaseWrite(REG_RXCONFIG,
                RF_RXCONFIG_RESTARTRXONCOLLISION_OFF |
                RF_RXCONFIG_AFCAUTO_ON |
                RF_RXCONFIG_AGCAUTO_ON |
                RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT);

    // rssi config
    // SxBaseWrite(REG_RSSICONFIG, 0xFF);    // defaults are good

    // rssi collision
    // SxBaseWrite(REG_RSSICOLLISION, 0xFF);    // defaults are good

    // rssi threshold
    // SxBaseWrite(REG_RSSITHRESH, 0xFF);    // defaults are good

    // rx bandwith
    // SxBaseWrite(REG_RXBW, 0xFF);    // defaults are good

    // rx afc bw
    // SxBaseWrite(REG_AFCBW, 0xFF);    // defaults are good

    // afc and fei to defaults

    // preamble config
    // SxBaseWrite(REG_PREAMBLEDETECT, 0xFF);    // defaults are good

    // preamble size
    // SxBaseWrite(REG_PREAMBLEMSB, 0xFF);    // defaults are good
    // SxBaseWrite(REG_PREAMBLELSB, 0xFF);    // defaults are good

    // sync config
    // SxBaseWrite(REG_SYNCCONFIG, 0xFF);    // defaults are good

    // sync key
    // SxBaseWrite(REG_SYNCVALUE1, 0xFF);    // defaults are good
    // SxBaseWrite(REG_SYNCVALUE2, 0xFF);    // defaults are good
    // SxBaseWrite(REG_SYNCVALUE3, 0xFF);    // defaults are good
    // SxBaseWrite(REG_SYNCVALUE4, 0xFF);    // defaults are good    
    // SX1278->RegSyncValue1 = 0x69;
    // SX1278->RegSyncValue2 = 0x81;
    // SX1278->RegSyncValue3 = 0x7E;
    // SX1278->RegSyncValue4 = 0x96;

    // SxBaseWrite(REG_PACKETCONFIG1,
    //             RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE |
    //             RF_PACKETCONFIG1_DCFREE_OFF |
    //             RF_PACKETCONFIG1_CRC_ON |
    //             RF_PACKETCONFIG1_CRCAUTOCLEAR_ON |
    //             RF_PACKETCONFIG1_ADDRSFILTERING_OFF |
    //             RF_PACKETCONFIG1_CRCWHITENINGTYPE_CCITT);    // defaults are good

    SxBaseWrite(REG_PACKETCONFIG2, RF_PACKETCONFIG2_DATAMODE_CONTINUOUS);

    // SX1278FskSetOpMode( RF_OPMODE_STANDBY );

    // Calibrate the HF
    // SX1278FskRxCalibrate( );
    RFState = RF_IDLE;
    return 1;

}


void SxFskSetFreq (unsigned int freq)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    
    freg = ( unsigned int )( ( float )freq / ( float )FREQ_STEP );
    buff[0] = (unsigned char) (( freg >> 16 ) & 0xFF);
    buff[1] = (unsigned char) (( freg >> 8 ) & 0xFF);
    buff[2] = (unsigned char) (( freg ) & 0xFF);
    SxBaseBurstWrite(REG_FRFMSB, buff, 3);
}


unsigned int SxFskGetFreq (void)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    SxBaseBurstRead(REG_FRFMSB, buff, 3);

    freg = buff[0] << 16 ;
    freg += buff[1] << 8 ;
    freg += buff[2];
    float freq = (float)freg * (float)FREQ_STEP;
    return (unsigned int)(freq);
}


void SxFskSetOpMode (unsigned char new_mode)
{
    unsigned char last_mode = 0;
    
    last_mode = SxBaseRead(REG_OPMODE);
    last_mode &= RF_OPMODE_MASK;

    new_mode &= (~(RF_OPMODE_MASK));
    new_mode |= last_mode;
    SxBaseWrite(REG_OPMODE, new_mode);
}


unsigned char SxFskGetOpMode (void)
{
    unsigned char last_mode = 0;
    
    last_mode = SxBaseRead(REG_OPMODE);
    last_mode &= (~(RF_OPMODE_MASK));

    return last_mode;
}


// void SxFsk_Process (void)
// {
//     switch (RFState)
//     {
//     case RF_NO_MODULE:
//         break;

//     case RF_IDLE:
//         break;

//         case RF_TX
//     }
// }

// uint32_t SX1278FskProcess( void )
// {
//     uint32_t result = RF_BUSY;

//     switch( RFState )
//     {
//     case RF_STATE_IDLE:
//         break;
//     // Rx management
//     case RF_STATE_RX_INIT:
//         // DIO mapping setup
//         if( ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_CRC_ON ) == RF_PACKETCONFIG1_CRC_ON )
//         {
//             //                           CrcOk,                   FifoLevel,               SyncAddr,               FifoEmpty
//             SX1278->RegDioMapping1 = RF_DIOMAPPING1_DIO0_01 | RF_DIOMAPPING1_DIO1_00 | RF_DIOMAPPING1_DIO2_11 | RF_DIOMAPPING1_DIO3_00;
//         }
//         else
//         {
//             //                           PayloadReady,            FifoLevel,               SyncAddr,               FifoEmpty
//             SX1278->RegDioMapping1 = RF_DIOMAPPING1_DIO0_00 | RF_DIOMAPPING1_DIO1_00 | RF_DIOMAPPING1_DIO2_11 | RF_DIOMAPPING1_DIO3_00;
//         }
//         //                          Preamble,                   Data
//         SX1278->RegDioMapping2 = RF_DIOMAPPING2_DIO4_11 | RF_DIOMAPPING2_DIO5_10 | RF_DIOMAPPING2_MAP_PREAMBLEDETECT;
//         SX1278WriteBuffer( REG_DIOMAPPING1, &SX1278->RegDioMapping1, 2 );

//         SX1278FskSetOpMode( RF_OPMODE_RECEIVER );
    
//         memset( RFBuffer, 0, ( size_t )RF_BUFFER_SIZE );

//         PacketTimeout = ( uint16_t )( round( ( 8.0 * ( ( double )SX1278FskGetPacketPayloadSize( ) ) / ( double )FskSettings.Bitrate ) * 1000.0 ) + 1.0 );
//         PacketTimeout = PacketTimeout + ( PacketTimeout >> 1 ); // Set the Packet timeout as 1.5 times the full payload transmission time

//         Preamble2SyncTimeout = PacketTimeout;

//         Preamble2SyncTimer = RxTimeoutTimer = GET_TICK_COUNT( );

//         SX1278->RegFifoThresh = RF_FIFOTHRESH_TXSTARTCONDITION_FIFONOTEMPTY | 0x20; // 32 bytes of data
//         SX1278Write( REG_FIFOTHRESH, SX1278->RegFifoThresh );

//         PreambleDetected = false;
//         SyncWordDetected = false;
//         PacketDetected = false;
//         RxBytesRead = 0;
//         RxPacketSize = 0;
//         RFState = RF_STATE_RX_SYNC;
//         break;
//     case RF_STATE_RX_SYNC:
//         if( ( DIO4 == 1 ) && ( PreambleDetected == false ) )// Preamble
//         {
//             PreambleDetected = true;
//             Preamble2SyncTimer = GET_TICK_COUNT( );
//         }
//         if( ( DIO2 == 1 ) && ( PreambleDetected == true ) && ( SyncWordDetected == false ) ) // SyncAddr
//         {
//             SyncWordDetected = true;
        
//             RxPacketRssiValue = SX1278FskReadRssi( );

//             RxPacketAfcValue = SX1278FskReadAfc( );
//             RxGain = SX1278FskReadRxGain( );
        
//             Preamble2SyncTimer = RxTimeoutTimer = GET_TICK_COUNT( );

//             RFState = RF_STATE_RX_RUNNING;
//         }

//         // Preamble 2 SyncAddr timeout
//         if( ( SyncWordDetected == false ) && ( PreambleDetected == true ) && ( ( GET_TICK_COUNT( ) - Preamble2SyncTimer ) > Preamble2SyncTimeout ) )
//         {
//             RFState = RF_STATE_RX_INIT;
//             SX1278Write( REG_RXCONFIG, SX1278->RegRxConfig | RF_RXCONFIG_RESTARTRXWITHPLLLOCK );
//         }
//         if( ( SyncWordDetected == false ) &&
//             ( PreambleDetected == false ) &&
//             ( PacketDetected == false ) &&
//             ( ( GET_TICK_COUNT( ) - RxTimeoutTimer ) > PacketTimeout ) )
//         {
//             RFState = RF_STATE_RX_TIMEOUT;
//         }
//         break;
//     case RF_STATE_RX_RUNNING:
//         if( RxPacketSize > RF_BUFFER_SIZE_MAX )
//         {
//             RFState = RF_STATE_RX_LEN_ERROR;
//             break;
//         }
// #if 1
//         if( DIO1 == 1 ) // FifoLevel
//         {
//             if( ( RxPacketSize == 0 ) && ( RxBytesRead == 0 ) ) // Read received packet size
//             {
//                 if( ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) == RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE )
//                 {
//                     SX1278ReadFifo( ( uint8_t* )&RxPacketSize, 1 );
//                 }
//                 else
//                 {
//                     RxPacketSize = SX1278->RegPayloadLength;
//                 }
//             }

//             if( ( RxPacketSize - RxBytesRead ) > ( SX1278->RegFifoThresh & 0x3F ) )
//             {
//                 SX1278ReadFifo( ( RFBuffer + RxBytesRead ), ( SX1278->RegFifoThresh & 0x3F ) );
//                 RxBytesRead += ( SX1278->RegFifoThresh & 0x3F );
//             }
//             else
//             {
//                 SX1278ReadFifo( ( RFBuffer + RxBytesRead ), RxPacketSize - RxBytesRead );
//                 RxBytesRead += ( RxPacketSize - RxBytesRead );
//             }
//         }
// #endif
//         if( DIO0 == 1 ) // PayloadReady/CrcOk
//         {
//             RxTimeoutTimer = GET_TICK_COUNT( );
//             if( ( RxPacketSize == 0 ) && ( RxBytesRead == 0 ) ) // Read received packet size
//             {
//                 if( ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) == RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE )
//                 {
//                     SX1278ReadFifo( ( uint8_t* )&RxPacketSize, 1 );
//                 }
//                 else
//                 {
//                     RxPacketSize = SX1278->RegPayloadLength;
//                 }
//                 SX1278ReadFifo( RFBuffer + RxBytesRead, RxPacketSize - RxBytesRead );
//                 RxBytesRead += ( RxPacketSize - RxBytesRead );
//                 PacketDetected = true;
//                 RFState = RF_STATE_RX_DONE;
//             }
//             else
//             {
//                 SX1278ReadFifo( RFBuffer + RxBytesRead, RxPacketSize - RxBytesRead );
//                 RxBytesRead += ( RxPacketSize - RxBytesRead );
//                 PacketDetected = true;
//                 RFState = RF_STATE_RX_DONE;
//             }
//         }
        
//         // Packet timeout
//         if( ( PacketDetected == false ) && ( ( GET_TICK_COUNT( ) - RxTimeoutTimer ) > PacketTimeout ) )
//         {
//             RFState = RF_STATE_RX_TIMEOUT;
//         }
//         break;
//     case RF_STATE_RX_DONE:
//         RxBytesRead = 0;
//         RFState = RF_STATE_RX_INIT;
//         result = RF_RX_DONE;
//         break;
//     case RF_STATE_RX_TIMEOUT:
//         RxBytesRead = 0;
//         RxPacketSize = 0;
//         SX1278Write( REG_RXCONFIG, SX1278->RegRxConfig | RF_RXCONFIG_RESTARTRXWITHPLLLOCK );
//         RFState = RF_STATE_RX_INIT;
//         result = RF_RX_TIMEOUT;
//         break;
//     case RF_STATE_RX_LEN_ERROR:
//         RxBytesRead = 0;
//         RxPacketSize = 0;
//         SX1278Write( REG_RXCONFIG, SX1278->RegRxConfig | RF_RXCONFIG_RESTARTRXWITHPLLLOCK );
//         RFState = RF_STATE_RX_INIT;
//         result = RF_LEN_ERROR;
//         break;
//     // Tx management
//     case RF_STATE_TX_INIT:
//         // Packet DIO mapping setup
//         //                           PacketSent,               FifoLevel,              FifoFull,               TxReady
//         SX1278->RegDioMapping1 = RF_DIOMAPPING1_DIO0_00 | RF_DIOMAPPING1_DIO1_00 | RF_DIOMAPPING1_DIO2_00 | RF_DIOMAPPING1_DIO3_01;
//         //                           LowBat,                   Data
//         SX1278->RegDioMapping2 = RF_DIOMAPPING2_DIO4_00 | RF_DIOMAPPING2_DIO5_10;
//         SX1278WriteBuffer( REG_DIOMAPPING1, &SX1278->RegDioMapping1, 2 );

//         SX1278->RegFifoThresh = RF_FIFOTHRESH_TXSTARTCONDITION_FIFONOTEMPTY | 0x18; // 24 bytes of data
//         SX1278Write( REG_FIFOTHRESH, SX1278->RegFifoThresh );

//         SX1278FskSetOpMode( RF_OPMODE_TRANSMITTER );
//         RFState = RF_STATE_TX_READY_WAIT;
//         TxBytesSent = 0;
//         break;
//     case RF_STATE_TX_READY_WAIT:
//         if( DIO3 == 1 )    // TxReady
//         {
//             if( ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE ) == RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE )
//             {
//                 SX1278WriteFifo( ( uint8_t* )&TxPacketSize, 1 );
//             }
            
//             if( ( TxPacketSize > 0 ) && ( TxPacketSize <= 64 ) )
//             {
//                 DataChunkSize = TxPacketSize;
//             }
//             else
//             {
//                 DataChunkSize = 32;
//             }
            
//             SX1278WriteFifo( RFBuffer, DataChunkSize );
//             TxBytesSent += DataChunkSize;
//             TxTimeoutTimer = GET_TICK_COUNT( );
//             RFState = RF_STATE_TX_RUNNING;
//         }
//         break;

//     case RF_STATE_TX_RUNNING:
//         if( DIO1 == 0 )    // FifoLevel below thresold
//         {  
//             if( ( TxPacketSize - TxBytesSent ) > DataChunkSize )
//             {
//                 SX1278WriteFifo( ( RFBuffer + TxBytesSent ), DataChunkSize );
//                 TxBytesSent += DataChunkSize;
//             }
//             else 
//             {
//                 // we write the last chunk of data
//                 SX1278WriteFifo( RFBuffer + TxBytesSent, TxPacketSize - TxBytesSent );
//                 TxBytesSent += TxPacketSize - TxBytesSent;
//             }
//         }

//         if( DIO0 == 1 ) // PacketSent
//         {
//             TxTimeoutTimer = GET_TICK_COUNT( );
//             RFState = RF_STATE_TX_DONE;
//             SX1278FskSetOpMode( RF_OPMODE_STANDBY );
//         }
         
//         // Packet timeout
//         if( ( GET_TICK_COUNT( ) - TxTimeoutTimer ) > TICK_RATE_MS( 1000 ) )
//         {
//             RFState = RF_STATE_TX_TIMEOUT;
//         }
//         break;
//     case RF_STATE_TX_DONE:
//         RFState = RF_STATE_IDLE;
//         result = RF_TX_DONE;
//         break;
//     case RF_STATE_TX_TIMEOUT:
//         RFState = RF_STATE_IDLE;
//         result = RF_TX_TIMEOUT;
//         break;
//     default:
//         break;
//     }
//     return result;
// }

// #endif // USE_SX1278_RADIO

// int32_t SX1278FskReadFei( void )
// {
//     SX1278ReadBuffer( REG_FEIMSB, &SX1278->RegFeiMsb, 2 );                          // Reads the FEI value

//     return ( int32_t )( double )( ( ( uint16_t )SX1278->RegFeiMsb << 8 ) | ( uint16_t )SX1278->RegFeiLsb ) * ( double )FREQ_STEP;
// }

// int32_t SX1278FskReadAfc( void )
// {
//     SX1278ReadBuffer( REG_AFCMSB, &SX1278->RegAfcMsb, 2 );                            // Reads the AFC value
//     return ( int32_t )( double )( ( ( uint16_t )SX1278->RegAfcMsb << 8 ) | ( uint16_t )SX1278->RegAfcLsb ) * ( double )FREQ_STEP;
// }

// uint8_t SX1278FskReadRxGain( void )
// {
//     SX1278Read( REG_LNA, &SX1278->RegLna );
//     return( SX1278->RegLna >> 5 ) & 0x07;
// }

// double SX1278FskReadRssi( void )
// {
//     SX1278Read( REG_RSSIVALUE, &SX1278->RegRssiValue );                               // Reads the RSSI value

//     return -( double )( ( double )SX1278->RegRssiValue / 2.0 );
// }

// uint8_t SX1278FskGetPacketRxGain( void )
// {
//     return RxGain;
// }

// double SX1278FskGetPacketRssi( void )
// {
//     return RxPacketRssiValue;
// }

// uint32_t SX1278FskGetPacketAfc( void )
// {
//     return RxPacketAfcValue;
// }

// void SX1278FskStartRx( void )
// {
//     SX1278FskSetRFState( RF_STATE_RX_INIT );
// }

// void SX1278FskGetRxPacket( void *buffer, uint16_t *size )
// {
//     *size = RxPacketSize;
//     RxPacketSize = 0;
//     memcpy( ( void * )buffer, ( void * )RFBuffer, ( size_t )*size );
// }

// void SX1278FskSetTxPacket( const void *buffer, uint16_t size )
// {
//     TxPacketSize = size;
//     memcpy( ( void * )RFBuffer, buffer, ( size_t )TxPacketSize ); 

//     RFState = RF_STATE_TX_INIT;
// }

// // Remark: SX1278 must be fully initialized before calling this function
// static uint16_t SX1278FskGetPacketPayloadSize( void )
// {
//     uint16_t syncSize;
//     uint16_t variableSize;
//     uint16_t addressSize;
//     uint16_t payloadSize;
//     uint16_t crcSize;

//     syncSize = ( SX1278->RegSyncConfig & 0x07 ) + 1;
//     variableSize = ( ( SX1278->RegPacketConfig1 & 0x80 ) == 0x80 ) ? 1 : 0;
//     addressSize = ( ( SX1278->RegPacketConfig1 & 0x06 ) != 0x00 ) ? 1 : 0;
//     payloadSize = SX1278->RegPayloadLength;
//     crcSize = ( ( SX1278->RegPacketConfig1 & 0x10 ) == 0x10 ) ? 2 : 0;
    
//     return syncSize + variableSize + addressSize + payloadSize + crcSize;
// }

// // Remark: SX1278 must be fully initialized before calling this function
// static uint16_t SX1278FskGetPacketHeaderSize( void )
// {
//     uint16_t preambleSize;
//     uint16_t syncSize;

//     preambleSize = ( ( uint16_t )SX1278->RegPreambleMsb << 8 ) | ( uint16_t )SX1278->RegPreambleLsb;
//     syncSize = ( SX1278->RegSyncConfig & 0x07 ) + 1;
    
//     return preambleSize + syncSize;
// }

// uint8_t SX1278FskGetRFState( void )
// {
//     return RFState;
// }

// void SX1278FskSetRFState( uint8_t state )
// {
//     RFState = state;
// }


// void SX1278FskRxCalibrate( void )
// {
//     // the function RadioRxCalibrate is called just after the reset so all register are at their default values
//     uint8_t regPaConfigInitVal;
//     uint32_t initialFreq;

//     // save register values;
//     SX1278Read( REG_PACONFIG, &regPaConfigInitVal );
//     initialFreq = SX1278FskGetRFFrequency( );

//     // Cut the PA just in case
//     SX1278->RegPaConfig = 0x00; // RFO output, power = -1 dBm
//     SX1278Write( REG_PACONFIG, SX1278->RegPaConfig );

//     // Set Frequency in HF band
//     SX1278FskSetRFFrequency( 860000000 );

//     // Rx chain re-calibration workaround
//     SX1278Read( REG_IMAGECAL, &SX1278->RegImageCal );    
//     SX1278->RegImageCal = ( SX1278->RegImageCal & RF_IMAGECAL_IMAGECAL_MASK ) | RF_IMAGECAL_IMAGECAL_START;
//     SX1278Write( REG_IMAGECAL, SX1278->RegImageCal );

//     SX1278Read( REG_IMAGECAL, &SX1278->RegImageCal );
//     // rx_cal_run goes low when calibration in finished
//     while( ( SX1278->RegImageCal & RF_IMAGECAL_IMAGECAL_RUNNING ) == RF_IMAGECAL_IMAGECAL_RUNNING )
//     {
//         SX1278Read( REG_IMAGECAL, &SX1278->RegImageCal );
//     }

//     // reload saved values into the registers
//     SX1278->RegPaConfig = regPaConfigInitVal;
//     SX1278Write( REG_PACONFIG, SX1278->RegPaConfig );

//     SX1278FskSetRFFrequency( initialFreq );

// }

// void SX1278FskSetBitrate( uint32_t bitrate )
// {
//     FskSettings.Bitrate = bitrate;
    
//     bitrate = ( uint16_t )( ( double )XTAL_FREQ / ( double )bitrate );
//     SX1278->RegBitrateMsb    = ( uint8_t )( bitrate >> 8 );
//     SX1278->RegBitrateLsb    = ( uint8_t )( bitrate & 0xFF );
//     SX1278WriteBuffer( REG_BITRATEMSB, &SX1278->RegBitrateMsb, 2 );    
// }

// uint32_t SX1278FskGetBitrate( void )
// {
//     SX1278ReadBuffer( REG_BITRATEMSB, &SX1278->RegBitrateMsb, 2 );
//     FskSettings.Bitrate = ( ( ( uint32_t )SX1278->RegBitrateMsb << 8 ) | ( ( uint32_t )SX1278->RegBitrateLsb ) );
//     FskSettings.Bitrate = ( uint16_t )( ( double )XTAL_FREQ / ( double )FskSettings.Bitrate );

//     return FskSettings.Bitrate;
// }

// void SX1278FskSetFdev( uint32_t fdev )
// {
//     FskSettings.Fdev = fdev;

//     SX1278Read( REG_FDEVMSB, &SX1278->RegFdevMsb ); 

//     fdev = ( uint16_t )( ( double )fdev / ( double )FREQ_STEP );
//     SX1278->RegFdevMsb    = ( ( SX1278->RegFdevMsb & RF_FDEVMSB_FDEV_MASK ) | ( ( ( uint8_t )( fdev >> 8 ) ) & ~RF_FDEVMSB_FDEV_MASK ) );
//     SX1278->RegFdevLsb    = ( uint8_t )( fdev & 0xFF );
//     SX1278WriteBuffer( REG_FDEVMSB, &SX1278->RegFdevMsb, 2 );    
// }

// uint32_t SX1278FskGetFdev( void )
// {
//     SX1278ReadBuffer( REG_FDEVMSB, &SX1278->RegFdevMsb, 2 );
//     FskSettings.Fdev = ( ( ( uint32_t )( ( SX1278->RegFdevMsb << 8 ) & ~RF_FDEVMSB_FDEV_MASK ) ) | ( ( uint32_t )SX1278->RegFdevLsb ) );
//     FskSettings.Fdev = ( uint16_t )( ( double )FskSettings.Fdev * ( double )FREQ_STEP );

//     return FskSettings.Fdev;
// }

// void SX1278FskSetRFPower( int8_t power )
// {
//     SX1278Read( REG_PACONFIG, &SX1278->RegPaConfig );
//     SX1278Read( REG_PADAC, &SX1278->RegPaDac );
    
//     if( ( SX1278->RegPaConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
//     {
//         if( ( SX1278->RegPaDac & 0x87 ) == 0x87 )
//         {
//             if( power < 5 )
//             {
//                 power = 5;
//             }
//             if( power > 20 )
//             {
//                 power = 20;
//             }
//             SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_MAX_POWER_MASK ) | 0x70;
//             SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
//         }
//         else
//         {
//             if( power < 2 )
//             {
//                 power = 2;
//             }
//             if( power > 17 )
//             {
//                 power = 17;
//             }
//             SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_MAX_POWER_MASK ) | 0x70;
//             SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
//         }
//     }
//     else
//     {
//         if( power < -1 )
//         {
//             power = -1;
//         }
//         if( power > 14 )
//         {
//             power = 14;
//         }
//         SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_MAX_POWER_MASK ) | 0x70;
//         SX1278->RegPaConfig = ( SX1278->RegPaConfig & RF_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
//     }
//     SX1278Write( REG_PACONFIG, SX1278->RegPaConfig );
//     FskSettings.Power = power;
// }

// int8_t SX1278FskGetRFPower( void )
// {
//     SX1278Read( REG_PACONFIG, &SX1278->RegPaConfig );
//     SX1278Read( REG_PADAC, &SX1278->RegPaDac );

//     if( ( SX1278->RegPaConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
//     {
//         if( ( SX1278->RegPaDac & 0x07 ) == 0x07 )
//         {
//             FskSettings.Power = 5 + ( SX1278->RegPaConfig & ~RF_PACONFIG_OUTPUTPOWER_MASK );
//         }
//         else
//         {
//             FskSettings.Power = 2 + ( SX1278->RegPaConfig & ~RF_PACONFIG_OUTPUTPOWER_MASK );
//         }
//     }
//     else
//     {
//         FskSettings.Power = -1 + ( SX1278->RegPaConfig & ~RF_PACONFIG_OUTPUTPOWER_MASK );
//     }
//     return FskSettings.Power;
// }

// /*!
//  * \brief Computes the Rx bandwidth with the mantisse and exponent
//  *
//  * \param [IN] mantisse Mantisse of the bandwidth value
//  * \param [IN] exponent Exponent of the bandwidth value
//  * \retval bandwidth Computed bandwidth
//  */
// static uint32_t SX1278FskComputeRxBw( uint8_t mantisse, uint8_t exponent )
// {
//     // rxBw
//     if( ( SX1278->RegOpMode & RF_OPMODE_MODULATIONTYPE_FSK ) == RF_OPMODE_MODULATIONTYPE_FSK )
//     {
//         return ( uint32_t )( ( double )XTAL_FREQ / ( mantisse * ( double )pow( 2, exponent + 2 ) ) );
//     }
//     else
//     {
//         return ( uint32_t )( ( double )XTAL_FREQ / ( mantisse * ( double )pow( 2, exponent + 3 ) ) );
//     }
// }

// /*!
//  * \brief Computes the mantisse and exponent from the bandwitdh value
//  *
//  * \param [IN] rxBwValue Bandwidth value
//  * \param [OUT] mantisse Mantisse of the bandwidth value
//  * \param [OUT] exponent Exponent of the bandwidth value
//  */
// static void SX1278FskComputeRxBwMantExp( uint32_t rxBwValue, uint8_t* mantisse, uint8_t* exponent )
// {
//     uint8_t tmpExp = 0;
//     uint8_t tmpMant = 0;

//     double tmpRxBw = 0;
//     double rxBwMin = 10e6;

//     for( tmpExp = 0; tmpExp < 8; tmpExp++ )
//     {
//         for( tmpMant = 16; tmpMant <= 24; tmpMant += 4 )
//         {
//             if( ( SX1278->RegOpMode & RF_OPMODE_MODULATIONTYPE_FSK ) == RF_OPMODE_MODULATIONTYPE_FSK )
//             {
//                 tmpRxBw = ( double )XTAL_FREQ / ( tmpMant * ( double )pow( 2, tmpExp + 2 ) );
//             }
//             else
//             {
//                 tmpRxBw = ( double )XTAL_FREQ / ( tmpMant * ( double )pow( 2, tmpExp + 3 ) );
//             }
//             if( fabs( tmpRxBw - rxBwValue ) < rxBwMin )
//             {
//                 rxBwMin = fabs( tmpRxBw - rxBwValue );
//                 *mantisse = tmpMant;
//                 *exponent = tmpExp;
//             }
//         }
//     }
// }

// void SX1278FskSetDccBw( uint8_t* reg, uint32_t dccValue, uint32_t rxBwValue )
// {
//     uint8_t mantisse = 0;
//     uint8_t exponent = 0;
    
//     if( reg == &SX1278->RegRxBw )
//     {
//         *reg = ( uint8_t )dccValue & 0x60;
//     }
//     else
//     {
//         *reg = 0;
//     }

//     SX1278FskComputeRxBwMantExp( rxBwValue, &mantisse, &exponent );

//     switch( mantisse )
//     {
//         case 16:
//             *reg |= ( uint8_t )( 0x00 | ( exponent & 0x07 ) );
//             break;
//         case 20:
//             *reg |= ( uint8_t )( 0x08 | ( exponent & 0x07 ) );
//             break;
//         case 24:
//             *reg |= ( uint8_t )( 0x10 | ( exponent & 0x07 ) );
//             break;
//         default:
//             // Something went terribely wrong
//             break;
//     }

//     if( reg == &SX1278->RegRxBw )
//     {
//         SX1278Write( REG_RXBW, *reg );
//         FskSettings.RxBw = rxBwValue;
//     }
//     else
//     {
//         SX1278Write( REG_AFCBW, *reg );
//         FskSettings.RxBwAfc = rxBwValue;
//     }
// }

// uint32_t SX1278FskGetBw( uint8_t* reg )
// {
//     uint32_t rxBwValue = 0;
//     uint8_t mantisse = 0;
//     switch( ( *reg & 0x18 ) >> 3 )
//     {
//         case 0:
//             mantisse = 16;
//             break;
//         case 1:
//             mantisse = 20;
//             break;
//         case 2:
//             mantisse = 24;
//             break;
//         default:
//             break;
//     }
//     rxBwValue = SX1278FskComputeRxBw( mantisse, ( uint8_t )*reg & 0x07 );
//     if( reg == &SX1278->RegRxBw )
//     {
//         return FskSettings.RxBw = rxBwValue;
//     }
//     else
//     {
//         return FskSettings.RxBwAfc = rxBwValue;
//     }
// }

// void SX1278FskSetPacketCrcOn( bool enable )
// {
//     SX1278Read( REG_PACKETCONFIG1, &SX1278->RegPacketConfig1 );
//     SX1278->RegPacketConfig1 = ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_CRC_MASK ) | ( enable << 4 );
//     SX1278Write( REG_PACKETCONFIG1, SX1278->RegPacketConfig1 );
//     FskSettings.CrcOn = enable;
// }

// bool SX1278FskGetPacketCrcOn( void )
// {
//     SX1278Read( REG_PACKETCONFIG1, &SX1278->RegPacketConfig1 );
//     FskSettings.CrcOn = ( SX1278->RegPacketConfig1 & RF_PACKETCONFIG1_CRC_ON ) >> 4;
//     return FskSettings.CrcOn;
// }

// void SX1278FskSetAfcOn( bool enable )
// {
//     SX1278Read( REG_RXCONFIG, &SX1278->RegRxConfig );
//     SX1278->RegRxConfig = ( SX1278->RegRxConfig & RF_RXCONFIG_AFCAUTO_MASK ) | ( enable << 4 );
//     SX1278Write( REG_RXCONFIG, SX1278->RegRxConfig );
//     FskSettings.AfcOn = enable;
// }

// bool SX1278FskGetAfcOn( void )
// {
//     SX1278Read( REG_RXCONFIG, &SX1278->RegRxConfig );
//     FskSettings.AfcOn = ( SX1278->RegRxConfig & RF_RXCONFIG_AFCAUTO_ON ) >> 4;
//     return FskSettings.AfcOn;
// }

// void SX1278FskSetPayloadLength( uint8_t value )
// {
//     SX1278->RegPayloadLength = value;
//     SX1278Write( REG_PAYLOADLENGTH, SX1278->RegPayloadLength );
//     FskSettings.PayloadLength = value;
// }

// uint8_t SX1278FskGetPayloadLength( void )
// {
//     SX1278Read( REG_PAYLOADLENGTH, &SX1278->RegPayloadLength );
//     FskSettings.PayloadLength = SX1278->RegPayloadLength;
//     return FskSettings.PayloadLength;
// }

// void SX1278FskSetPa20dBm( bool enale )
// {
//     SX1278Read( REG_PADAC, &SX1278->RegPaDac );
//     SX1278Read( REG_PACONFIG, &SX1278->RegPaConfig );

//     if( ( SX1278->RegPaConfig & RF_PACONFIG_PASELECT_PABOOST ) == RF_PACONFIG_PASELECT_PABOOST )
//     {    
//         if( enale == true )
//         {
//             SX1278->RegPaDac = 0x87;
//         }
//     }
//     else
//     {
//         SX1278->RegPaDac = 0x84;
//     }
//     SX1278Write( REG_PADAC, SX1278->RegPaDac );
// }

// bool SX1278FskGetPa20dBm( void )
// {
//     SX1278Read( REG_PADAC, &SX1278->RegPaDac );
    
//     return ( ( SX1278->RegPaDac & 0x07 ) == 0x07 ) ? true : false;
// }

// void SX1278FskSetPAOutput( uint8_t outputPin )
// {
//     SX1278Read( REG_PACONFIG, &SX1278->RegPaConfig );
//     SX1278->RegPaConfig = (SX1278->RegPaConfig & RF_PACONFIG_PASELECT_MASK ) | outputPin;
//     SX1278Write( REG_PACONFIG, SX1278->RegPaConfig );
// }

// uint8_t SX1278FskGetPAOutput( void )
// {
//     SX1278Read( REG_PACONFIG, &SX1278->RegPaConfig );
//     return SX1278->RegPaConfig & ~RF_PACONFIG_PASELECT_MASK;
// }


// void SX1278FskSetPaRamp( uint8_t value )
// {
//     SX1278Read( REG_PARAMP, &SX1278->RegPaRamp );
//     SX1278->RegPaRamp = ( SX1278->RegPaRamp & RF_PARAMP_MASK ) | ( value & ~RF_PARAMP_MASK );
//     SX1278Write( REG_PARAMP, SX1278->RegPaRamp );
// }

// uint8_t SX1278FskGetPaRamp( void )
// {
//     SX1278Read( REG_PARAMP, &SX1278->RegPaRamp );
//     return SX1278->RegPaRamp & ~RF_PARAMP_MASK;
// }

// void SX1278FskSetRssiOffset( int8_t offset )
// {
//     SX1278Read( REG_RSSICONFIG, &SX1278->RegRssiConfig );
//     if( offset < 0 )
//     {
//         offset = ( ~offset & 0x1F );
//         offset += 1;
//         offset = -offset;
//     }
//     SX1278->RegRssiConfig |= ( uint8_t )( ( offset & 0x1F ) << 3 );
//     SX1278Write( REG_RSSICONFIG, SX1278->RegRssiConfig );
// }

// int8_t SX1278FskGetRssiOffset( void )
// {
//     SX1278Read( REG_RSSICONFIG, &SX1278->RegRssiConfig );
//     int8_t offset = SX1278->RegRssiConfig >> 3;
//     if( ( offset & 0x10 ) == 0x10 )
//     {
//         offset = ( ~offset & 0x1F );
//         offset += 1;
//         offset = -offset;
//     }
//     return offset;
// }

// int8_t SX1278FskGetRawTemp( void )
// {
//     int8_t temp = 0;
//     uint8_t previousOpMode;
//     uint32_t startTick;
    
//     // Enable Temperature reading
//     SX1278Read( REG_IMAGECAL, &SX1278->RegImageCal );
//     SX1278->RegImageCal = ( SX1278->RegImageCal & RF_IMAGECAL_TEMPMONITOR_MASK ) | RF_IMAGECAL_TEMPMONITOR_ON;
//     SX1278Write( REG_IMAGECAL, SX1278->RegImageCal );

//     // save current Op Mode
//     SX1278Read( REG_OPMODE, &SX1278->RegOpMode );
//     previousOpMode = SX1278->RegOpMode;

//     // put device in FSK RxSynth
//     SX1278->RegOpMode = RF_OPMODE_SYNTHESIZER_RX;
//     SX1278Write( REG_OPMODE, SX1278->RegOpMode );

//     // Wait 1ms
//     startTick = GET_TICK_COUNT( );
//     while( ( GET_TICK_COUNT( ) - startTick ) < TICK_RATE_MS( 1 ) );  

//     // Disable Temperature reading
//     SX1278Read( REG_IMAGECAL, &SX1278->RegImageCal );
//     SX1278->RegImageCal = ( SX1278->RegImageCal & RF_IMAGECAL_TEMPMONITOR_MASK ) | RF_IMAGECAL_TEMPMONITOR_OFF;
//     SX1278Write( REG_IMAGECAL, SX1278->RegImageCal );

//     // Read temperature
//     SX1278Read( REG_TEMP, &SX1278->RegTemp );
    
//     temp = SX1278->RegTemp & 0x7F;
    
//     if( ( SX1278->RegTemp & 0x80 ) == 0x80 )
//     {
//         temp *= -1;
//     }

//     // Reload previous Op Mode
//     SX1278Write( REG_OPMODE, previousOpMode );

//     return temp;
// }

// int8_t SX1278FskCalibreateTemp( int8_t actualTemp )
// {
//     return actualTemp - SX1278FskGetRawTemp( );
// }

// int8_t SX1278FskGetTemp( int8_t compensationFactor )
// {
//     return SX1278FskGetRawTemp( ) + compensationFactor;
// }

// #endif // USE_SX1278_RADIO






//--- end of file ---//
