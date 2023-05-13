#include <Common.h>
#include "Hal.h"

/*****************************************************************************
 Define
******************************************************************************/
struct FILE
{
  int handle; 
};

FILE __stdout; //STDOUT



int fputc(int ch, FILE * stream)
{
	write_ASCII_UART0(ch); //Transmit Character
	return ch; //return the character written to denote a successful write
}
