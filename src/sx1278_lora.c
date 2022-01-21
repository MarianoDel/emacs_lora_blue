//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_LORA.C ############################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "sx1278_base.h"
#include "sx1278_lora.h"
#include "tim.h"
#include "spi.h"
#include "hard.h"

#include <stdio.h>
#include <string.h>


// Module Private Types Constants & Macros -------------------------------------
/*!
 * Constant values need to compute the RSSI value
 */
#define RSSI_OFFSET_LF    -164
#define RSSI_OFFSET_HF    -157


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
// RfState_e RFState = RF_NO_MODULE;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------


// Default settings
// tLoRaSettings LoRaSettings =
// {
//     434000000,        // RFFrequency
//     20,               // Power
//     9,                // SignalBw [0: 7.8kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
//                       // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]
//     7,                // SpreadingFactor [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
//     2,                // ErrorCoding [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
//     true,             // CrcOn [0: OFF, 1: ON]
//     false,            // ImplicitHeaderOn [0: OFF, 1: ON]
//     1,                // RxSingleOn [0: Continuous, 1 Single]
//     0,                // FreqHopOn [0: OFF, 1: ON]
//     4,                // HopPeriod Hops every frequency hopping period symbols
//     100,              // TxPacketTimeout
//     100,              // RxPacketTimeout
//     128,              // PayloadLength (used for implicit header mode)
// };

unsigned char SxLoRaInit( void )
{
    // RFState = RF_NO_MODULE;
    SxBaseRst();
    if (!SxBaseCheckDeviceID())
        return 0;

    // change to lora mode and registers
    SxBaseWrite(REG_LR_OPMODE, RFLR_OPMODE_SLEEP);
    
    SxBaseWrite(REG_LR_OPMODE,
                RFLR_OPMODE_LONGRANGEMODE_ON |
                RFLR_OPMODE_FREQMODE_ACCESS_LF |
                RFLR_OPMODE_SLEEP);

    SxLoraSetOpMode(RFLR_OPMODE_STANDBY);

    // set freq
    // SxLoraSetFreq(434000000);    // defaults are good

    /////////////////////////
    // rf blocks settings  //
    /////////////////////////
    // set power output
    // SxBaseWrite(REG_LR_PACONFIG, RFLR_PACONFIG_PASELECT_PABOOST | 0x0f);    // power +17dBm
    SxBaseWrite(REG_LR_PACONFIG, RFLR_PACONFIG_PASELECT_PABOOST);    // power +2dBm

    // pa ramp
    // SxBaseWrite(REG_LR_PARAMP, RFLR_PARAMP_0040_US);    // defaults are good

    // set overload current
    // SxBaseWrite(REG_LR_OCP, RFLR_OCP_ON | RFLR_OCP_TRIM_100_MA);    // defaults are good

    // lna config
    // SxBaseWrite(REG_LR_LNA, RFLR_LNA_GAIN_G1);    // defaults are good

    // set spread config
    // SxBaseWrite(REG_LR_MODEMCONFIG1,
    //             RFLR_MODEMCONFIG1_BW_125_KHZ |
    //             RFLR_MODEMCONFIG1_CODINGRATE_4_5 |
    //             RFLR_MODEMCONFIG1_IMPLICITHEADER_OFF);    // defaults are good

    // SxBaseWrite(REG_LR_MODEMCONFIG2, RFLR_MODEMCONFIG2_SF_7);    // defaults are good

    // preamble length
    // SxBaseWrite(REG_LR_PREAMBLEMSB, 0x00);
    // SxBaseWrite(REG_LR_PREAMBLELSB, 0x08);    // defaults are good

                
    // set the RF settings 
    // SX1278LoRaSetRFFrequency( LoRaSettings.RFFrequency );
    // SX1278LoRaSetSpreadingFactor( LoRaSettings.SpreadingFactor ); // SF6 only operates in implicit header mode.
    // SX1278LoRaSetErrorCoding( LoRaSettings.ErrorCoding );
    // SX1278LoRaSetPacketCrcOn( LoRaSettings.CrcOn );
    // SX1278LoRaSetSignalBandwidth( LoRaSettings.SignalBw );

    // SX1278LoRaSetImplicitHeaderOn( LoRaSettings.ImplicitHeaderOn );
    // SX1278LoRaSetSymbTimeout( 0x3FF );
    // SX1278LoRaSetPayloadLength( LoRaSettings.PayloadLength );
    //SX1278LoRaSetLowDatarateOptimize( true );

    return 1;
}


