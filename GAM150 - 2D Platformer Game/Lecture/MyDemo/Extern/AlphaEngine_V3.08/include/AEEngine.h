/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEEngine.h

\author     Sun Tjen Fam
\date       January 31, 2008

\brief      Header file for the Alpha Engine.

\details    The Alpha Engine consist of the following header files:
                - AEEngine.h
                - AETypes.h
                - AEMath.h
                    - AEVec2.h
                    - AEMtx33.h
                    - AELineSegment2.h
                - AEUtil.h
                - AEFrameRateController.h
                - AESystem.h
                - AEGraphcs.h
                - AEInput.h
                - AEGameStateMgr.h

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_ENGINE_H
#define AE_ENGINE_H

// ---------------------------------------------------------------------------
// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <windows.h>

#include "AEExport.h"
//#include "AECollision_Part2.h"//for CS230_Cage_Part2 project
#include "AECollision_Part3.h"//for CS230_Cage_Part3 project

// ---------------------------------------------------------------------------
// assert defines

#ifndef AE_FINAL    //User should define AE_FINAL to remove asserts

// ---------------------------------------------------------------------------

#define AE_ASSERT(x)														\
{																			\
	if((x) == 0)															\
	{																		\
		PRINT("AE_ASSERT: %s\nLine: %d\nFunc: %s\nFile: %s\n",				\
			#x, __LINE__, __FUNCTION__, __FILE__); 							\
		MessageBox(NULL, "Program Must Exit!", "Program Error", MB_OK);		\
		exit(1);															\
	}																		\
}

// ---------------------------------------------------------------------------

#define AE_ASSERT_MESG(x, ...)												\
{																			\
	if((x) == 0)															\
	{																		\
		PRINT("AE_ASSERT_MESG: %s\nLine: %d\nFunc: %s\nFile: %s\n",			\
			#x, __LINE__, __FUNCTION__, __FILE__);							\
		PRINT("Mesg: ");													\
		PRINT(__VA_ARGS__);													\
		PRINT("\n");														\
		MessageBox(NULL, "Program Must Exit!", "Program Error", MB_OK);		\
		exit(1);															\
	}																		\
}

// ---------------------------------------------------------------------------

#define AE_ASSERT_PARM(x)													\
{																			\
	if((x) == 0)															\
	{																		\
		PRINT("AE_ASSERT_PARM: %s\nLine: %d\nFunc: %s\nFile: %s\n",			\
			#x, __LINE__, __FUNCTION__, __FILE__);							\
		MessageBox(NULL, "Program Must Exit!", "Program Error", MB_OK);		\
		exit(1);															\
	}																		\
}

// ---------------------------------------------------------------------------

#define AE_ASSERT_ALLOC(x)													\
{																			\
	if((x) == 0)															\
	{																		\
		PRINT("AE_ASSERT_ALLOC: %s\nLine: %d\nFunc: %s\nFile: %s\n",		\
			#x, __LINE__, __FUNCTION__, __FILE__);							\
		MessageBox(NULL, "Program Must Exit!", "Program Error", MB_OK);		\
		exit(1);															\
	}																		\
}


// ---------------------------------------------------------------------------

#define AE_WARNING(x)											\
{																\
	if((x) == 0)												\
	{															\
		PRINT("AE_WARNING: %s\nLine: %d\nFunc: %s\nFile: %s\n",	\
			#x, __LINE__, __FUNCTION__, __FILE__); 				\
	}															\
}

// ---------------------------------------------------------------------------

#define AE_WARNING_MESG(x, ...)											\
{																		\
	if((x) == 0)														\
	{																	\
		PRINT("AE_WARNING_MESG: %s\nLine: %d\nFunc: %s\nFile: %s\n",	\
			#x, __LINE__, __FUNCTION__, __FILE__);						\
		PRINT("Mesg: ");												\
		PRINT(__VA_ARGS__);												\
		PRINT("\n");													\
	}																	\
}

// ---------------------------------------------------------------------------

#define AE_WARNING_PARM(x)												\
{																		\
	if((x) == 0)														\
	{																	\
		PRINT("AE_WARNING_PARM: %s\nLine: %d\nFunc: %s\nFile: %s\n",	\
			#x, __LINE__, __FUNCTION__, __FILE__);						\
	}																	\
}

// ---------------------------------------------------------------------------

#else // AE_FINAL

// ---------------------------------------------------------------------------

#define AE_ASSERT(x)
#define AE_ASSERT_MESG(x, ...)
#define AE_ASSERT_PARM(x)
#define AE_ASSERT_ALLOC(x)

#define AE_WARNING(x)
#define AE_WARNING_MESG(x, ...)
#define AE_WARNING_PARM(x)
#define AE_WARNING_ALLOC(x)

// ---------------------------------------------------------------------------

#endif // AE_FINAL

// ---------------------------------------------------------------------------

#define AE_FATAL_ERROR(...)												\
{																		\
	PRINT("AE_FATAL_ERROR: ");											\
	PRINT(__VA_ARGS__);													\
	MessageBox(NULL, "Program Must Exit!", "Program Error", MB_OK);		\
	exit(1);															\
}

// ---------------------------------------------------------------------------
// Alpha engine includes

#include "AETypes.h"                    //Typedefs
#include "AEMath.h"                     //Maths libraray
#include "AEUtil.h"                     //Utility library
#include "AEFrameRateController.h"      //Frame controller
#include "AESystem.h"                   //System library
#include "AEGraphics.h"                 //Graphics library
#include "AEInput.h"                    //Input library
#include "AEGameStateMgr.h"             //Gamestates manager

// ---------------------------------------------------------------------------

#endif // AE_ENGINE_H

