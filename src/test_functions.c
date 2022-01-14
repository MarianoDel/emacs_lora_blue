//------------------------------------------------
// ## Internal Test Functions Module
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ##
// #### TEST_FUNCTIONS.C #########################
//------------------------------------------------

// Includes --------------------------------------------------------------------
#include "test_functions.h"
#include "stm32f10x.h"
#include "hard.h"
#include "tim.h"
#include "usart.h"
// #include "adc.h"
// #include "dma.h"
#include "tim.h"
#include "spi.h"
// #include "flash_program.h"

#include "sx1278_base.h"
#include "sx1278_base_io.h"
#include "sx1278_fsk.h"
#include "data_slicer.h"

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
extern volatile unsigned char adc_int_seq_ready;
extern volatile unsigned short adc_ch [];
extern volatile unsigned short timer_standby;
extern volatile unsigned short comms_timeout;


// Globals ---------------------------------------------------------------------


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------


// Module Functions ------------------------------------------------------------
void TF_Led (void)
{
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        Wait_ms(300);
    }
}


void TF_Timer1_OnePulseMode (void)
{
    // Activate Pulses on Timer1
    TIM_1_Init();
    Update_TIM1_CH1(DUTY_50_PERCENT);
    while (1)
    {
        Wait_ms(1);
        ENABLE_TIM1;
    }
}
    

void TF_Usart1_Tx_Int (void)
{
    // start usart1 with ints
    Usart1Config();

    while (1)
    {
        Wait_ms(1800);
        LED_ON;
        Usart1Send("Prueba Usart1\n");
        Wait_ms(200);
        LED_OFF;
    }
}


void TF_Usart1_Tx_Rx_Int (void)
{
    // start usart1 and loop rx -> tx after 3 secs
    Usart1Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    while(1)
    {
        Wait_ms(3000);
        if (Usart1HaveData())
        {
            Usart1HaveDataReset();
            readed = Usart1ReadBuffer((unsigned char *)buff_local, 127);
            *(buff_local + readed) = '\n';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart1Send(buff_local);
        }
    }    
}


void TF_Usart2_Tx_Int (void)
{
    // start usart2 with ints
    Usart2Config();

    while (1)
    {
        Wait_ms(1800);
        LED_ON;
        Usart2Send("Prueba Usart2\n");
        Wait_ms(200);
        LED_OFF;
    }
}


void TF_Usart2_Tx_Rx_Int (void)
{
    // start usart2 and loop rx -> tx after 3 secs
    Usart2Config();
    Usart2Send("\nLoop test on Usart2\n");
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    while(1)
    {
        Wait_ms(3000);
        if (Usart2HaveData())
        {
            Usart2HaveDataReset();
            readed = Usart2ReadBuffer((unsigned char *)buff_local, 127);
            *(buff_local + readed) = '\n';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart2Send(buff_local);
        }
    }    
}


void TF_Usart3_Tx_Int (void)
{
    // start usart1 with ints
    Usart3Config();

    while (1)
    {
        Wait_ms(1800);
        LED_ON;
        Usart3Send("Prueba Usart3\n");
        Wait_ms(200);
        LED_OFF;
    }
}


void TF_Usart3_Tx_Rx_Int (void)
{
    // start usart3 and loop rx -> tx after 3 secs
    Usart3Config();
    char buff_local [128] = { 0 };
    unsigned char readed = 0;

    while(1)
    {
        Wait_ms(3000);
        if (Usart3HaveData())
        {
            Usart3HaveDataReset();
            readed = Usart3ReadBuffer((unsigned char *)buff_local, 127);
            *(buff_local + readed) = '\n';    //cambio el '\0' por '\n' antes de enviar
            *(buff_local + readed + 1) = '\0';    //ajusto el '\0'
            Usart3Send(buff_local);
        }
    }    
}


void TF_PB0 (void)
{
    while (1)
    {
        LED_ON;
        PB0_ON;
        Wait_ms(100);
        LED_OFF;
        PB0_OFF;
        Wait_ms(1000);        
    }
}


void TF_Spi_Chip_Select (void)
{
    while (1)
    {
        LED_ON;
        SPI_CS_ON;
        Wait_ms(100);
        LED_OFF;
        SPI_CS_OFF;
        Wait_ms(1000);        
    }
}


