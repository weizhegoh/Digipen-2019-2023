/*****************************************************************************
 @Project	: 
 @File 		: 
 @Details  	: All Ports and peripherals configuration                    
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/

#ifndef __HAL_DOT_H__
#define __HAL_DOT_H__

/*****************************************************************************
 Define
******************************************************************************/
#define PA_LCD_SSI0_SCK		2U
#define PA_LCD_SSI0_CS		3U
#define PA_BUZZER					4U
#define PA_LCD_SSI0_MOSI 	5U
#define PA_LCD_DC					6U
#define PA_LCD_RESET			7U

#define PB_LED_IR					0U
#define PB_LCD_BL					1U
#define PB_T3CCP0					2U
#define PB_T3CCP1					3U
#define PB_ADC_AIN10			4U

#define PC_STEPPER0				4U
#define PC_STEPPER1				5U
#define PC_STEPPER2				6U
#define PC_STEPPER3				7U

#define PD_OUTPUT					1U
#define PD_KEYPAD_ROW0		0U
#define PD_KEYPAD_ROW1		1U
#define PD_KEYPAD_ROW2		2U
#define PD_KEYPAD_ROW3		3U

#define PE_IR_INPUT				0U
#define PE_KEYPAD_COL0		1U
#define PE_KEYPAD_COL1		2U
#define PE_KEYPAD_COL2		3U
#define PE_ADC_AIN8				5U
#define PE_ADC_AIN9				4U

#define PF_SW2						0U
#define PF_LED_RED				1U
#define PF_LED_BLUE				2U
#define PF_LED_GREEN			3U
#define PF_SW1						4U

#define PC_4 4U
#define PC_5 5U
#define PC_6 6U

#define PERI_BASE  0x40000000 
#define GPIOA_DATA 0x400043FC 
#define GPIOB_DATA 0x400053FC 
#define GPIOC_DATA 0x400063FC 
#define GPIOD_DATA 0x400073FC 
#define GPIOE_DATA 0x400243FC 
#define GPIOF_DATA 0x400253FC 

#define BITBAND_PERI_BASE 0x42000000 
#define BITBAND_PERI(addr,bit_no)((BITBAND_PERI_BASE+(addr-PERI_BASE)*32+(bit_no*4)))

/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Macro
******************************************************************************/
#define BIT( x )					(1U<<(x))

/* Buzzer */
#define GPIOA_B4 *((volatile unsigned int *)(BITBAND_PERI(GPIOA_DATA,4))) 
#define BUZZER_ON()					( GPIOA_B4 = 1 )
#define BUZZER_OFF()				( GPIOA_B4 = 0 )
#define BUZZER_SET( x )			((x)? BUZZER_ON() : BUZZER_OFF())

/* Buttons */
#define GPIOF_B4 *((volatile unsigned int *)(BITBAND_PERI(GPIOF_DATA, 4))) 
#define  SW1()  					( GPIOF_B4 ) 	

#define GPIOF_B0 *((volatile unsigned int *)(BITBAND_PERI(GPIOF_DATA, 0))) 
#define  SW2()  					( GPIOF_B0 ) 	

#define GPIOC_B4 *((volatile unsigned int *)(BITBAND_PERI(GPIOC_DATA, 4))) 
#define PC4_ON()         (GPIOC_B4 = 1)
#define PC4_OFF()        (GPIOC_B4 = 0)
#define CS_LOW()         PC4_OFF()
#define CS_HIGH()        PC4_ON()

#define GPIOC_B5 *((volatile unsigned int *)(BITBAND_PERI(GPIOC_DATA, 5))) 
#define PC5_ON()         (GPIOC_B5 = 1)
#define PC5_OFF()        (GPIOC_B5 = 0)
#define WR_LOW()         PC5_OFF()
#define WR_HIGH()        PC5_ON()

#define GPIOC_B6 *((volatile unsigned int *)(BITBAND_PERI(GPIOC_DATA, 6))) 
#define PC6_ON()         (GPIOC_B6 = 1)
#define PC6_OFF()        (GPIOC_B6 = 0)
#define DATA_LOW()       PC6_OFF()
#define DATA_HIGH()      PC6_ON()

#define PA_UART0_RX				0U		
#define PA_UART0_TX				1U	
/******************************************************************************
 Global functions
******************************************************************************/


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void Port_Init( void );
void UART_Init(void);
void write_ASCII_UART0 (char);
char read_ASCII_UART0 (void);
#endif /* __HAL_DOT_H__ */









