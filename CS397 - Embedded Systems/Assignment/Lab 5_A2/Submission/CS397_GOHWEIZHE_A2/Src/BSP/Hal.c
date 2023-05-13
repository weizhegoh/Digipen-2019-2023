/*****************************************************************************
 @Project	: 
 @File 		: Hal.c
 @Details  	: All Ports and peripherals configuration                    
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/
#include <Common.h>
#include "Hal.h"
/*****************************************************************************
 Define
******************************************************************************/


/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/


/*****************************************************************************
 Local Variables
******************************************************************************/


/*****************************************************************************
 Implementation
******************************************************************************/
void Port_Init( void )
{
	/* enable GPIO port and clock		*/
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R0; 
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R2; 
	SYSCTL->RCGCGPIO |= SYSCTL_RCGCGPIO_R5; 

	/* Wait for GPIO ports to be ready */
	while( 0 == (SYSCTL->PRGPIO & SYSCTL_PRGPIO_R0)){};	
	while( 0 == (SYSCTL->PRGPIO & SYSCTL_PRGPIO_R2)){};	
	while( 0 == (SYSCTL->PRGPIO & SYSCTL_PRGPIO_R5)){};	
	
	// UART0 Clock
	SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R0;
	while( 0 == (SYSCTL->PRUART & SYSCTL_PRUART_R0) ); 

	/* Enable Buzzer pin as output */
	GPIOA->DIR |= BIT(PA_BUZZER);
	GPIOA->DEN |= BIT(PA_BUZZER);	
		
	/* Unlock GPIO PF[0] */
	GPIOF->LOCK = GPIO_LOCK_KEY;	/* unlock Port F (pg 684)		*/
	GPIOF->CR |= BIT(PF_SW2);
		
	/* Initialize button SW1 and SW2 with Interrupts  */
	GPIOF->PUR |= BIT(PF_SW1) | BIT(PF_SW2);
	GPIOF->DEN |= BIT(PF_SW1) | BIT(PF_SW2);
	GPIOF->DIR &= ~( BIT(PF_SW1) | BIT(PF_SW2) );
	GPIOF->AFSEL &= ~(BIT(PF_SW1) | BIT(PF_SW2));
		
	GPIOF->IM &= ~(BIT(PF_SW1) | BIT(PF_SW2));
	GPIOF->IBE &= ~(BIT(PF_SW1) | BIT(PF_SW2)); /* Disable both edge */
	GPIOF->IS &= ~(BIT(PF_SW1) | BIT(PF_SW2));  /* Edge detection */
	GPIOF->IEV = ~(BIT(PF_SW1) | BIT(PF_SW2));  /* Falling edge */
	GPIOF->IM |= (BIT(PF_SW1) | BIT(PF_SW2));
	
	/* Enable pin as output */
	GPIOC->DIR |= BIT(PC_4) | BIT(PC_5) | BIT(PC_6);
	GPIOC->DEN |= BIT(PC_4) | BIT(PC_5) | BIT(PC_6);

	/* Turn on pins */
	PC4_ON(); 
	PC6_ON(); 
	PC5_ON(); 
	
	/** enable timer clock **/
	SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R0;            
	while(0 == (SYSCTL->PRTIMER & SYSCTL_PRTIMER_R0)){}; 
		
	/** setup Timer 0A in 16-bits operation **/
	TIMER0->CTL &= ~TIMER_CTL_TAEN;          /* 1. disable timer 0 during setup */
	TIMER0->CFG |= TIMER_CFG_16_BIT;         /* 2. set to 16-bit mode */
	TIMER0->TAMR |= TIMER_TAMR_TAMR_PERIOD ; /* 3. periodic mode */
	TIMER0->TAMR &= ~TIMER_TAMR_TACDIR;      /* 4. count down */
	TIMER0->TAMR |= TIMER_TAMR_TAILD;        /* new TAILR/PR values loaded */
	/* after time-out */
	TIMER0->TAILR = 0x0C80;                  /* 5. reload value; */
	TIMER0->TAPR  = 0x00;                    /* 6. prescaler (8 bits) */
	TIMER0->IMR |= TIMER_IMR_TATOIM;         /* 7. enable timeout intr */
	TIMER0->ICR |= TIMER_ICR_TATOCINT;       /* 8. clear timeout flag */
	TIMER0->CTL |= TIMER_CTL_TAEN;           /* 9. enable timer 0 */
}		

void UART_Init(void)
{
	/* initialize GPIO PA0 (UART0_RX) & PA1 (UART0_TX)   */
	GPIOA->LOCK = GPIO_LOCK_KEY; 
	GPIOA->CR |= BIT(PA_UART0_RX) | BIT(PA_UART0_TX); 
	
	GPIOA->AFSEL |= BIT(PA_UART0_RX) | BIT(PA_UART0_TX);
	GPIOA->DEN   |= BIT(PA_UART0_RX) | BIT(PA_UART0_TX);
	GPIOA->AMSEL &= ~( BIT(PA_UART0_RX) | BIT(PA_UART0_TX) );
	GPIOA->PCTL  &= ~( GPIO_PCTL_PA0_M | GPIO_PCTL_PA1_M ); 
	GPIOA->PCTL  |= GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;
	
	UART0->CC &= ~UART_CC_CS_M; 							        
	UART0->CC |= UART_CC_CS_SYSCLK; 					        
  UART0->CTL &= ~UART_CTL_UARTEN;                   
  UART0->CTL &= ~UART_CTL_HSE;                       
                                                    
  UART0->IBRD = 43;                                 
  UART0->FBRD = 26;                                 
																						         
  UART0->LCRH &= ~UART_LCRH_WLEN_M; 	
	UART0->LCRH |= UART_LCRH_WLEN_8 | UART_LCRH_PEN; 	  
	UART0->LCRH |= UART_LCRH_EPS ;   									  									
	UART0->LCRH &= ~UART_LCRH_STP2;  									 									
																																							 
	UART0->LCRH |= UART_LCRH_FEN; 										             
																																							 
	UART0->CTL |= UART_CTL_TXE | UART_CTL_RXE; 										
	UART0->CTL |= UART_CTL_UARTEN; 										   		
}

void write_ASCII_UART0 (char c)
{
	while(0 != (UART0->FR & UART_FR_TXFF)){};
	UART0->DR = c;
}

char read_ASCII_UART0 (void)
{
	return(UART0->DR);
}










