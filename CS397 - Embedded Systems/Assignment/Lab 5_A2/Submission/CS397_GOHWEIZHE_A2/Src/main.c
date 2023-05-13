	/*****************************************************************************
 @Project		: 
 @File 			: main.c
 @Details  	:
 @Author		: fongfh
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     16 Jul 18  		Initial Release
   
******************************************************************************/

#include "Common.h"
#include "Hal.h"
#include "BSP.h"
#include "LED.h"
#include "IRQ.h"
#include "string.h"

/*****************************************************************************
 Define
******************************************************************************/
#define BUZZ_5MS 					5U
#define DEBOUNCE 					15U

#define MAX_SECONDS				60
#define MAX_MINUTES				60
#define MAX_HOURS					23

#define MAX_NUM_BIT				9
#define BITSHIFT_1 				1

#define NUM128 						0x80

#define COMMAND_MODE  		0b10000000
#define WRITE_MODE    		0b10100000 
#define SYS_EN     				0b00000010 
#define LCD_ON      			0b00000110 
#define RC256K  	   			0b00110000
#define BIAS_THIRD_4_COM  0b01010010 

/*****************************************************************************
 commandType definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/
enum Commands
{
	START = 1,
	ADDRESS, 
	CODE
};

enum CommandSettings
{
	SYSTEM_ENABLE = 1,
	RC_256,
	BIAS,
	LCD_TURN_ON
};

static volatile BOOL		  g_bSystemTick = FALSE;

static volatile int 			nRGB 	 = RGB_OFF;
static volatile uint8_t 	nColor = 3U;
static volatile BOOL			g_bSW1 = FALSE;
static volatile BOOL			g_bSW2 = FALSE;
static volatile uint16_t	g_nSW1 = 0U;
static volatile uint16_t	g_nSW2 = 0U;
static volatile uint16_t	g_nBuzzer = 0U;

static volatile BOOL 			dataSent	= TRUE; 
static volatile BOOL 			dataStart = TRUE; 
static volatile BOOL 			sw1Pressed = FALSE;     
static volatile BOOL 			toggleDot = TRUE;      
static volatile BOOL		  secondsTick = FALSE;
static volatile BOOL		  Interrupt = FALSE;	

static volatile int  			commandType = 1;    
static volatile int  			index  = 0; 
static volatile int 			s_index = 0;      
static volatile int 			Hours = 0;              
static volatile int 			Minutes = 0;              
static volatile int 			Seconds = 0; 
static volatile int     	command = START;       
static volatile int     	i = 0;   

static volatile uint8_t 	commandID = COMMAND_MODE;       
static volatile uint8_t 	commandCode = SYS_EN;       
static volatile uint8_t 	address = 0;       
static volatile uint8_t 	Arr[6];      

static char 							s[255] = "";  
static volatile char 			g_UART_CharRx;  

//0,1,2,3,4,5,6,7,8,9
uint8_t m[10] = {0x7D,0x60,0x3E,0x7A,0x63,0x5B,0x5F,0x70,0x7F,0x7B}; 

/*****************************************************************************
 Local Functions
******************************************************************************/
extern void TIMER0A_IRQHandler( uint32_t Status );
void SetClock();
void DisplayString(char c);

/*****************************************************************************
 Implementation
******************************************************************************/

int main()
{
	BSPInit(); /* in BSP.c */
	UART_Init();
	SystemCoreClockUpdate();
	SysTick_Config( SystemCoreClock/1000 );   /* Initialize SysTick Timer to trigger every 1 ms */
	IRQ_Init();

	printf("Input New Time (stHHMMSS):\n\r");
	
	for(;;)
  {
		if( FALSE != g_bSystemTick )	/* Check if flag is set by the SysTick Handler  */
    {
      g_bSystemTick = FALSE;	/* Clear SysTick flag so we only processes it once */
			
			if(((UART0->FR & UART_FR_RXFE) == 0)) 
			{
				g_UART_CharRx = read_ASCII_UART0();		
				DisplayString(g_UART_CharRx);
			}
		}		
		
		if( FALSE != secondsTick ) 
		{
			secondsTick = FALSE;
			++Seconds;        
			
			if(Seconds >= MAX_SECONDS) 
			{
				g_nBuzzer = BUZZ_5MS;
				BUZZER_ON();
				++Minutes;  
				Seconds = 0;    	
				
				if(Minutes >= MAX_MINUTES) 
				{
				 ++Hours;  
				 Minutes = 0;   					 
	
				 if(Hours > MAX_HOURS) 
					 Hours = 0;  
				}
			}
			
			toggleDot = !toggleDot; 
			SetClock();             
		}
		
		if( FALSE != Interrupt ) 
		{
			Interrupt = FALSE;	
			
			if(dataSent) 
			{
				CS_LOW();        
				
				if(dataStart)  
					dataStart = !dataStart;						
				else if(GPIOC_B5)
					WR_LOW();
				else
					WR_HIGH(); 
				
				if(!dataStart && !GPIOC_B5) 
				{
					if(command) 
					{
						if(commandType == START)
						{
							if(commandID & NUM128)
								DATA_HIGH();
							else
							DATA_LOW(); 
							
							commandID <<= BITSHIFT_1;                              
							index++;
							
							if(index >= 3)                       
							{
								index = 0;
								commandType = CODE;
								commandCode >>= BITSHIFT_1;                          
							}
						}
						else if(commandType == CODE) 
						{
							if(commandCode & NUM128)
								DATA_HIGH();
							else 
								DATA_LOW(); 
							
							commandCode <<= BITSHIFT_1;                              
							index++;
							
							if(index >= MAX_NUM_BIT)                         
							{
								command++;
								
								if(command == RC_256)
									commandCode = RC256K;                       
								else if(command == BIAS)
									commandCode = BIAS_THIRD_4_COM;             
								else if(command == LCD_TURN_ON)
									commandCode = LCD_ON;                       
								else
								{
									command = 0;
									dataSent = FALSE;                  
								}
								commandCode >>= BITSHIFT_1;                            
								index = 0;
							}
						}		
					}
					else
					{
						if(commandType == START) 
						{
							if(commandID & NUM128)
								DATA_HIGH();
							else 
								DATA_LOW(); 
							
							commandID <<= BITSHIFT_1;                              
							index++;
							if(index >= 3)                       
							{
								index = 0;
								commandType = ADDRESS;
							}
						}
						else if(commandType == ADDRESS) 
						{
							if(address & NUM128)
								DATA_HIGH();
							else 
								DATA_LOW(); 
							
							address <<= BITSHIFT_1;                              
							index++;
							if(index >= 6)                        
							{
								commandType = CODE;
								index = 0;
							}
						}	
						else if(commandType == CODE)
						{
							if(commandCode & NUM128)
								DATA_HIGH();
							else 
								DATA_LOW(); 
							
							commandCode <<= BITSHIFT_1;                              
							index++;
							
							if(index >= 8)                         
							{
								i++;
								index = 0;
								commandCode = Arr[i];
								if(i >= 12)
								{
									commandType = 0;
									dataSent = FALSE;
									i = 0;
								}
							}
						}	
					}
				}
			}
			else
			{
				CS_HIGH();    
				DATA_HIGH();
			}
		}
		
		if(g_bSW1 == TRUE || g_bSW2 == TRUE)
		{
			if(g_bSW1 == TRUE)
				g_bSW1 = FALSE;
			
			if(g_bSW2 == TRUE)
				g_bSW2 = FALSE;
		}
		if(!SW1())
			sw1Pressed = TRUE;
		else
			sw1Pressed = FALSE;	
	}
}

