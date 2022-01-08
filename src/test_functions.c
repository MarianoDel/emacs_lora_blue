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

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
extern volatile unsigned char adc_int_seq_ready;
extern volatile unsigned short adc_ch [];

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
