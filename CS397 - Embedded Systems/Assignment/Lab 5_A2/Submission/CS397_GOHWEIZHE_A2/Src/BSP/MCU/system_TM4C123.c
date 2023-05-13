/**************************************************************************//**
 * @file     system_TM4C.c
 * @brief    CMSIS Device System Source File for
 *           Texas Instruments TIVA TM4C123 Device Series
 * @version  V1.01
 * @date     19. March 2015
 *
 * @note
 *                                                             modified by Keil
 ******************************************************************************/

#include <stdint.h>
#include "TM4C123GH6PM7.h"



/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define XTALM       (16000000UL)            /* Main         oscillator freq */
#define XTALI       (16000000UL)            /* Internal     oscillator freq */
#define XTAL30K     (   30000UL)            /* Internal 30K oscillator freq */
#define XTAL32K     (   32768UL)            /* external 32K oscillator freq */

#define PLL_CLK    (400000000UL)
#define ADC_CLK     (PLL_CLK/25)
#define CAN_CLK     (PLL_CLK/50)


/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = XTALI; /* by default internal RC 16MHz */


static uint32_t const XTAL_NO_PLL[] = 
{ 
	0,0,0,0,0,0,
	4000000,
	4096000,
	4915200,
	5000000,
	5120000,
	6000000,
	6144000,
	7372800,
	8000000,
	8192000,
	10000000,
	12000000,
	12288000,
	13560000,
	14318180,
	16000000,
	16384000,
	18000000,
	20000000,
	24000000,
	25000000
};



/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Get the OSC clock
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate( void )            /* Get Core Clock Frequency      */
{
	uint32_t rcc, rcc2;

	/* Determine clock frequency according to clock register values */
	rcc  = SYSCTL->RCC;
	rcc2 = SYSCTL->RCC2;

	/* If RCC2 enabled, it will override RCC for same bit field */
	if( 0 != (rcc2 & SYSCTL_RCC2_USERCC2) )
	{   
		/* RCC2 is used */

		if( 0 == (rcc2&SYSCTL_RCC2_BYPASS2) )
		{
			/* With PLL */
			SystemCoreClock = PLL_CLK;
			
			if( 0 == (rcc2&SYSCTL_RCC2_DIV400) )
			{
				/* PLL output is devided by 2. Refer page. 222 of datasheet */
				SystemCoreClock = SystemCoreClock>>1;
				
				/* divisor */
				if( 0 != (rcc&SYSCTL_RCC_USESYSDIV) ) 
				{ 
					SystemCoreClock = SystemCoreClock / (((rcc2>>SYSCTL_RCC2_SYSDIV2_S) & (0x3F)) + 1);
				}
			}
			else
			{
				/* divisor */
				if( 0 != (rcc&SYSCTL_RCC_USESYSDIV) ) 
				{ 
					SystemCoreClock = SystemCoreClock / (((rcc2>>SYSCTL_RCC2_SYSDIV2_DIV400_S) & (0x7F)) + 1);
				}
			}
		}
		else
		{
			/* Without PLL */
			SystemCoreClock = XTAL_NO_PLL[(rcc>>6)&0x1fU];
					/* */
			if( 0 != (rcc&SYSCTL_RCC_USESYSDIV) ) 
			{ 
				SystemCoreClock = SystemCoreClock / (((rcc2>>SYSCTL_RCC2_SYSDIV2_S) & (0x3F)) + 1);
			}
		}
		
	} 
	else 
	{
		if( 0 != (rcc&SYSCTL_RCC_BYPASS) )
		{                            /* check BYPASS */ /* Simulation does not work at this point */
			SystemCoreClock = XTAL_NO_PLL[(rcc>>6)&0x1fU];
		} 
		else
		{
			SystemCoreClock = PLL_CLK;
			SystemCoreClock = SystemCoreClock>>1; /* Devided by 2 */
		}

		if( 0 != (rcc&SYSCTL_RCC_USESYSDIV) ) 
		{
			SystemCoreClock = SystemCoreClock / (((rcc>>SYSCTL_RCC_SYSDIV_S) & (0x0F)) + 1);
		}
	}
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
	__IO uint32_t rcc;
	__IO uint32_t rcc2;
	
#if(__FPU_USED == 1)
    SCB->CPACR |= ((3UL<<10*2)           /* set CP10 Full Access */
                   | (3UL<<11*2) );      /* set CP11 Full Access */
#endif
	
	rcc = SYSCTL->RCC;
	rcc &= ~SYSCTL_RCC_MOSCDIS;
	
	SYSCTL->RCC = rcc;

 	/* Wait main clock ready */
	while( 0 == (SYSCTL->RIS & SYSCTL_RIS_MOSCPUPRIS) ){};	
	SYSCTL->MISC = SYSCTL_RIS_MOSCPUPRIS;
		
	/* Select main osc */
	rcc &= ~SYSCTL_RCC_OSCSRC_M;
	rcc |= SYSCTL_RCC_OSCSRC_MAIN;
		
	/* Select xtal value */
	rcc &= ~SYSCTL_RCC_XTAL_M;
	rcc |= SYSCTL_RCC_XTAL_16MHZ;
		
	/* Use divider. rcc2 will enable later. divisor2 will be used */
	rcc &= ~SYSCTL_RCC_SYSDIV_M;
	rcc |= SYSCTL_RCC_USESYSDIV;
		
	/* enable PLL to run normally and Switch to PLL */
	rcc &= ~(SYSCTL_RCC_PWRDN | SYSCTL_RCC_BYPASS);
		
	/* PWM clock */
	rcc &= ~SYSCTL_RCC_PWMDIV_M;
	rcc |= SYSCTL_RCC_PWMDIV_2;
		
	/* Enable RCC2 */
	SYSCTL->RCC2 |= SYSCTL_RCC2_USERCC2;
	
	rcc2 = 	SYSCTL->RCC2;
	rcc2 |= SYSCTL_RCC2_DIV400;
	rcc2 &= ~SYSCTL_RCC2_SYSDIV2LSB;
	rcc2 &= ~(0x7f<<SYSCTL_RCC2_SYSDIV2_DIV400_S);
	rcc2 &= ~SYSCTL_RCC2_OSCSRC2_M;
	rcc2 |=  SYSCTL_RCC2_OSCSRC2_MO;
	rcc2 |=  SYSCTL_RCC2_OSCSRC2_MO 
					| (4U<<SYSCTL_RCC2_SYSDIV2_DIV400_S);
	rcc2 &= ~SYSCTL_RCC2_PWRDN2;
	
	/* It is important to write RCC first prior to RCC2 */
	SYSCTL->RCC = rcc;
	SYSCTL->RCC2 = rcc2;

 	/* Wait PLL Lock */
	while( 0 == (SYSCTL->PLLSTAT & SYSCTL_PLLSTAT_LOCK) ){};
					
	/* Switch to PLL base on RCC2 now. PLL is locked correctly */
	SYSCTL->RCC2 &= ~SYSCTL_RCC2_BYPASS2;	
}




























