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


// Module Private Types & Macros -----------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------
RfState_e RFState = RF_NO_MODULE;


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
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

    //////////////////////////
    // transmitter settings //
    //////////////////////////
    // set power output
    // SxBaseWrite(REG_PACONFIG, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good
    // SxBaseWrite(REG_PACONFIG, RF_PACONFIG_PASELECT_PABOOST | 0x0f);    // power +17dBm
    SxBaseWrite(REG_PACONFIG, RF_PACONFIG_PASELECT_PABOOST);    // power +2dBm    

    SxBaseWrite(REG_PARAMP,
                RF_PARAMP_MODULATIONSHAPING_01 |
                RF_PARAMP_0040_US);    // BT = 1
    
    // SxBaseWrite(REG_PARAMP,
    //             RF_PARAMP_MODULATIONSHAPING_10 |
    //             RF_PARAMP_0040_US);    // BT = 0.5

    // set overload current
    // SxBaseWrite(REG_OCP, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good

    ///////////////////////
    // receiver settings //
    ///////////////////////
    // set lna
    // SxBaseWrite(REG_LNA, RF_LNA_GAIN_G1);    // defaults are good

    // rx config
    // SxBaseWrite(REG_RXCONFIG,
    //             RF_RXCONFIG_RESTARTRXONCOLLISION_OFF |
    //             RF_RXCONFIG_AFCAUTO_OFF |
    //             RF_RXCONFIG_AGCAUTO_ON |
    //             RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT);    // defauls are good

    // rssi config
    // SxBaseWrite(REG_RSSICONFIG, 0xFF);    // defaults are good

    // rssi collision
    // SxBaseWrite(REG_RSSICOLLISION, 0xFF);    // defaults are good

    // rssi threshold
    SxBaseWrite(REG_RSSITHRESH, 240);    // -120dBm detection
    // SxBaseWrite(REG_RSSITHRESH, 220);    // -110dBm detection
    // SxBaseWrite(REG_RSSITHRESH, 200);    // -100dBm detection    
    // SxBaseWrite(REG_RSSITHRESH, 160);    // -80dBm detection    

    // rx bandwith
    // SxBaseWrite(REG_RXBW, 0xFF);    // defaults are good

    // rx afc bw
    // SxBaseWrite(REG_AFCBW, 0xFF);    // defaults are good

    // rx ook peak
    // SxBaseWrite(REG_OOKPEAK, RF_OOKPEAK_BITSYNC_ON);    // defaults are good
    
    // afc and fei to defaults

    // rx preamble config
    SxBaseWrite(REG_PREAMBLEDETECT,
                RF_PREAMBLEDETECT_DETECTOR_ON |
                RF_PREAMBLEDETECT_DETECTORSIZE_2 |
                RF_PREAMBLEDETECT_DETECTORTOL_10);    // preable det err 0%

    // SxBaseWrite(REG_PREAMBLEDETECT,
    //             RF_PREAMBLEDETECT_DETECTOR_ON |
    //             RF_PREAMBLEDETECT_DETECTORSIZE_2 |
    //             RF_PREAMBLEDETECT_DETECTORTOL_4);    // preamble det err 14%
    
    // SxBaseWrite(REG_PREAMBLEDETECT,
    //             RF_PREAMBLEDETECT_DETECTOR_OFF |
    //             RF_PREAMBLEDETECT_DETECTORSIZE_2 |
    //             RF_PREAMBLEDETECT_DETECTORTOL_4);    // no preable detector
    
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


void SxFskSetFreqInt (unsigned int freq)
{
    unsigned long long int freg = 0;
    unsigned char buff[3] = { 0 };

    freg = freq;
    freg = freg * 524288;
    freg = freg / 32000000;
    buff[0] = (unsigned char) (( freg >> 16 ) & 0xFF);
    buff[1] = (unsigned char) (( freg >> 8 ) & 0xFF);
    buff[2] = (unsigned char) (( freg ) & 0xFF);
    SxBaseBurstWrite(REG_FRFMSB, buff, 3);
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


void SxFskSetFreqDev (unsigned int fdev)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    
    freg = ( unsigned int )( ( float )fdev / ( float )FREQ_STEP );    // original
    buff[0] = (unsigned char) (( freg >> 8 ) & 0xFF);
    buff[1] = (unsigned char) (( freg ) & 0xFF);
    SxBaseBurstWrite(REG_FDEVMSB, buff, 2);
}


