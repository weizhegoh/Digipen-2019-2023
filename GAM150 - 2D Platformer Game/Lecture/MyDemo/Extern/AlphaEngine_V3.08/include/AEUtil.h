/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEUtil.h

\author     Sun Tjen Fam
\date       February 02, 2008

\brief      Header file for the utility library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef AE_UTIL_H
#define AE_UTIL_H

// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

/******************************************************************************/
/*!
\fn         f64 AEGetTime(f64* pTime)

\brief      Get the current time in seconds.

\param      [out] pTime
            Pointer to f64 to store the current time.
            May be left null if not needed.

\retval     f64
            Returns the current time in seconds.
*/
/******************************************************************************/
AE_API f64		AEGetTime(f64* pTime);

/******************************************************************************/
/*!
\fn         f32 AERandFloat()

\brief      Get a random real number between 0.0f to 1.0f.

\retval     f32
            Returns a random real number between 0.0f to 1.0f.
*/
/******************************************************************************/
AE_API f32		AERandFloat();


#define isZero(x) ((x < EPSILON) && (x > -EPSILON))

#define isEqual(x,y) (((x >= y) ? (x-y) : (y-x)) < EPSILON)




#ifdef __cplusplus 
}
#endif

/******************************************************************************/
/*!
\fn         s8* ReadFromFile(const s8 *fileName)

\brief      Read file into memory.

\details    Open a file named filename, allocate sufficient storage and
            read the file into memory.

\warning    User must delete the memory allocated on their own.

\param      [in] fileName
            Pointer to the name of the file.

\retval     s8*
            Pointer to the start of the memory containing the file.
*/
/******************************************************************************/
s8* ReadFromFile(const s8 *fileName);

// ---------------------------------------------------------------------------

#endif // AE_UTIL_H


