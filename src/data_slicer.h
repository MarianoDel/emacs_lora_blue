//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### DATA_SLICER.H ############################
//------------------------------------------------

// Prevent recursive inclusion -------------------------------------------------
#ifndef _DATA_SLICER_H_
#define _DATA_SLICER_H_


// Module Exported Types Constants and Macros ----------------------------------
typedef enum {
    SLICER_WORKING,
    SLICER_BYTE,
    SLICER_DOUBLE_BYTE,
    SLICER_TRIPLE_BYTE,
    SLICER_END_PCKT

} slicer_answer_e;


// Module Exported Functions ---------------------------------------------------
void DataSlicerRxInit (void);
slicer_answer_e DataSlicerRx (unsigned char bit, unsigned char * data);

void DataSlicerTxInit (void);
slicer_answer_e DataSlicerTx (unsigned char * bit, unsigned char * data, unsigned char len);


#endif    /* _DATA_SLICER_H_ */

//--- end of file ---//