void SxLoraSetOpMode (unsigned char new_mode)
{
    unsigned char last_mode = 0;
    
    last_mode = SxBaseRead(REG_LR_OPMODE);
    last_mode &= RFLR_OPMODE_MASK;

    new_mode &= (~(RFLR_OPMODE_MASK));
    new_mode |= last_mode;
    SxBaseWrite(REG_LR_OPMODE, new_mode);
}


unsigned char SxLoraGetOpMode (void)
{
    unsigned char last_mode = 0;
    
    last_mode = SxBaseRead(REG_LR_OPMODE);
    last_mode &= (~(RFLR_OPMODE_MASK));

    return last_mode;
}


short SxLoraGetCurrentRssi (void)
{
    unsigned char rssi = 0;
    rssi = SxBaseRead(REG_LR_RSSIVALUE);

    return (RSSI_OFFSET_LF + rssi);
}


short SxLoraGetPcktSnr (void)
{
    short snr = 0;
    unsigned char snr_reg = SxBaseRead (REG_LR_PKTSNRVALUE);
    snr = snr_reg / 4;
    
    return snr;
}


short SxLoraGetPcktRssi (void)
{
    unsigned char rssi = SxBaseRead(REG_LR_PKTRSSIVALUE);

    return (RSSI_OFFSET_LF + rssi);
}


short SxLoraGetPcktStrength (void)
{
    short snr = SxLoraGetPcktSnr();
    short rssi = SxLoraGetPcktRssi();
    
    if (snr >= 0)
        return (RSSI_OFFSET_LF + rssi);
    else
        return (RSSI_OFFSET_LF + rssi + snr);
    
}




// #define XTAL_FREQ                                   32000000
// #define FREQ_STEP                                   61.03515625

// extern tLoRaSettings LoRaSettings;

// void SX1278LoRaSetRFFrequency( uint32_t freq )
// {
//     LoRaSettings.RFFrequency = freq;

//     freq = ( uint32_t )( ( double )freq / ( double )FREQ_STEP );
//     SX1278LR->RegFrfMsb = ( uint8_t )( ( freq >> 16 ) & 0xFF );
//     SX1278LR->RegFrfMid = ( uint8_t )( ( freq >> 8 ) & 0xFF );
//     SX1278LR->RegFrfLsb = ( uint8_t )( freq & 0xFF );
//     SX1278WriteBuffer( REG_LR_FRFMSB, &SX1278LR->RegFrfMsb, 3 );
// }

// uint32_t SX1278LoRaGetRFFrequency( void )
// {
//     SX1278ReadBuffer( REG_LR_FRFMSB, &SX1278LR->RegFrfMsb, 3 );
//     LoRaSettings.RFFrequency = ( ( uint32_t )SX1278LR->RegFrfMsb << 16 ) | ( ( uint32_t )SX1278LR->RegFrfMid << 8 ) | ( ( uint32_t )SX1278LR->RegFrfLsb );
//     LoRaSettings.RFFrequency = ( uint32_t )( ( double )LoRaSettings.RFFrequency * ( double )FREQ_STEP );

//     return LoRaSettings.RFFrequency;
// }

// void SX1278LoRaSetRFPower( int8_t power )
// {
//     SX1278Read( REG_LR_PACONFIG, &SX1278LR->RegPaConfig );
//     SX1278Read( REG_LR_PADAC, &SX1278LR->RegPaDac );
    
