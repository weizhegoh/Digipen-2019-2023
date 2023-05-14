/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEVec2.h

\author     Sun Tjen Fam
\date       January 31, 2008

\brief      Header file for the 2D vector library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_VEC2_H
#define AE_VEC2_H

#include "AEExport.h"
#include "AETypes.h"

// ---------------------------------------------------------------------------

//AE_API typedef struct AEVec2
typedef struct AEVec2
{
	f32 x, y;
}AEVec2;

// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

//For testing? Function purpose unclear
AE_API s32	AEVec2Test				(s32 i1, s32 i2);

/******************************************************************************/
/*!
\fn         void AEVec2Zero(AEVec2* pResult)

\brief      Set pResult to zero.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Zero				(AEVec2* pResult);

/******************************************************************************/
/*!
\fn         void AEVec2Set(AEVec2* pResult, 
                           f32 x, 
                           f32 y)

\brief      Set pResult to (x, y).

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] x
            X value to set with.

\param      [in] y
            Y value to set with.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Set				(AEVec2* pResult, f32 x, f32 y);

/******************************************************************************/
/*!
\fn         void AEVec2Neg(AEVec2* pResult, 
                           AEVec2* pVec0)

\brief      Set pResult to the negative of pVec0. 

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Neg				(AEVec2* pResult, AEVec2* pVec0);

/******************************************************************************/
/*!
\fn         void AEVec2Add(AEVec2* pResult, 
                           AEVec2* pVec0, 
                           AEVec2* pVec1)

\brief      Set pResult to (pVec0 + pVec1). 

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Add				(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         void AEVec2Sub(AEVec2* pResult, 
                           AEVec2* pVec0, 
                           AEVec2* pVec1)

\brief      Set pResult to (pVec0 - pVec1). 

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Sub				(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         void AEVec2Normalize(AEVec2* pResult, 
                                 AEVec2* pVec0)

\brief      Set pResult to the normalized of pVec0.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Normalize			(AEVec2* pResult, AEVec2* pVec0);

/******************************************************************************/
/*!
\fn         void AEVec2Scale(AEVec2* pResult, 
                             AEVec2* pVec0, 
                             f32 s)

\brief      Set pResult to (pVec0 * s).

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] s
            Value to scale with.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Scale				(AEVec2* pResult, AEVec2* pVec0, f32 s);

/******************************************************************************/
/*!
\fn         void AEVec2ScaleAdd(AEVec2* pResult, 
                                AEVec2* pVec0, 
                                AEVec2* pVec1, 
                                f32 s)

\brief      Set pResult to ((pVec0 + pVec1) * s).

\warning    Purpose unclear or error in implementation.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\param      [in] s
            Value to scale with.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2ScaleAdd			(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1, f32 s);

/******************************************************************************/
/*!
\fn         void AEVec2ScaleSub(AEVec2* pResult, 
                                AEVec2* pVec0, 
                                AEVec2* pVec1, 
                                f32 s)

\brief      Set pResult to ((pVec0 - pVec1) * s).

\warning    Purpose unclear or error in implementation.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\param      [in] s
            Value to scale with.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2ScaleSub			(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1, f32 s);

/******************************************************************************/
/*!
\fn         void AEVec2Project(AEVec2* pResult, 
                               AEVec2* pVec0, 
                               AEVec2* pVec1)

\brief      Set pResult to the projection of pVec0 onto pVec1.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Project			(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         void AEVec2ProjectPerp(AEVec2* pResult, 
                                   AEVec2* pVec0, 
                                   AEVec2* pVec1)

\brief      Set pResult to the perpendicular projection (rejection)
            of pVec0 onto pVec1.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2ProjectPerp		(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         void AEVec2Lerp(AEVec2* pResult, 
                            AEVec2* pVec0, 
                            AEVec2* pVec1, 
                            f32 t)

\brief      Set pResult to the linear interpolation between pVec0 and
            pVec1 at time interval t.

\warning    t should be between 0.0f to 1.0f.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] pVec0
            Pointer to AEVec2 for input.

\param      [in] pVec1
            Pointer to AEVec2 for input.

\param      [in] t
            The time interval to calculate the linear interpolation at.
            At t = 0.0f, the result will be pVec0.
            At t = 1.0f, the result will be pVec1.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2Lerp				(AEVec2* pResult, AEVec2* pVec0, AEVec2* pVec1, f32 t);

/******************************************************************************/
/*!
\fn         f32 AEVec2Length(AEVec2* pVec0)

\brief      Calculate the length of pVec0.

\param      [in] pVec0
            Pointer to AEVec2 for input

\retval     f32
            Returns the length of pVec0.
*/
/******************************************************************************/
AE_API f32	AEVec2Length			(AEVec2* pVec0);

/******************************************************************************/
/*!
\fn         f32 AEVec2SquareLength(AEVec2* pVec0)

\brief      Calculate the squared length of pVec0.

\param      [in] pVec0
            Pointer to AEVec2 for input

\retval     f32
            Returns the squared length of pVec0.
*/
/******************************************************************************/
AE_API f32	AEVec2SquareLength		(AEVec2* pVec0);

/******************************************************************************/
/*!
\fn         f32	AEVec2Distance(AEVec2* pVec0, 
                               AEVec2* pVec1)

\brief      Calculate the distance between 2 points.

\param      [in] pVec0
            Pointer to AEVec2 of first point.

\param      [in] pVec1
            Pointer to AEVec2 of second point.

\retval     f32
            Returns the distance between pVec0 and pVec1.
*/
/******************************************************************************/
AE_API f32	AEVec2Distance			(AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         f32 AEVec2SquareDistance(AEVec2* pVec0, 
                                     AEVec2* pVec1)

\brief      Calculate the squared distance between 2 points.

\param      [in] pVec0
            Pointer to AEVec2 of first point.

\param      [in] pVec1
            Pointer to AEVec2 of second point.

\retval     f32
            Returns the squared distance between pVec0 and pVec1.
*/
/******************************************************************************/
AE_API f32	AEVec2SquareDistance	(AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         f32	AEVec2DotProduct(AEVec2* pVec0, 
                                 AEVec2* pVec1)

\brief      Calculate the dot product of 2 vectors.

\param      [in] pVec0
            Pointer to AEVec2 of first vector.

\param      [in] pVec1
            Pointer to AEVec2 of second vector.

\retval     f32
            Returns the dot product of pVec0 and pVec1.
*/
/******************************************************************************/
AE_API f32	AEVec2DotProduct		(AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         f32	AEVec2CrossProductMag(AEVec2* pVec0, 
                                      AEVec2* pVec1)

\brief      Calculate the magnitude of the cross product of 2 vectors.

\param      [in] pVec0
            Pointer to AEVec2 of first vector.

\param      [in] pVec1
            Pointer to AEVec2 of second vector.

\retval     f32
            Returns the magnitude of the cross product of pVec0 
            and pVec1.
*/
/******************************************************************************/
AE_API f32	AEVec2CrossProductMag	(AEVec2* pVec0, AEVec2* pVec1);

/******************************************************************************/
/*!
\fn         void AEVec2FromAngle(AEVec2* pResult, 
                                 f32 angle)

\brief      Set pResult to unit vector of angle.

\param      [out] pResult
            Pointer to AEVec2 to be set.

\param      [in] angle
            Angle of the unit vector.

\retval     void
            No return.
*/
/******************************************************************************/
AE_API void	AEVec2FromAngle         (AEVec2* pResult, f32 angle);

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // VEC2_H