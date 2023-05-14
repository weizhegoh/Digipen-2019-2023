/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEFrameRateController.h

\author     Sun Tjen Fam
\date       April 26, 2007

\brief      Header file for the frame rate controller

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_FRAME_RATE_CONTROLLER_H
#define AE_FRAME_RATE_CONTROLLER_H

// ---------------------------------------------------------------------------
// Externs

// ---------------------------------------------------------------------------
// Function prototypes
// ---------------------------------------------------------------------------

#ifdef __cplusplus

extern "C"
{
#endif

// ---------------------------------------------------------------------------

// Frame management
    
/******************************************************************************/
/*!
\fn         bool AEFrameRateControllerInit(u32 FrameRateMax)

\brief      Initialize the frame rate controller.

\warning    This function is already called in AESysInit.

\param      [in] FrameRateMax
            The maximum number of frames per second.

\retval     bool
            Returns true
*/
/******************************************************************************/
AE_API void	AEFrameRateControllerInit	(u32 FrameRateMax);

/******************************************************************************/
/*!
\fn         void AEFrameRateControllerReset()

\brief      Reset the frame rate controller.

\warning    This function is already called in AESysReset.

\retval     void
            No return
*/
/******************************************************************************/
AE_API void	AEFrameRateControllerReset	();

/******************************************************************************/
/*!
\fn         void AEFrameRateControllerStart()

\brief      Tell the frame rate controller that a new frame 
            is starting.

\warning    This function is already called in AESysFrameStart.

\retval     void
            No return
*/
/******************************************************************************/
AE_API void	AEFrameRateControllerStart	();

/******************************************************************************/
/*!
\fn         void AEFrameRateControllerEnd()

\brief      Tell the frame rate controller that the current frame 
            is ending.

\warning    This function is already called in AESysFrameEnd.

\retval     void
            No return
*/
/******************************************************************************/
AE_API void	AEFrameRateControllerEnd	();

/******************************************************************************/
/*!
\fn         f64 AEFrameRateControllerGetFrameTime()

\brief      Get the time for the previous frame, in seconds.

\retval     f64
            Return the time for the previous frame, in seconds.
*/
/******************************************************************************/
AE_API f64 AEFrameRateControllerGetFrameTime();

/******************************************************************************/
/*!
\fn         u32 AEFrameRateControllerGetFrameCount()

\brief      Get the total number of frames elapsed.

\retval     u32
            Return the total number of frames elapsed.
*/
/******************************************************************************/
AE_API u32 AEFrameRateControllerGetFrameCount();


/******************************************************************************/
/*!
\fn         u32 AEFrameRateControllerGetFrameRate()

\brief      Get the current fps.

\retval     f64
Return the total number of frames elapsed.
*/
/******************************************************************************/
AE_API f64 AEFrameRateControllerGetFrameRate();

// ---------------------------------------------------------------------------

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_FRAME_RATE_CONTROLLER_H

