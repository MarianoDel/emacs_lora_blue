//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_BASE.H ############################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _SX1278_BASE_H_
#define _SX1278_BASE_H_


// Module Exported Types Constants and Macros ----------------------------------


// Module Exported Functions ---------------------------------------------------
void SX1278FskSetRFFrequency( uint32_t freq );
uint32_t SX1278FskGetRFFrequency( void );
void SX1278FskRxCalibrate( void );
void SX1278FskSetBitrate( uint32_t bitrate );
uint32_t SX1278FskGetBitrate( void );    // [bps]
void SX1278FskSetFdev( uint32_t fdev );    // [Hz]
uint32_t SX1278FskGetFdev( void );    // [Hz]
void SX1278FskSetRFPower( int8_t power );    // [dBm]
int8_t SX1278FskGetRFPower( void );    // [dBm]
void SX1278FskSetDccBw( uint8_t* reg, uint32_t dccValue, uint32_t rxBwValue );
uint32_t SX1278FskGetBw( uint8_t* reg );
void SX1278FskSetPacketCrcOn( bool enable );
bool SX1278FskGetPacketCrcOn( void );
void SX1278FskSetAfcOn( bool enable );
bool SX1278FskGetAfcOn( void );
void SX1278FskSetPayloadLength( uint8_t value );
uint8_t SX1278FskGetPayloadLength( void );
void SX1278FskSetPa20dBm( bool enale );
bool SX1278FskGetPa20dBm( void );
void SX1278FskSetPAOutput( uint8_t outputPin );
uint8_t SX1278FskGetPAOutput( void );
void SX1278FskSetPaRamp( uint8_t value );
uint8_t SX1278FskGetPaRamp( void );
void SX1278FskSetRssiOffset( int8_t offset );
int8_t SX1278FskGetRssiOffset( void );
void SX1278FskSetPreambleSize( uint16_t size );
int8_t SX1278FskGetRawTemp( void );
int8_t SX1278FskCalibreateTemp( int8_t actualTemp );
int8_t SX1278FskGetTemp( int8_t compensationFactor );


#endif    /* _SX1278_BASE_H_ */

//--- end of file ---//