void TF_Spi1_Send_Single (void)
{
    SPI_CS_OFF;
    SPI1_Config();
    
    while(1)
    {
        Wait_ms(10);
        SPI1_Send_Single(0x55);
    }    
}


void TF_Spi1_Send_Receive_Usart2 (void)
{
    SPI1_Config();
    Usart2Config();

    unsigned char getted = 0;
    char buff [100] = { 0 };
    while (1)
    {
        Wait_ms(2000);
        LED_ON;

        SPI_CS_ON;
        SPI1_Send_Receive(0x42);
        getted = SPI1_Send_Receive(0x00);
        SPI_CS_OFF;
        sprintf(buff, "reg42: %d\n", getted);
        Usart2Send(buff);
        
        SPI_CS_ON;
        SPI1_Send_Receive(0x4b);
        getted = SPI1_Send_Receive(0x00);
        SPI1_Wait_Clk256();
        SPI_CS_OFF;        
        sprintf(buff, "reg4b: %d\n", getted);
        Usart2Send(buff);


        LED_OFF;
    }
}


void TF_Spi1_Send_Single_Chip_Select (void)
{
    SPI1_Config();
    
    while(1)
    {
        SPI_CS_ON;
        SPI1_Send_Single(0x55);
        SPI_CS_OFF;
    }    
}


void TF_Sx_Init (void)
{
    SPI1_Config();
    Usart2Config();

    // unsigned char getted = 0;
    // char buff [100] = { 0 };

    Usart2Send ("\nreset to Sx module\n");
    SxBaseRst();
    Usart2Send ("check device id: ");
    if (SxBaseCheckDeviceID())
        Usart2Send("OK\n");
    else
        Usart2Send("ERROR!\n");

    while (1);
    
}


void TF_Sx_Fsk_Transmit (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    Sx_Dio2_Output();
    
    while (1)
    {
        Usart2Send("setting tx on! ");
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        Wait_ms(10);
        Sx_Dio2_Set();
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(500);
        Sx_Dio2_Reset();
        Wait_ms(500);

        Usart2Send("setting tx off ");
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(3000);
    }
}


void TF_Sx_Fsk_Transmit_Zero_Dev (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(100);
    SxFskSetFreqDev(0);
    Usart2Send("Freq dev to zero\n");
    Wait_ms(200);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);

    while (1)
    {
        Usart2Send("setting tx on! ");
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        Wait_ms(10);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(1000);

        Usart2Send("setting tx off ");
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(3000);
    }
}


void TF_Sx_Fsk_Transmit_Modulation (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    Sx_Dio2_Output();
    unsigned char last_dio1 = 0;
    
    while (1)
    {
        Usart2Send("setting tx on mod on! ");
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        timer_standby = 1000;
        while (timer_standby)
        {            
            // change data on DCLK (DIO1) falling edge
            if ((last_dio1) &&
                (!Sx_Dio1_Get()))
            {
                // falling edge
                if (LED)
                {
                    LED_OFF;
                    Sx_Dio2_Reset();
                }
                else
                {
                    LED_ON;
                    Sx_Dio2_Set();                    
                }

                last_dio1 = 0;
            }               

            if ((!last_dio1) &&
                (Sx_Dio1_Get()))
            {
                // rising edge
                last_dio1 = 1;
            }
        }

        Usart2Send("setting tx off ");
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(3000);
    }
}


