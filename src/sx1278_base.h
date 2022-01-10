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
void SxBaseRst (void);
unsigned char SxBaseCheckDeviceID (void);
void SxBaseWrite (unsigned char addr, unsigned char data);
unsigned char SxBaseRead (unsigned char addr);
void SxBaseBurstWrite (unsigned char addr, unsigned char * pdata, unsigned char len);
void SxBaseBurstRead (unsigned char addr, unsigned char * pdata, unsigned char len);


#endif    /* _SX1278_BASE_H_ */

//--- end of file ---//

