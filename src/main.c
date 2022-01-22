//------------------------------------------------------------------------------
// #### TEMPLATE FOR PROJECTS WITH ARDUINO BLUEPILL - Processor STM32F103C8T6
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### MAIN.C #################################################################
//------------------------------------------------------------------------------

// Includes --------------------------------------------------------------------
#include "stm32f10x.h"
#include "hard.h"
#include "test_functions.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#include <stdio.h>
#include <string.h>



// Externals -------------------------------------------------------------------
// -- Externals for timers
volatile unsigned short wait_ms_var = 0;
volatile unsigned short comms_timeout = 0;
volatile unsigned short timer_standby = 0;
volatile unsigned short timer_led = 0;


// Globals ---------------------------------------------------------------------


// Module Private Functions ----------------------------------------------------
void TimingDelay_Decrement(void);
void SysTickError (void);
extern void EXTI0_IRQHandler (void);


// Module Functions ------------------------------------------------------------
int main (void)
{
    // System Clock is already configured at this point

    // Gpio Configuration.
    GpioInit ();
    
    // Systick Timer Activation
    if (SysTick_Config(72000))
        SysTickError();

    // Hardware Tests Functions
    // TF_Led ();
    // TF_Timer1_OnePulseMode ();
    // TF_Usart1_Tx_Int ();
    // TF_Usart1_Tx_Rx_Int ();
    // TF_Usart2_Tx_Int ();
    // TF_Usart2_Tx_Rx_Int ();
    // TF_Usart3_Tx_Int ();
    // TF_Usart3_Tx_Rx_Int ();
    // TF_Spi1_Send_Single ();
    // TF_Spi_Chip_Select ();
    // TF_Spi1_Send_Single_Chip_Select ();
    // TF_Spi1_Send_Receive_Usart2 ();
    // TF_PB0 ();
    // TF_Sx_Init ();

    // transmit tests
    // TF_Sx_Fsk_Transmit ();
    // TF_Sx_Fsk_Transmit_Zero_Dev ();
    // TF_Sx_Fsk_Transmit_Modulation ();
    // TF_Sx_Fsk_Transmit_Modulation_Preamble ();
    // TF_Sx_Fsk_Transmit_Modulation2 ();
    // TF_Sx_Fsk_Transmit_Modulation_Sliced ();
    // TF_Sx_Fsk_Transmit_Packet_for_Slicer ();
    // TF_Sx_Fsk_Transmit_Packet ();

    // receive tests
    // TF_Sx_Fsk_Receive_Rssi ();
    // TF_Sx_Fsk_Receive_Preamble ();
    // TF_Sx_Fsk_Receive_Data_Sliced ();
    // TF_Sx_Fsk_Receive_Data_Sliced_OnlyData ();
    // TF_Sx_Fsk_Receive_Packet_Data ();

    // transmit lora tests
    // TF_Sx_Lora_Transmit_Packet ();
    // TF_Sx_Lora_Transmit_Packet_Crc_On ();

    // receive lora tests
    TF_Sx_Lora_Receive_Packet ();

    // Peripherals Activation
    // USART2Config();

    // TIM_1_Init();
    // TIM_3_Init();

}

//--- End of Main ---//

void TimingDelay_Decrement(void)
{
    if (wait_ms_var)
        wait_ms_var--;

    if (comms_timeout)
        comms_timeout--;
    
    if (timer_standby)
        timer_standby--;

    if (timer_led)
        timer_led--;

    TF_Timeouts ();

}

void EXTI0_IRQHandler (void)
{
    if(EXTI->PR & 0x00000001)	//Line0
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        
        EXTI->PR |= 0x00000001;
    }
}


void SysTickError (void)
{
    //Capture systick error...
    while (1)
    {
        if (LED)
            LED_OFF;
        else
            LED_ON;

        for (unsigned char i = 0; i < 255; i++)
        {
            asm ("nop \n\t"
                 "nop \n\t"
                 "nop \n\t" );
        }
    }
}

//--- end of file ---//