void TF_Sx_Fsk_Transmit_Modulation_Preamble (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    Sx_Dio2_Output();
    unsigned char last_dio1 = 0;
    unsigned char preamble_cnt = 0;
    unsigned char data_cnt = 0;    
    
    while (1)
    {
        Usart2Send("setting tx on mod on! ");
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        timer_standby = 1000;
        while (timer_standby)
        {
            if (preamble_cnt < 16)
            {
                // change data on DCLK (DIO1) falling edge
                if ((last_dio1) &&
                    (!Sx_Dio1_Get()))
                {
                    // falling edge
                    if (LED)
                    {
                        LED_OFF;
                        Sx_Dio2_Reset();
                    }
                    else
                    {
                        LED_ON;
                        Sx_Dio2_Set();                    
                    }

                    last_dio1 = 0;
                    preamble_cnt++;
                }               

                if ((!last_dio1) &&
                    (Sx_Dio1_Get()))
                {
                    // rising edge
                    last_dio1 = 1;
                }
            }
            else
            {
                // send 0x01
                // change data on DCLK (DIO1) falling edge
                if ((last_dio1) &&
                    (!Sx_Dio1_Get()))
                {
                    // falling edge
                    if (data_cnt < 7)
                    {
                        Sx_Dio2_Reset();
                        data_cnt++;
                    }
                    else
                    {
                        Sx_Dio2_Set();
                        data_cnt = 0;
                    }
                    last_dio1 = 0;
                }               

                if ((!last_dio1) &&
                    (Sx_Dio1_Get()))
                {
                    // rising edge
                    last_dio1 = 1;
                }
            }
        }

        Usart2Send("setting tx off ");
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        preamble_cnt = 0;
        data_cnt = 0;
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(3000);
    }
}


char buff_tx [] = {"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789\n"};    //54 bytes?
void TF_Sx_Fsk_Transmit_Modulation_Sliced (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    Sx_Dio2_Output();
    unsigned char last_dio1 = 0;
    unsigned char tx_status = 0;
    
    while (1)
    {
        DataSlicerTxInit();
        Usart2Send("setting tx on mod quick ");
        LED_ON;
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        tx_status = 1;

        while (tx_status)
        {
            // change data on DCLK (DIO1) falling edge
            if ((last_dio1) &&
                (!Sx_Dio1_Get()))
            {
                unsigned char bit = 0;
                slicer_answer_e answer = DataSlicerTx (&bit, (unsigned char *) buff_tx, sizeof(buff_tx) - 1);

                if (bit)
                    Sx_Dio2_Set();                    
                else
                    Sx_Dio2_Reset();

                last_dio1 = 0;

                if (answer == SLICER_END_PCKT)
                {
                    //power off the tx
                    Wait_ms(1);
                    tx_status = 0;
                }
            }
            else if ((!last_dio1) &&
                     (Sx_Dio1_Get()))
            {
                // rising edge
                last_dio1 = 1;
            }
        }

        Usart2Send("setting tx off ");
        LED_OFF;
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(2000);
    }
}


void TF_Sx_Fsk_Transmit_Modulation2 (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);

    Sx_Dio2_Output();
    unsigned char last_dio1 = 0;
    unsigned char mod_index = 1;
    
    while (1)
    {
        if (mod_index == 1)
        {
            Usart2Send("setting tx on mod 1.0\n");
            SxFskSetModuletionIndex(4800, 1.0);
            mod_index = 2;
        }
        else
        {
            Usart2Send("setting tx on mod 2.0\n");            
            SxFskSetModuletionIndex(4800, 2.0);
            mod_index = 1;
        }
        
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);
        timer_standby = 1000;
        while (timer_standby)
        {
            // change data on DCLK (DIO1) falling edge
            if ((last_dio1) &&
                (!Sx_Dio1_Get()))
            {
                // falling edge
                if (LED)
                {
                    LED_OFF;
                    Sx_Dio2_Reset();
                }
                else
                {
                    LED_ON;
                    Sx_Dio2_Set();                    
                }

                last_dio1 = 0;
            }               

            if ((!last_dio1) &&
                (Sx_Dio1_Get()))
            {
                // rising edge
                last_dio1 = 1;
            }
        }

        Usart2Send("setting tx off ");
        SxFskSetOpMode(RF_OPMODE_STANDBY);
        Wait_ms(1);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(3000);
    }
}