/*****************************************************************************
 Callback functions
******************************************************************************/
void SysTick_Handler( void )  
{
	static int nCount = 0;
	g_bSystemTick = TRUE;
	
  if (nCount++ == 1000)
  {
		secondsTick = TRUE;
		nCount=0;
  }
	 
	if(g_nBuzzer != 0)
	{
		--g_nBuzzer;
    if (g_nBuzzer == 0)
			BUZZER_OFF();
	}
	
	if(g_nSW2 != 0)
	{
		--g_nSW2;
    if (g_nSW2 == 0)
			g_bSW2 = SW2();
	}
}

/*****************************************************************************
 Local functions
******************************************************************************/
void SetClock()
{
	memset((void*)Arr, 0, sizeof(Arr)); 
	
	index = 0;
	commandID = WRITE_MODE;
	commandType = START;
	dataStart = TRUE;
	dataSent = TRUE;
	
	if(sw1Pressed) 
	{
		int i = Seconds/10;
		int j = Seconds%10;
		
		Arr[2] = m[i];
		Arr[1] = m[j];
		
		if(toggleDot)
			Arr[2] |= 0b10000000;
	}
	else
	{
		int i = Hours/10;
		int j = Hours%10;
		int k = Minutes/10;
		int l = Minutes%10;
		
		Arr[4] = m[i];
		Arr[3] = m[j];
		Arr[2] = m[k];
		Arr[1] = m[l];
		
		if(toggleDot)
			Arr[2] |= 0b10000000;
	}
	
	commandCode = Arr[0]; 
	address = 0;
	i = 0;
}

void DisplayString(char c)
{
	if(c == '\r' )
	{
		s[s_index] = '\0';
		
		if( (s_index >= 8 ) && (s[0] == 's' || s[0] == 'S' )&&
			(s[1] == 't' || s[1] == 'T' ) ) 
		{
			char str[2];
			str[0] = s[2];
			str[1] = s[3];
			
			int hour = atoi(str);
			str[0] = s[4];
			str[1] = s[5];
			
			int minutes = atoi(str);
			str[0] = s[6];
			str[1] = s[7];
			
			int seconds = atoi(str);
			
			if( hour > MAX_HOURS || minutes >= MAX_MINUTES || seconds >= MAX_SECONDS)
				printf("\nError: Invalid Time Inputted\n\r");
			else
			{				
				for(int i = 2 ; i < 8; i++)
					write_ASCII_UART0(s[i]);
				
				printf("\n\r");
				Hours = hour;
				Minutes = minutes;
				Seconds = seconds;
			}
		}
		else
		{
			printf("\nError: Wrong Format\n\r"); 
		}
		
		printf("Input New Time(stHHMMSS):\n\r");
		s_index = 0;
	}
	else
		s[s_index++] = c;
}

/*****************************************************************************
 Interrupt functions
******************************************************************************/
void GPIOF_Button_IRQHandler( uint32_t Status )
{

	if( 0 != (Status&BIT(PF_SW1)) )
	{
		GPIOF->ICR = BIT(PF_SW1);
		sw1Pressed = TRUE;
	}
	
	if( 0 != (Status&BIT(PF_SW2)) )
	{
		GPIOF->ICR = BIT(PF_SW2);
			
		if(g_nSW2 == 0)
		{
			g_nSW2 = DEBOUNCE;
		}
	}
}

extern void TIMER0A_IRQHandler( uint32_t Status )
{
	/* if TRUE, timeout intr has occurred */
	if( 0 != (Status & TIMER_RIS_TATORIS) )
	{
		TIMER0->ICR |= TIMER_ICR_TATOCINT; /* clear intr */
		Interrupt = TRUE;
	}
}