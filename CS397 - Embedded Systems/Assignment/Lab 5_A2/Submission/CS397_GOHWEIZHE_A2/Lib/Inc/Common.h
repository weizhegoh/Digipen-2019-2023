/*****************************************************************************
 @Project	: Common include header for embbeded system
 @File 		: Common.h
 @Details  	: Common share macro, type         
 @Author	: lcgan
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/

#ifndef __COMMON_DOT_H__
#define __COMMON_DOT_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "TM4C123GH6PM7.h"

/*****************************************************************************
 Define
******************************************************************************/
#define TRUE		1
#define FALSE		0

#define PI			3.1415926535897932384626433832795
#define RAD_TO_DEG	57.295779513082320876798154814105
#define UNUSE( a )	(void)(a)
#define ABS( a )	(((a)<0)? -(a) : (a))
#define BIT( x )	(1U<<(x))

#define CLOCK_WAIT() __NOP();__NOP();__NOP();__NOP();

#define CONCAT (a,b) a ## b				/* concatenate  */

#define CI_TRACE   printf

#ifdef _DEBUG
	#define TRACE   printf
#else	
    inline  void 	PrintfEmpty( const char * string, ... ) __attribute__((always_inline));
	inline  void 	PrintfEmpty( const char * string, ... ){}
	#define TRACE	PrintfEmpty
#endif

#ifdef _DEBUG
#define ASSERT( e )													\
{																	\
	if (!(e))														\
	{																\
		TRACE( "Error! Halt at %s %d\r\n", __FILE__, __LINE__ ); 	\
		while( 1 );													\
	}																\
}
#else
	#define ASSERT( e )( (void)0 )		/* no effect in release */
#endif


/*****************************************************************************
 typedef
******************************************************************************/
typedef int BOOL;

		
#endif /* __COMMON_DOT_H__ */