unsigned int SxFskGetFreqDev (void)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    SxBaseBurstRead(REG_FDEVMSB, buff, 2);

    freg = buff[0] << 8 ;
    freg += buff[1];
    float fdev = (float)freg * (float)FREQ_STEP;
    return (unsigned int)(fdev);
}


void SxFskSetBitrate (unsigned int bitrate)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    
    freg = XTAL_FREQ / bitrate;
    buff[0] = (unsigned char) (( freg >> 8 ) & 0xFF);
    buff[1] = (unsigned char) (( freg ) & 0xFF);
    SxBaseBurstWrite(REG_BITRATEMSB, buff, 2);
}


unsigned int SxFskGetBitrate (void)
{
    unsigned int freg = 0;
    unsigned char buff[10] = { 0 };
    SxBaseBurstRead(REG_BITRATEMSB, buff, 2);

    freg = buff[0] << 8 ;
    freg += buff[1];
    return XTAL_FREQ / freg;
}


void SxFskSetModuletionIndex (unsigned int bitrate, float mi)
{
    unsigned int fdev = bitrate / 2;
    fdev = fdev * mi;    

    // set bitrate
    SxFskSetBitrate (bitrate);
    // set deviation
    SxFskSetFreqDev (fdev);
}


float SxFskGetModuletionIndex (void)
{
    unsigned int bitrate = SxFskGetBitrate ();    
    unsigned int fdev = SxFskGetFreqDev ();

    float mi = 2 * (float)fdev / (float)bitrate;
    return mi;
}


short SxFskGetRssiValue (void)
{
    short rssi = SxBaseRead (REG_RSSIVALUE);
    return (-rssi / 2);
}


