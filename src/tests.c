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
void SendRates (unsigned int err_preamble,
                unsigned int err_sync,
                unsigned int p_good);


// Module Auxiliary Functions --------------------------------------------------
void Test_Functions (void);
void Usart2Send (char * pb);


// Module Functions ------------------------------------------------------------
int main (int argc, char *argv[])
{
    Test_Functions ();

    return 0;
}


// Functions for tests ---------------------------------------------------------
void Test_Functions (void)
{
    // test 1%
    unsigned int packet_good = 99;
    unsigned int packet_err_preamble = 1;
    unsigned int packet_err_sync = 0;

    SendRates(packet_err_preamble, packet_err_sync, packet_good);

    // test 0.1%
    SendRates(1, 0, 999);

    // test 0.01%
    SendRates(1, 0, 9999);

    // test 0.001%
    SendRates(1, 0, 99999);
}


// Functions to test -----------------------------------------------------------
void SendRates (unsigned int err_preamble,
                unsigned int err_sync,
                unsigned int p_good)
{
    char buff [200] = { 0 };
    unsigned int total = err_preamble + err_sync + p_good;
    unsigned int ratep_int = err_preamble * 100 / total;
    unsigned int ratep_dec = (err_preamble * 10000 / total) - ratep_int * 100;

    unsigned int rates_int = err_sync * 100 / total;
    unsigned int rates_dec = (err_sync * 10000 / total) - rates_int * 100;
                
    sprintf(buff, "total: %d preamble err: %d sync err: %d pre: %d.%02d%% sync: %d.%d%%\n",
            total,
            err_preamble,
            err_sync,
            ratep_int,
            ratep_dec,
            rates_int,
            rates_dec);
    Usart2Send(buff);
 
}


// Mocked Functions ------------------------------------------------------------
void Usart2Send (char * pb)
{
    printf("%s", pb);
}
//--- end of file ---//