void TF_Sx_Fsk_Receive_Rssi (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    
    Sx_Dio1_Input();
    Sx_Dio2_Input();    
    SxBaseWrite(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01);
    
    Usart2Send("setting rx continuous...");
    SxFskSetOpMode(RF_OPMODE_RECEIVER);

    unsigned char preamble_det = 0;
    short rssi_value = 0;
    while (1)
    {
        // rssi detection
        if ((!preamble_det) &&
            (Sx_Dio0_Get()))
        {
            // new rssi detected
            Usart2Send("\nnew rssi threshold: ");
            // ask rssi value
            rssi_value = SxFskGetRssiValue();
            sprintf(buff, "%d dBm\n", rssi_value);
            Usart2Send(buff);
            Wait_ms(20);
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            // SxBaseWrite(REG_IRQFLAGS1,
            //             RF_IRQFLAGS1_RSSI |
            //             RF_IRQFLAGS1_PREAMBLEDETECT);
            preamble_det = 1;
            timer_standby = 1000;
        }

        // end of preamble
        if ((preamble_det) &&
            (!Sx_Dio0_Get()))
        {
            Usart2Send("finish\n");
            preamble_det = 0;
        }

        // end of preamble by timeout
        if ((preamble_det) &&
            (Sx_Dio0_Get()) &&
            (!timer_standby))
        {
            Usart2Send("finish timeout\n");
            // only rssi flag
            SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            // SxBaseWrite(REG_IRQFLAGS1,
            //             RF_IRQFLAGS1_RSSI |
            //             RF_IRQFLAGS1_PREAMBLEDETECT);
            
            preamble_det = 0;
        }
    }
}


void TF_Sx_Fsk_Receive_Preamble (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    
    Sx_Dio1_Input();
    Sx_Dio2_Input();    
    SxBaseWrite(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01);    // map to RSSI/preamble
    SxBaseWrite(REG_DIOMAPPING2, RF_DIOMAPPING2_MAP_PREAMBLEDETECT);    // map preamble to DIO
    
    Usart2Send("setting rx continuous...");
    SxFskSetOpMode(RF_OPMODE_RECEIVER);

    unsigned char preamble_det = 0;
    short rssi_value = 0;
    unsigned char irq_value = 0;
    
    while (1)
    {
        // preamble detection
        if ((!preamble_det) &&
            (Sx_Dio0_Get()))
        {
            // new preamble detected
            Usart2Send("\nnew preamble rssi: ");
            // ask rssi value
            rssi_value = SxFskGetRssiValue();
            irq_value = SxBaseRead(REG_IRQFLAGS1);
            sprintf(buff, "%d dBm irq: 0x%02x\n", rssi_value, irq_value);
            Usart2Send(buff);
            Wait_ms(20);
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            // SxBaseWrite(REG_IRQFLAGS1,
            //             RF_IRQFLAGS1_RSSI |
            //             RF_IRQFLAGS1_PREAMBLEDETECT);
            preamble_det = 1;
            timer_standby = 1000;
        }

        // end of preamble
        if ((preamble_det) &&
            (!Sx_Dio0_Get()))
        {
            Usart2Send("preamble finish\n");
            preamble_det = 0;
        }

        // end of preamble by timeout
        if ((preamble_det) &&
            (Sx_Dio0_Get()) &&
            (!timer_standby))
        {
            Usart2Send("preamble timeout\n");
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            SxBaseWrite(REG_IRQFLAGS1,
                        RF_IRQFLAGS1_RSSI |
                        RF_IRQFLAGS1_PREAMBLEDETECT);
            
            preamble_det = 0;
        }
    }
}


