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

#include "sx1278_lora.h"

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
extern volatile unsigned char adc_int_seq_ready;
extern volatile unsigned short adc_ch [];
extern volatile unsigned short timer_standby;
extern volatile unsigned short comms_timeout;


// Globals ---------------------------------------------------------------------
volatile unsigned short timer_millis = 0;
volatile unsigned short timer_secs = 0;
volatile unsigned short timer_pckt = 0;


// Module Private Types & Macros -----------------------------------------------


// Module Private Functions ----------------------------------------------------
void SendRates (unsigned int err_preamble,
                unsigned int err_sync,
                unsigned int p_good);


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

    Wait_ms(10);
    SxFskSetFreq(434000000);
    Usart2Send("Freq set\n");
    Wait_ms(10);
    SxFskSetFreqDev(0);
    Usart2Send("Freq dev to zero\n");
    Wait_ms(300);
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

    SxFskSetFreqInt(434000000);
    Usart2Send("Freq set\n");
    Wait_ms(10);
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
        Wait_ms(1999);
    }
}


char buff_tx_pckt [] = {"THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG 0123456789\nEND"};    //54 bytes?
void TF_Sx_Fsk_Transmit_Packet_for_Slicer (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk and packet mode: ");
    if (!SxFskPacketInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    SxFskSetFreqInt(434000000);
    Usart2Send("Freq set\n");
    Wait_ms(10);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(10);

    unsigned char fifo_st = 0;
    unsigned char pckt_len = sizeof(buff_tx_pckt);    // sync word to rx and (buff_tx - 1)
    SxBaseWrite(REG_PAYLOADLENGTH, pckt_len);
    Sx_Dio2_Output();
        
    while (1)
    {
        fifo_st = SxBaseRead (REG_IRQFLAGS2);
        sprintf(buff, "fifo status irq2: 0x%02x\n", fifo_st);
        Usart2Send(buff);
        
        // set the fifo
        SxBaseWrite(REG_FIFO, 0x01);    //sync address to rx
        SxBaseBurstWrite(REG_FIFO, (unsigned char *) buff_tx_pckt, pckt_len - 1);    //-1 for sync address
        LED_ON;
        timer_standby = 100;
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);

        // wait end of tx
        while (timer_standby)
        {
            fifo_st = SxBaseRead(REG_IRQFLAGS2);
            if (fifo_st & RF_IRQFLAGS2_PACKETSENT)
            {
                Usart2Send("setting tx off ");
                LED_OFF;
                SxFskSetOpMode(RF_OPMODE_STANDBY);
                timer_standby = 0;
            }
        }

        if (!(fifo_st & RF_IRQFLAGS2_PACKETSENT))
        {
            Usart2Send("timeout on tx ");
            LED_OFF;
            SxFskSetOpMode(RF_OPMODE_STANDBY);
        }
        
        Wait_ms(10);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(1990);
    }
}


