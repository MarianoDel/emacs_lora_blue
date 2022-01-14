//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### SX1278_BASE_IO.H #########################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _SX1278_BASE_IO_H_
#define _SX1278_BASE_IO_H_


// Module Exported Types Constants and Macros ----------------------------------


// Module Exported Functions ---------------------------------------------------
void Sx_Rst_On (void);
void Sx_Rst_Off (void);
void Sx_Cs_On (void);
void Sx_Cs_Off (void);

unsigned char Sx_Dio0_Get (void);

void Sx_Dio1_Output (void);
void Sx_Dio1_Input (void);
unsigned char Sx_Dio1_Get (void);

void Sx_Dio2_Output (void);
void Sx_Dio2_Input (void);
void Sx_Dio2_Set (void);
void Sx_Dio2_Reset (void);
unsigned char Sx_Dio2_Get (void);


#endif    /* _SX1278_BASE_IO_H_ */

//--- end of file ---//