void TF_Sx_Fsk_Receive_Data_Sliced (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);
    
    Sx_Dio1_Input();
    Sx_Dio2_Input();    
    SxBaseWrite(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01);    // map to RSSI/preamble
    SxBaseWrite(REG_DIOMAPPING2, RF_DIOMAPPING2_MAP_PREAMBLEDETECT);    // map preamble to DIO
    
    Usart2Send("setting rx continuous...");
    SxFskSetOpMode(RF_OPMODE_RECEIVER);

    unsigned char preamble_det = 0;
    short rssi_value = 0;
    unsigned char irq_value = 0;
    unsigned char last_dio1 = 0;
    unsigned char data_rx_buff [3] = { 0 };
    slicer_answer_e sl_answer = SLICER_WORKING;

    
    while (1)
    {
        // preamble detection
        if ((!preamble_det) &&
            (Sx_Dio0_Get()))
        {
            LED_ON;
            // init the slicer
            DataSlicerRxInit();

            // new preamble detected
            Usart2Send("\nnew preamble rssi: ");
            // ask rssi value
            rssi_value = SxFskGetRssiValue();
            irq_value = SxBaseRead(REG_IRQFLAGS1);
            sprintf(buff, "%d dBm irq: 0x%02x\n", rssi_value, irq_value);
            Usart2Send(buff);
            // Wait_ms(20);
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            // SxBaseWrite(REG_IRQFLAGS1,
            //             RF_IRQFLAGS1_RSSI |
            //             RF_IRQFLAGS1_PREAMBLEDETECT);
            preamble_det = 1;
            last_dio1 = Sx_Dio1_Get();
            timer_standby = 100;
        }

        // end of preamble send data to slicer
        if (preamble_det)
        {
            // read data on DCLK (DIO1) rising edge
            if ((!last_dio1) &&
                (Sx_Dio1_Get()))
            {
                // rising edge
                sl_answer = DataSlicerRx (Sx_Dio2_Get(), data_rx_buff);

                if (sl_answer == SLICER_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x ", data_rx_buff[0]);
                    // for ascii
                    sprintf(buff, "%c", data_rx_buff[0]);
                    Usart2Send(buff);
                }

                if (sl_answer == SLICER_DOUBLE_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x 0x%02x ",
                    //         data_rx_buff[0],
                    //         data_rx_buff[1]);
                    // for ascii
                    sprintf(buff, "%c%c",
                            data_rx_buff[0],
                            data_rx_buff[1]);
                    
                    Usart2Send(buff);
                }

                if (sl_answer == SLICER_TRIPLE_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x 0x%02x 0x%02x ",
                    //         data_rx_buff[0],
                    //         data_rx_buff[1],                            
                    //         data_rx_buff[2]);
                    // for ascii
                    sprintf(buff, "%c%c%c",
                            data_rx_buff[0],
                            data_rx_buff[1],                            
                            data_rx_buff[2]);
                    Usart2Send(buff);
                }
                
                if (sl_answer == SLICER_END_PCKT)
                {
                    LED_OFF;
                    Usart2Send("\nend of packet\n");
                    SxBaseWrite(REG_IRQFLAGS1,
                                RF_IRQFLAGS1_RSSI |
                                RF_IRQFLAGS1_PREAMBLEDETECT);
            
                    preamble_det = 0;
                }

                last_dio1 = 1;
            }               

            if ((last_dio1) &&
                (!Sx_Dio1_Get()))
            {
                // falling edge
                last_dio1 = 0;
            }
            
        }

        // end of preamble by timeout
        if ((preamble_det) &&
            (Sx_Dio0_Get()) &&
            (!timer_standby))
        {
            LED_OFF;
            Usart2Send("preamble timeout\n");
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            SxBaseWrite(REG_IRQFLAGS1,
                        RF_IRQFLAGS1_RSSI |
                        RF_IRQFLAGS1_PREAMBLEDETECT);
            
            preamble_det = 0;
        }
    }
}


