//---------------------------------------------
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    TEST PLATFORM FOR FIRMWARE
// ##
// #### TESTS.C ###############################
//---------------------------------------------

// Includes Modules for tests --------------------------------------------------

#include <stdio.h>
#include <string.h>


// Types Constants and Macros --------------------------------------------------


// Externals -------------------------------------------------------------------


// Globals ---------------------------------------------------------------------



// Module Functions to Test ----------------------------------------------------


// Module Auxiliary Functions --------------------------------------------------
void Test_Functions (void);

// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    Test_Functions ();

    return 0;
}


void Test_Functions (void)
{
    char buff [100] = { 0 };
    unsigned int pckt_error = 20;
    unsigned int pckt_good = 200000;

    float rate = (float) pckt_error * 100.0;
    rate = rate / ((float) (pckt_error + pckt_good));
    sprintf(buff, "pck errors: %d total: %d rate: %3.2f%%\n",
            pckt_error,
            pckt_error + pckt_good,
            rate);
    printf("%s", buff);
}



//--- end of file ---//