//     if( ( SX1278LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
//     {
//         if( ( SX1278LR->RegPaDac & 0x87 ) == 0x87 )
//         {
//             if( power < 5 )
//             {
//                 power = 5;
//             }
//             if( power > 20 )
//             {
//                 power = 20;
//             }
//             SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
//             SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 5 ) & 0x0F );
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
//             SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
//             SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power - 2 ) & 0x0F );
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
//         SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_MAX_POWER_MASK ) | 0x70;
//         SX1278LR->RegPaConfig = ( SX1278LR->RegPaConfig & RFLR_PACONFIG_OUTPUTPOWER_MASK ) | ( uint8_t )( ( uint16_t )( power + 1 ) & 0x0F );
//     }
//     SX1278Write( REG_LR_PACONFIG, SX1278LR->RegPaConfig );
//     LoRaSettings.Power = power;
// }

// int8_t SX1278LoRaGetRFPower( void )
// {
//     SX1278Read( REG_LR_PACONFIG, &SX1278LR->RegPaConfig );
//     SX1278Read( REG_LR_PADAC, &SX1278LR->RegPaDac );

//     if( ( SX1278LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
//     {
//         if( ( SX1278LR->RegPaDac & 0x07 ) == 0x07 )
//         {
//             LoRaSettings.Power = 5 + ( SX1278LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
//         }
//         else
//         {
//             LoRaSettings.Power = 2 + ( SX1278LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
//         }
//     }
//     else
//     {
//         LoRaSettings.Power = -1 + ( SX1278LR->RegPaConfig & ~RFLR_PACONFIG_OUTPUTPOWER_MASK );
//     }
//     return LoRaSettings.Power;
// }

// void SX1278LoRaSetSignalBandwidth( uint8_t bw )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     SX1278LR->RegModemConfig1 = ( SX1278LR->RegModemConfig1 & RFLR_MODEMCONFIG1_BW_MASK ) | ( bw << 4 );
//     SX1278Write( REG_LR_MODEMCONFIG1, SX1278LR->RegModemConfig1 );
//     LoRaSettings.SignalBw = bw;
// }

// uint8_t SX1278LoRaGetSignalBandwidth( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     LoRaSettings.SignalBw = ( SX1278LR->RegModemConfig1 & ~RFLR_MODEMCONFIG1_BW_MASK ) >> 4;
//     return LoRaSettings.SignalBw;
// }

// void SX1278LoRaSetSpreadingFactor( uint8_t factor )
// {

//     if( factor > 12 )
//     {
//         factor = 12;
//     }
//     else if( factor < 6 )
//     {
//         factor = 6;
//     }

//     if( factor == 6 )
//     {
//         SX1278LoRaSetNbTrigPeaks( 5 );
//     }
//     else
//     {
//         SX1278LoRaSetNbTrigPeaks( 3 );
//     }

//     SX1278Read( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2 );    
//     SX1278LR->RegModemConfig2 = ( SX1278LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SF_MASK ) | ( factor << 4 );
//     SX1278Write( REG_LR_MODEMCONFIG2, SX1278LR->RegModemConfig2 );    
//     LoRaSettings.SpreadingFactor = factor;
// }

// uint8_t SX1278LoRaGetSpreadingFactor( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2 );   
//     LoRaSettings.SpreadingFactor = ( SX1278LR->RegModemConfig2 & ~RFLR_MODEMCONFIG2_SF_MASK ) >> 4;
//     return LoRaSettings.SpreadingFactor;
// }

// void SX1278LoRaSetErrorCoding( uint8_t value )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     SX1278LR->RegModemConfig1 = ( SX1278LR->RegModemConfig1 & RFLR_MODEMCONFIG1_CODINGRATE_MASK ) | ( value << 1 );
//     SX1278Write( REG_LR_MODEMCONFIG1, SX1278LR->RegModemConfig1 );
//     LoRaSettings.ErrorCoding = value;
// }

// uint8_t SX1278LoRaGetErrorCoding( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     LoRaSettings.ErrorCoding = ( SX1278LR->RegModemConfig1 & ~RFLR_MODEMCONFIG1_CODINGRATE_MASK ) >> 1;
//     return LoRaSettings.ErrorCoding;
// }