void TF_Sx_Fsk_Transmit_Packet (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk and packet mode: ");
    if (!SxFskPacketInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    SxFskSetFreqInt(434000000);
    Usart2Send("Freq set\n");
    Wait_ms(10);
    opmode = SxFskGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(10);

    unsigned char fifo_st = 0;
    unsigned char pckt_len = sizeof(buff_tx) - 1;
    SxBaseWrite(REG_PAYLOADLENGTH, pckt_len);
    sprintf(buff,"setting tx in packet mode len: %d\n", pckt_len);
    Usart2Send(buff);
    
    Sx_Dio2_Output();
        
    while (1)
    {
        fifo_st = SxBaseRead (REG_IRQFLAGS2);
        sprintf(buff, "fifo status irq2: 0x%02x\n", fifo_st);
        Usart2Send(buff);
        
        // set the fifo
        SxBaseBurstWrite(REG_FIFO, (unsigned char *) buff_tx, pckt_len);
        LED_ON;
        timer_standby = 100;
        SxFskSetOpMode(RF_OPMODE_TRANSMITTER);

        // wait end of tx
        while (timer_standby)
        {
            fifo_st = SxBaseRead(REG_IRQFLAGS2);
            if (fifo_st & RF_IRQFLAGS2_PACKETSENT)
            {
                Usart2Send("setting tx off ");
                LED_OFF;
                SxFskSetOpMode(RF_OPMODE_STANDBY);
                timer_standby = 0;
            }
        }

        if (!(fifo_st & RF_IRQFLAGS2_PACKETSENT))
        {
            Usart2Send("timeout on tx ");
            LED_OFF;
            SxFskSetOpMode(RF_OPMODE_STANDBY);
        }
        
        Wait_ms(10);
        opmode = SxFskGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);
        Wait_ms(1990);
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


unsigned char report_sended = 0;
void TF_Sx_Fsk_Receive_Data_Sliced_OnlyData (void)
{
    char buff [200] = { 0 };
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

    SxFskSetFreqInt(433997500);
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
    unsigned int error_sync = 0;
    unsigned int error_preamble = 0;    
    unsigned int pckt_good = 0;    
    comms_timeout = 20000;
    
    while (1)
    {
        // preamble detection
        if ((!preamble_det) &&
            (Sx_Dio0_Get()))
        {
            // LED_ON;

            // init the slicer
            DataSlicerRxInit();

            // new preamble detected
            preamble_det = 1;
            last_dio1 = Sx_Dio1_Get();
            timer_standby = 100;
            comms_timeout = 3000;    //middle of next packet
            pckt_good++;
            report_sended = 0;
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
            error_sync++;
            if (pckt_good)
                pckt_good--;
            
            // LED_OFF;
            Usart2Send("preamble timeout\n");
            // only rssi flag
            // SxBaseWrite(REG_IRQFLAGS1, RF_IRQFLAGS1_RSSI);
            // preamble and rssi flag
            SxBaseWrite(REG_IRQFLAGS1,
                        RF_IRQFLAGS1_RSSI |
                        RF_IRQFLAGS1_PREAMBLEDETECT);
            
            preamble_det = 0;
        }

        // check continuos preamble int with LED
        if (Sx_Dio0_Get())
            LED_ON;
        else
            LED_OFF;
                

        if (!comms_timeout)
        {
            comms_timeout = 2000;    // time for next packet
            error_preamble++;
            if ((error_preamble % 20) == 0)
            {
                SendRates (error_preamble, error_sync, pckt_good);
            }
        }

        if ((!preamble_det) &&
            (comms_timeout > 800) &&
            (comms_timeout < 1200))
        {
            if (((pckt_good % 100) == 0) && (!report_sended))
            {
                report_sended = 1;
                SendRates (error_preamble, error_sync, pckt_good);
            }
        }
    }
}


void TF_Sx_Fsk_Receive_Packet_Data (void)
{
    char buff [200] = { 0 };
    char buff_rx [60] = { 0 };    
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on fsk packet mode: ");
    if (!SxFskPacketInit())
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

    SxFskSetFreqInt(433997500);
    Sx_Dio1_Input();
    Sx_Dio2_Input();    

    unsigned char pckt_len = sizeof(buff_tx) - 1;    // only payload
    unsigned int error_preamble = 0;
    unsigned int error_sync = 0;        
    unsigned int pckt_good = 0;    

    sprintf(buff,"setting rx in packet mode len: %d\n", pckt_len);
    Usart2Send(buff);
    SxBaseWrite(REG_PAYLOADLENGTH, pckt_len);
    unsigned char fifo_st = SxBaseRead (REG_IRQFLAGS2);
    sprintf(buff, "fifo status irq2: 0x%02x\n", fifo_st);
    Usart2Send(buff);

    // get rx with crc errors
    // check init

    SxFskSetOpMode(RF_OPMODE_RECEIVER);
    comms_timeout = 20000;
    
    while (1)
    {
        // packet ready
        if (Sx_Dio0_Get())
        {
            LED_ON;
            comms_timeout = 3000;

            // unsigned char rx_flags1 = SxBaseRead(REG_IRQFLAGS1);
            // unsigned char rx_flags2 = SxBaseRead(REG_IRQFLAGS2);            
            // sprintf(buff, "rx fifo irq1: 0x%02x irq2: 0x%02x\n",
            //         rx_flags1,
            //         rx_flags2);
            
            // Usart2Send(buff);
            // Wait_ms(10);

            SxBaseBurstRead(REG_FIFO, (unsigned char *) buff_rx, pckt_len);
            Usart2Send(buff_rx);
            
            // Wait_ms(10);            
            // rx_flags1 = SxBaseRead(REG_IRQFLAGS1);
            // rx_flags2 = SxBaseRead(REG_IRQFLAGS2);
            // sprintf(buff, "irq1: 0x%02x irq2: 0x%02x\n",
            //         rx_flags1,
            //         rx_flags2);
            // Usart2Send(buff);
            // Wait_ms(10);

            unsigned char rx_flags2 = SxBaseRead(REG_IRQFLAGS2);
            if (rx_flags2 & RF_IRQFLAGS2_PAYLOADREADY)
            {
                error_sync++;
                Wait_ms(100);
                Usart2Send("Payload len error\n");
            }
            else
                pckt_good++;
            
            LED_OFF;
        }

        if (!comms_timeout)
        {
            comms_timeout = 2000;    // time for next packet
            error_preamble++;
            if ((error_preamble % 20) == 0)
            {
                SendRates (error_preamble, error_sync, pckt_good);
            }
        }

        if (!timer_secs)
        {
            if ((comms_timeout > 800) &&
                (comms_timeout < 1200))
            {
                SendRates (error_preamble, error_sync, pckt_good);
                timer_secs = 60;                
            }
        }
    }
}


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
                
    sprintf(buff, "total: %d preamble err: %d sync err: %d pre: %d.%02d%% sync: %d.%02d%%\n",
            total,
            err_preamble,
            err_sync,
            ratep_int,
            ratep_dec,
            rates_int,
            rates_dec);
    Usart2Send(buff);
}


void TF_Timeouts (void)
{
    if (timer_millis)
        timer_millis--;
    else
    {
        timer_millis = 1000;

        if (timer_secs)
            timer_secs--;
    }

    if (timer_pckt)
        timer_pckt++;
}


////////////////
// Lora Tests //
////////////////
void TF_Sx_Lora_Transmit_Packet (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on lora packet mode: ");
    if (!SxLoRaInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    // SxFskSetFreqInt(434000000);
    // Usart2Send("Freq set\n");
    // Wait_ms(10);
    opmode = SxLoraGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(10);

    unsigned char fifo_st = 0;
    unsigned char pckt_len = sizeof(buff_tx) - 1;
    SxBaseWrite(REG_LR_PAYLOADLENGTH, pckt_len);
    sprintf(buff,"setting tx in packet mode len: %d\n", pckt_len);
    Usart2Send(buff);
    
    Sx_Dio2_Output();
        
    while (1)
    {
        timer_standby = 2000;
        unsigned char tx_ongoing = 1;
        fifo_st = SxBaseRead (REG_LR_IRQFLAGS);
        opmode = SxBaseRead(REG_LR_OPMODE);
        sprintf(buff, "irq: 0x%02x opmode_reg: 0x%02x\n", fifo_st, opmode);
        Usart2Send(buff);
        
        // set the fifo
        unsigned char pfifo = SxBaseRead(REG_LR_FIFOTXBASEADDR);
        SxBaseWrite(REG_LR_FIFOADDRPTR, pfifo);
        SxBaseBurstWrite(REG_LR_FIFO, (unsigned char *) buff_tx, pckt_len);
        SxBaseWrite(REG_LR_PAYLOADLENGTH, pckt_len);
        LED_ON;
        timer_pckt = 1;
        SxLoraSetOpMode(RFLR_OPMODE_TRANSMITTER);

        // wait end of tx
        while (tx_ongoing)
        {
            fifo_st = SxBaseRead(REG_LR_IRQFLAGS);
            if (fifo_st & RFLR_IRQFLAGS_TXDONE)
            {
                sprintf(buff, "tx off irq: 0x%02x time: %d\n", fifo_st, timer_pckt);
                Usart2Send(buff);
                SxBaseWrite(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
                LED_OFF;
                tx_ongoing = 0;
                timer_pckt = 0;
            }
        }
        
        Wait_ms(10);
        opmode = SxLoraGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);

        while(timer_standby);    // wait up to 2secs.
    }
}


void TF_Sx_Lora_Transmit_Packet_Crc_On (void)
{
    char buff [100] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on lora packet mode: ");
    if (!SxLoRaInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    // SxFskSetFreqInt(434000000);
    // Usart2Send("Freq set\n");
    // Wait_ms(10);
    opmode = SxLoraGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(10);

    unsigned char fifo_st = 0;
    unsigned char pckt_len = sizeof(buff_tx) - 1;
    SxBaseWrite(REG_LR_PAYLOADLENGTH, pckt_len);
    unsigned char modem_conf = SxBaseRead(REG_LR_MODEMCONFIG2);
    SxBaseWrite(REG_LR_MODEMCONFIG2, modem_conf | RFLR_MODEMCONFIG2_RXPAYLOADCRC_ON);
    sprintf(buff,"setting tx in packet mode len: %d crc: on\n", pckt_len);
    Usart2Send(buff);
    
    Sx_Dio2_Output();
        
    while (1)
    {
        timer_standby = 2000;
        unsigned char tx_ongoing = 1;
        fifo_st = SxBaseRead (REG_LR_IRQFLAGS);
        opmode = SxBaseRead(REG_LR_OPMODE);
        sprintf(buff, "irq: 0x%02x opmode_reg: 0x%02x\n", fifo_st, opmode);
        Usart2Send(buff);
        
        // set the fifo
        unsigned char pfifo = SxBaseRead(REG_LR_FIFOTXBASEADDR);
        SxBaseWrite(REG_LR_FIFOADDRPTR, pfifo);
        SxBaseBurstWrite(REG_LR_FIFO, (unsigned char *) buff_tx, pckt_len);
        SxBaseWrite(REG_LR_PAYLOADLENGTH, pckt_len);
        LED_ON;
        timer_pckt = 1;
        SxLoraSetOpMode(RFLR_OPMODE_TRANSMITTER);

        // wait end of tx
        while (tx_ongoing)
        {
            fifo_st = SxBaseRead(REG_LR_IRQFLAGS);
            if (fifo_st & RFLR_IRQFLAGS_TXDONE)
            {
                sprintf(buff, "tx off irq: 0x%02x time: %d\n", fifo_st, timer_pckt);
                Usart2Send(buff);
                SxBaseWrite(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
                LED_OFF;
                tx_ongoing = 0;
                timer_pckt = 0;
            }
        }
        
        Wait_ms(10);
        opmode = SxLoraGetOpMode();
        sprintf(buff, "opmode: %d\n", opmode);
        Usart2Send(buff);

        while(timer_standby);    // wait up to 2secs.
    }
}


void TF_Sx_Lora_Receive_Packet (void)
{
    char buff [100] = { 0 };
    char buff_rx [60] = { 0 };
    unsigned char opmode = 0;
    
    SPI1_Config();
    Usart2Config();

    Usart2Send ("init device on lora packet mode: ");
    if (!SxLoRaInit())
    {
        while (1)
        {
            Usart2Send("ERROR!\n");
            Wait_ms(1000);
        }
    }
    else
        Usart2Send("OK\n");

    // SxFskSetFreqInt(434000000);
    // Usart2Send("Freq set\n");
    // Wait_ms(10);
    opmode = SxLoraGetOpMode();
    sprintf(buff, "opmode: %d\n", opmode);
    Usart2Send(buff);
    Wait_ms(10);

    unsigned char fifo_st = 0;
    unsigned char pckt_len = 0;
    sprintf(buff,"setting rx in lora packet mode\n");
    Usart2Send(buff);
    
    Sx_Dio1_Input();
    Sx_Dio2_Input();    

    unsigned int error_preamble = 0;
    unsigned int error_sync = 0;        
    unsigned int pckt_good = 0;    


    // get rx with crc errors
    // check init

    SxLoraSetOpMode(RFLR_OPMODE_RECEIVER);
    comms_timeout = 20000;
    
    while (1)
    {
        // packet ready
        if (Sx_Dio0_Get())
        {
            LED_ON;
            comms_timeout = 3000;
            // get interrupts
            fifo_st = SxBaseRead(REG_LR_IRQFLAGS);
            // get packet len
            pckt_len = SxBaseRead(REG_LR_NBRXBYTES);
            // get packet addr
            unsigned char addr = SxBaseRead(REG_LR_FIFORXCURRENTADDR);

            if (fifo_st & (RFLR_IRQFLAGS_RXTIMEOUT |
                           RFLR_IRQFLAGS_PAYLOADCRCERROR))
            {
                error_sync++;
                SxBaseWrite(REG_LR_IRQFLAGS,
                            RFLR_IRQFLAGS_RXTIMEOUT |
                            RFLR_IRQFLAGS_PAYLOADCRCERROR);

                short rssi = SxLoraGetPcktRssi();
                short snr = SxLoraGetPcktSnr();
                sprintf(buff,"irq: 0x%02x rssi: %d snr: %d\n",
                        fifo_st,
                        rssi,
                        snr);
                Usart2Send(buff);
            }
            else
            {
                // sprintf(buff, "rx irq: 0x%02x len: %d addr: 0x%02x\n",
                //         fifo_st,
                //         pckt_len,
                //         addr);
                // Usart2Send(buff);
                // Wait_ms(20);

                // set spi pointer for read
                SxBaseWrite(REG_LR_FIFOADDRPTR, addr);
                SxBaseBurstRead(REG_LR_FIFO, (unsigned char *) buff_rx, pckt_len);
                Usart2Send(buff_rx);
                pckt_good++;
            }

            // reset the flag
            SxBaseWrite(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_RXDONE);
            
            // reset the pointer
            SxBaseWrite(REG_LR_FIFOADDRPTR, 0x00);

            LED_OFF;

        }

        if (!comms_timeout)
        {
            comms_timeout = 2000;    // time for next packet
            error_preamble++;
            if ((error_preamble % 20) == 0)
            {
                SendRates (error_preamble, error_sync, pckt_good);
            }
        }

        if (!timer_secs)
        {
            if ((comms_timeout > 800) &&
                (comms_timeout < 1200))
            {
                SendRates (error_preamble, error_sync, pckt_good);
                timer_secs = 60;                
            }
        }
    }
}

//--- end of file ---//