//////////////////////////
// Fsk Packet Functions //
//////////////////////////
unsigned char SxFskPacketInit (void)
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

    //////////////////////////
    // transmitter settings //
    //////////////////////////
    // set power output
    // SxBaseWrite(REG_PACONFIG, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good
    // SxBaseWrite(REG_PACONFIG, RF_PACONFIG_PASELECT_PABOOST | 0x0f);    // power +17dBm
    SxBaseWrite(REG_PACONFIG, RF_PACONFIG_PASELECT_PABOOST);    // power +2dBm    

    SxBaseWrite(REG_PARAMP,
                RF_PARAMP_MODULATIONSHAPING_01 |
                RF_PARAMP_0040_US);    // BT = 1
    
    // SxBaseWrite(REG_PARAMP,
    //             RF_PARAMP_MODULATIONSHAPING_10 |
    //             RF_PARAMP_0040_US);    // BT = 0.5

    // set overload current
    // SxBaseWrite(REG_OCP, RF_PARAMP_MODULATIONSHAPING_01);    // defaults are good

    ///////////////////////
    // receiver settings //
    ///////////////////////
    // set lna
    // SxBaseWrite(REG_LNA, RF_LNA_GAIN_G1);    // defaults are good

    // rx config
    // SxBaseWrite(REG_RXCONFIG,
    //             RF_RXCONFIG_RESTARTRXONCOLLISION_OFF |
    //             RF_RXCONFIG_AFCAUTO_OFF |
    //             RF_RXCONFIG_AGCAUTO_ON |
    //             RF_RXCONFIG_RXTRIGER_PREAMBLEDETECT);    // defauls are good

    // rssi config
    // SxBaseWrite(REG_RSSICONFIG, 0xFF);    // defaults are good

    // rssi collision
    // SxBaseWrite(REG_RSSICOLLISION, 0xFF);    // defaults are good

    // rssi threshold
    // SxBaseWrite(REG_RSSITHRESH, 240);    // -120dBm detection
    SxBaseWrite(REG_RSSITHRESH, 220);    // -110dBm detection    
    // SxBaseWrite(REG_RSSITHRESH, 200);    // -100dBm detection    
    // SxBaseWrite(REG_RSSITHRESH, 160);    // -80dBm detection    

    // rx bandwith
    // SxBaseWrite(REG_RXBW, 0xFF);    // defaults are good

    // rx afc bw
    // SxBaseWrite(REG_AFCBW, 0xFF);    // defaults are good

    // rx ook peak
    // SxBaseWrite(REG_OOKPEAK, RF_OOKPEAK_BITSYNC_ON);    // defaults are good
    
    // afc and fei to defaults

    // rx preamble config
    SxBaseWrite(REG_PREAMBLEDETECT,
                RF_PREAMBLEDETECT_DETECTOR_ON |
                RF_PREAMBLEDETECT_DETECTORSIZE_2 |
                RF_PREAMBLEDETECT_DETECTORTOL_10);    // defaults are good

    // SxBaseWrite(REG_PREAMBLEDETECT,
    //             RF_PREAMBLEDETECT_DETECTOR_OFF |
    //             RF_PREAMBLEDETECT_DETECTORSIZE_2 |
    //             RF_PREAMBLEDETECT_DETECTORTOL_4);    // no preable detector
    
    // preamble size
    // SxBaseWrite(REG_PREAMBLEMSB, 0xFF);    // defaults are good
    // SxBaseWrite(REG_PREAMBLELSB, 0xFF);    // defaults are good

    // sync config
    // SxBaseWrite(REG_SYNCCONFIG, 0xFF);    // defaults are good
    SxBaseWrite(REG_SYNCCONFIG,
                RF_SYNCCONFIG_AUTORESTARTRXMODE_WAITPLL_ON |
                RF_SYNCCONFIG_SYNC_ON |
                RF_SYNCCONFIG_SYNCSIZE_2);

    // sync key
    // SxBaseWrite(REG_SYNCVALUE1, 0xFF);    // defaults are good
    SxBaseWrite(REG_SYNCVALUE1, 0x01);    // sync to 0x01
    SxBaseWrite(REG_SYNCVALUE2, 0x66);    // defaults are good
    // SxBaseWrite(REG_SYNCVALUE3, 0xFF);    // defaults are good
    // SxBaseWrite(REG_SYNCVALUE4, 0xFF);    // defaults are good    
    // SX1278->RegSyncValue1 = 0x69;
    // SX1278->RegSyncValue2 = 0x81;
    // SX1278->RegSyncValue3 = 0x7E;
    // SX1278->RegSyncValue4 = 0x96;

    // packet config
    SxBaseWrite(REG_PACKETCONFIG1,
                RF_PACKETCONFIG1_PACKETFORMAT_FIXED |
                RF_PACKETCONFIG1_DCFREE_OFF |
                RF_PACKETCONFIG1_CRC_ON |
                RF_PACKETCONFIG1_CRCAUTOCLEAR_ON |
                RF_PACKETCONFIG1_ADDRSFILTERING_OFF |
                RF_PACKETCONFIG1_CRCWHITENINGTYPE_CCITT);    // clean fifo on crc error

    // SxBaseWrite(REG_PACKETCONFIG1,
    //             RF_PACKETCONFIG1_PACKETFORMAT_FIXED |
    //             RF_PACKETCONFIG1_DCFREE_OFF |
    //             RF_PACKETCONFIG1_CRC_ON |
    //             RF_PACKETCONFIG1_CRCAUTOCLEAR_OFF |
    //             RF_PACKETCONFIG1_ADDRSFILTERING_OFF |
    //             RF_PACKETCONFIG1_CRCWHITENINGTYPE_CCITT);    // no fifo clean on crc error
    
    // SxBaseWrite(REG_PACKETCONFIG1,
    //             RF_PACKETCONFIG1_PACKETFORMAT_VARIABLE |
    //             RF_PACKETCONFIG1_DCFREE_OFF |
    //             RF_PACKETCONFIG1_CRC_ON |
    //             RF_PACKETCONFIG1_CRCAUTOCLEAR_ON |
    //             RF_PACKETCONFIG1_ADDRSFILTERING_OFF |
    //             RF_PACKETCONFIG1_CRCWHITENINGTYPE_CCITT);    // defaults are good
    
    SxBaseWrite(REG_PACKETCONFIG2, RF_PACKETCONFIG2_DATAMODE_PACKET);    // 

    // packet length
    // SxBaseWrite(REG_PAYLOADLENGTH, RF_PACKETCONFIG2_DATAMODE_PACKET);    // 

    // SX1278FskSetOpMode( RF_OPMODE_STANDBY );

    // Calibrate the HF
    // SX1278FskRxCalibrate( );
    RFState = RF_IDLE;
    return 1;

}



//--- end of file ---//
