//---------------------------------------------
// ##
// ## @Author: Med
// ## @Editor: Emacs - ggtags
// ## @TAGS:   Global
// ## @CPU:    STM32F103
// ##
// #### HARD.H #################################
//---------------------------------------------

#ifndef _HARD_H_
#define _HARD_H_


// Defines For Configuration ---------------------------------------------------

//----- Board Configuration -------------------//
//--- Hardware ------------------//
#define HARDWARE_VERSION_1_0        //placa Arduino Blue Pill

//--- Software ------------------//
// #define SOFTWARE_VERSION_1_2		
// #define SOFTWARE_VERSION_1_1	     //habla contra pc o rpi con nuevo protocolo camilla
#define SOFTWARE_VERSION_1_0        //habla contra rpi con programa magneto y traduce a micros potencia

//-------- Type of Program (depending on software version) ----------------

//-------- Type of Program and Features ----------------

//-------- Kind of Reports Sended ----------------

//-------- Others Configurations depending on the formers ------------

//-------- Hysteresis Conf ------------------------

//-------- PWM Conf ------------------------

//-------- Oscillator and Crystal selection (Freq in startup_clocksh) ---
#define HSI_INTERNAL_RC
// #define HSE_CRYSTAL_OSC


//-------- End Of Defines For Configuration ------




//--- Hardware & Software Messages ------------------//
#ifdef HARDWARE_VERSION_1_0
#define HARD "Hardware Version: 1.0\r\n"
#endif
#ifdef HARDWARE_VERSION_2_1
#define HARD "Hardware Version: 2.1\r\n"
#endif
#ifdef SOFTWARE_VERSION_2_2
#define SOFT "Software Version: 2.2\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_0
#define SOFT "Software Version: 1.0\r\n"
#endif
#ifdef SOFTWARE_VERSION_1_1
#define SOFT "Software Version: 1.1\r\n"
#endif
//--- Type of Program Announcement ----------------

//--- End of Hardware & Software Messages ------------------//



// Module Exported Constants ---------------------------------------------------
typedef enum {
	resp_ok = 0,
	resp_not_own,
	resp_error

} resp_t;


// Gpios Configuration ---------------------------------------------------------
#ifdef HARDWARE_VERSION_1_0

//--- Port A ---//
//PA4 SPI_CS
#define SPI_CS    ((GPIOA->ODR & 0x0010) == 0)
#define SPI_CS_OFF    (GPIOA->BSRR = 0x00000010)
#define SPI_CS_ON    (GPIOA->BSRR = 0x00100000)

//PA8 Alternative TIM1_CH1
#define SW ((GPIOA->ODR & 0x0100) != 0)
#define SW_ON (GPIOA->BSRR = 0x00000100)
#define SW_OFF (GPIOA->BSRR = 0x01000000)


//PA9, PA10 Alternative Usart 1 Tx Rx


//--- Port B ---//
//PB8 
#define P0_14 ((GPIOB->ODR & 0x0100) != 0)
#define P0_14_ON (GPIOB->BSRR = 0x00000100)
#define P0_14_OFF (GPIOB->BSRR = 0x01000000)

//PB9
#define RESET ((GPIOB->ODR & 0x0200) != 0)
#define RESET_ON (GPIOB->BSRR = 0x00000200)
#define RESET_OFF (GPIOB->BSRR = 0x02000000)

//PB10, PB11 Alternative Usart 3 Tx Rx


//--- Port C ---//
//PC13
#define LED    ((GPIOC->ODR & 0x2000) == 0)
#define LED_OFF    (GPIOC->BSRR = 0x00002000)
#define LED_ON    (GPIOC->BSRR = 0x20000000)


#endif //HARDWARE_VERSION_1_0


// Module Exported Functions ---------------------------------------------------


#endif

//--- end of file ---//