// void SX1278LoRaSetPacketCrcOn( bool enable )
// {
//     SX1278Read( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2 );
//     SX1278LR->RegModemConfig2 = ( SX1278LR->RegModemConfig2 & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK ) | ( enable << 2 );
//     SX1278Write( REG_LR_MODEMCONFIG2, SX1278LR->RegModemConfig2 );
//     LoRaSettings.CrcOn = enable;
// }

// void SX1278LoRaSetPreambleLength( uint16_t value )
// {
//     SX1278ReadBuffer( REG_LR_PREAMBLEMSB, &SX1278LR->RegPreambleMsb, 2 );

//     SX1278LR->RegPreambleMsb = ( value >> 8 ) & 0x00FF;
//     SX1278LR->RegPreambleLsb = value & 0xFF;
//     SX1278WriteBuffer( REG_LR_PREAMBLEMSB, &SX1278LR->RegPreambleMsb, 2 );
// }

// uint16_t SX1278LoRaGetPreambleLength( void )
// {
//     SX1278ReadBuffer( REG_LR_PREAMBLEMSB, &SX1278LR->RegPreambleMsb, 2 );
//     return ( ( SX1278LR->RegPreambleMsb & 0x00FF ) << 8 ) | SX1278LR->RegPreambleLsb;
// }

// bool SX1278LoRaGetPacketCrcOn( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2 );
//     LoRaSettings.CrcOn = ( SX1278LR->RegModemConfig2 & RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON ) >> 1;
//     return LoRaSettings.CrcOn;
// }

// void SX1278LoRaSetImplicitHeaderOn( bool enable )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     SX1278LR->RegModemConfig1 = ( SX1278LR->RegModemConfig1 & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK ) | ( enable );
//     SX1278Write( REG_LR_MODEMCONFIG1, SX1278LR->RegModemConfig1 );
//     LoRaSettings.ImplicitHeaderOn = enable;
// }

// bool SX1278LoRaGetImplicitHeaderOn( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG1, &SX1278LR->RegModemConfig1 );
//     LoRaSettings.ImplicitHeaderOn = ( SX1278LR->RegModemConfig1 & RFLR_MODEMCONFIG1_IMPLICITHEADER_ON );
//     return LoRaSettings.ImplicitHeaderOn;
// }

// void SX1278LoRaSetRxSingleOn( bool enable )
// {
//     LoRaSettings.RxSingleOn = enable;
// }

// bool SX1278LoRaGetRxSingleOn( void )
// {
//     return LoRaSettings.RxSingleOn;
// }

// void SX1278LoRaSetFreqHopOn( bool enable )
// {
//     LoRaSettings.FreqHopOn = enable;
// }

// bool SX1278LoRaGetFreqHopOn( void )
// {
//     return LoRaSettings.FreqHopOn;
// }

// void SX1278LoRaSetPayloadLength( uint8_t value )
// {
//     SX1278LR->RegPayloadLength = value;
//     SX1278Write( REG_LR_PAYLOADLENGTH, SX1278LR->RegPayloadLength );
//     LoRaSettings.PayloadLength = value;
// }

// uint8_t SX1278LoRaGetPayloadLength( void )
// {
//     SX1278Read( REG_LR_PAYLOADLENGTH, &SX1278LR->RegPayloadLength );
//     LoRaSettings.PayloadLength = SX1278LR->RegPayloadLength;
//     return LoRaSettings.PayloadLength;
// }

// void SX1278LoRaSetPa20dBm( bool enale )
// {
//     SX1278Read( REG_LR_PADAC, &SX1278LR->RegPaDac );
//     SX1278Read( REG_LR_PACONFIG, &SX1278LR->RegPaConfig );

//     if( ( SX1278LR->RegPaConfig & RFLR_PACONFIG_PASELECT_PABOOST ) == RFLR_PACONFIG_PASELECT_PABOOST )
//     {    
//         if( enale == true )
//         {
//             SX1278LR->RegPaDac = 0x87;
//         }
//     }
//     else
//     {
//         SX1278LR->RegPaDac = 0x84;
//     }
//     SX1278Write( REG_LR_PADAC, SX1278LR->RegPaDac );
// }