void TF_Sx_Fsk_Receive_Data_Sliced_OnlyData (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk mode: ");
    if (!SxFskInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    Wait_ms(300);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(300);

    SxFskSetFreq(434000600);
    Sx_Dio1_Input();
    Sx_Dio2_Input();    
    SxBaseWrite(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01);    // map to RSSI/preamble
    SxBaseWrite(REG_DIOMAPPING2, RF_DIOMAPPING2_MAP_PREAMBLEDETECT);    // map preamble to DIO
    
    Usart2Send("setting rx continuous...\n");
    SxFskSetOpMode(RF_OPMODE_RECEIVER);

    unsigned char preamble_det = 0;
    unsigned char last_dio1 = 0;
    unsigned char data_rx_buff [3] = { 0 };
    slicer_answer_e sl_answer = SLICER_WORKING;
    unsigned int pckt_error = 0;
    unsigned int pckt_good = 0;    
    comms_timeout = 20000;
    
    while (1)
    {
        // preamble detection
        if ((!preamble_det) &&
            (Sx_Dio0_Get()))
        {
            LED_ON;

            // init the slicer
            DataSlicerRxInit();

            // new preamble detected
            preamble_det = 1;
            last_dio1 = Sx_Dio1_Get();
            timer_standby = 100;
            comms_timeout = 2000;
            pckt_good++;
        }

        // end of preamble send data to slicer
        if (preamble_det)
        {
            // read data on DCLK (DIO1) rising edge
            if ((!last_dio1) &&
                (Sx_Dio1_Get()))
            {
                // rising edge
                sl_answer = DataSlicerRx (Sx_Dio2_Get(), data_rx_buff);

                if (sl_answer == SLICER_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x ", data_rx_buff[0]);
                    // for ascii
                    sprintf(buff, "%c", data_rx_buff[0]);
                    Usart2Send(buff);
                }

                if (sl_answer == SLICER_DOUBLE_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x 0x%02x ",
                    //         data_rx_buff[0],
                    //         data_rx_buff[1]);
                    // for ascii
                    sprintf(buff, "%c%c",
                            data_rx_buff[0],
                            data_rx_buff[1]);
                    
                    Usart2Send(buff);
                }

                if (sl_answer == SLICER_TRIPLE_BYTE)
                {
                    // for hexa
                    // sprintf(buff, "0x%02x 0x%02x 0x%02x ",
                    //         data_rx_buff[0],
                    //         data_rx_buff[1],                            
                    //         data_rx_buff[2]);
                    // for ascii
                    sprintf(buff, "%c%c%c",
                            data_rx_buff[0],
                            data_rx_buff[1],                            
                            data_rx_buff[2]);
                    Usart2Send(buff);
                }
                
                if (sl_answer == SLICER_END_PCKT)
                {
                    LED_OFF;
                    SxBaseWrite(REG_IRQFLAGS1,
                                RF_IRQFLAGS1_RSSI |
                                RF_IRQFLAGS1_PREAMBLEDETECT);
            
                    preamble_det = 0;
                }

                last_dio1 = 1;
            }               

            if ((last_dio1) &&
                (!Sx_Dio1_Get()))
            {
                // falling edge
                last_dio1 = 0;
            }
            
        }

        // end of preamble by timeout
        if ((preamble_det) &&
            (Sx_Dio0_Get()) &&
            (!timer_standby))
        {
            pckt_error++;
            LED_OFF;
            Usart2Send("preamble timeout\n");
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            SxBaseWrite(REG_IRQFLAGS1,
                        RF_IRQFLAGS1_RSSI |
                        RF_IRQFLAGS1_PREAMBLEDETECT);
            
            preamble_det = 0;
        }

        if (!comms_timeout)
        {
            comms_timeout = 2000;
            pckt_error++;
            if ((pckt_error % 20) == 0)
            {
                unsigned int total = pckt_error + pckt_good;
                unsigned int rate_int = pckt_error * 100 / total;
                unsigned int rate_dec = (pckt_error * 10000 / total) - rate_int * 100;
                
                sprintf(buff, "pck errors: %d total: %d rate: %d.%d%%\n",
                        pckt_error,
                        pckt_error + pckt_good,
                        rate_int,
                        rate_dec);
                Usart2Send(buff);
            }
        }
    }
}

// void TF_Usart2_Adc_Dma (void)
// {
//     for (unsigned char i = 0; i < 5; i++)
//     {
//         LED_ON;
//         Wait_ms(250);
//         LED_OFF;
//         Wait_ms(250);
//     }
    
//     Usart2Config();

//     //-- ADC Init
//     AdcConfig();

//     //-- DMA configuration and Init
//     DMAConfig();
//     DMA1_Channel1->CCR |= DMA_CCR_EN;

//     ADC1->CR |= ADC_CR_ADSTART;

//     unsigned short cntr = 0;
//     char s_to_send [100] = { 0 };
//     Usart2Send("\nTesting ADC with dma transfers...\n");

//     while (1)
//     {
//         if (sequence_ready)
//         {
//             sequence_ready_reset;
//             if (cntr < 10000)
//                 cntr++;
//             else
//             {
//                 sprintf(s_to_send, "V_Sense_4V: %d V_Sense_12V: %d\n",
//                         V_Sense_4V,
//                         V_Sense_12V);
                
//                 Usart2Send(s_to_send);
//                 cntr = 0;
//             }
//         }            
//     }
// }


//--- end of file ---//