// bool SX1278LoRaGetPa20dBm( void )
// {
//     SX1278Read( REG_LR_PADAC, &SX1278LR->RegPaDac );
    
//     return ( ( SX1278LR->RegPaDac & 0x07 ) == 0x07 ) ? true : false;
// }

// void SX1278LoRaSetPAOutput( uint8_t outputPin )
// {
//     SX1278Read( REG_LR_PACONFIG, &SX1278LR->RegPaConfig );
//     SX1278LR->RegPaConfig = (SX1278LR->RegPaConfig & RFLR_PACONFIG_PASELECT_MASK ) | outputPin;
//     SX1278Write( REG_LR_PACONFIG, SX1278LR->RegPaConfig );
// }

// uint8_t SX1278LoRaGetPAOutput( void )
// {
//     SX1278Read( REG_LR_PACONFIG, &SX1278LR->RegPaConfig );
//     return SX1278LR->RegPaConfig & ~RFLR_PACONFIG_PASELECT_MASK;
// }

// void SX1278LoRaSetPaRamp( uint8_t value )
// {
//     SX1278Read( REG_LR_PARAMP, &SX1278LR->RegPaRamp );
//     SX1278LR->RegPaRamp = ( SX1278LR->RegPaRamp & RFLR_PARAMP_MASK ) | ( value & ~RFLR_PARAMP_MASK );
//     SX1278Write( REG_LR_PARAMP, SX1278LR->RegPaRamp );
// }

// uint8_t SX1278LoRaGetPaRamp( void )
// {
//     SX1278Read( REG_LR_PARAMP, &SX1278LR->RegPaRamp );
//     return SX1278LR->RegPaRamp & ~RFLR_PARAMP_MASK;
// }

// void SX1278LoRaSetSymbTimeout( uint16_t value )
// {
//     SX1278ReadBuffer( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2, 2 );

//     SX1278LR->RegModemConfig2 = ( SX1278LR->RegModemConfig2 & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) | ( ( value >> 8 ) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK );
//     SX1278LR->RegSymbTimeoutLsb = value & 0xFF;
//     SX1278WriteBuffer( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2, 2 );
// }

// uint16_t SX1278LoRaGetSymbTimeout( void )
// {
//     SX1278ReadBuffer( REG_LR_MODEMCONFIG2, &SX1278LR->RegModemConfig2, 2 );
//     return ( ( SX1278LR->RegModemConfig2 & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK ) << 8 ) | SX1278LR->RegSymbTimeoutLsb;
// }

// void SX1278LoRaSetLowDatarateOptimize( bool enable )
// {
//     SX1278Read( REG_LR_MODEMCONFIG3, &SX1278LR->RegModemConfig3 );
//     SX1278LR->RegModemConfig3 = ( SX1278LR->RegModemConfig3 & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_MASK ) | ( enable << 3 );
//     SX1278Write( REG_LR_MODEMCONFIG3, SX1278LR->RegModemConfig3 );
// }

// bool SX1278LoRaGetLowDatarateOptimize( void )
// {
//     SX1278Read( REG_LR_MODEMCONFIG3, &SX1278LR->RegModemConfig3 );
//     return ( ( SX1278LR->RegModemConfig3 & RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON ) >> 3 );
// }

// void SX1278LoRaSetNbTrigPeaks( uint8_t value )
// {
//     SX1278Read( 0x31, &SX1278LR->RegDetectOptimize );
//     SX1278LR->RegDetectOptimize = ( SX1278LR->RegDetectOptimize & 0xF8 ) | value;
//     SX1278Write( 0x31, SX1278LR->RegDetectOptimize );
// }

// uint8_t SX1278LoRaGetNbTrigPeaks( void )
// {
//     SX1278Read( 0x31, &SX1278LR->RegDetectOptimize );
//     return ( SX1278LR->RegDetectOptimize & 0x07 );
// }

// #endif // USE_SX1278_RADIO


//--- end of file ---//
