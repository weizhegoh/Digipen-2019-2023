/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AELineSegment2.h

\author     Antoine Abi Chacra
\date       March 19, 2012

\brief      Header file for the 2D line segment library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_LS2_H
#define AE_LS2_H

// ---------------------------------------------------------------------------

typedef struct AELineSegment2
{
	AEVec2 mP0;		//!< Point on the line
	AEVec2 mP1;		//!< Point on the line
	AEVec2 mN;		//!< Line's normal
	float mNdotP0;	//!< To avoid computing it every time it's needed
}AELineSegment2;


// ---------------------------------------------------------------------------
#ifdef __cplusplus 
extern "C"
{
#endif

/******************************************************************************/
/*!
\fn         s32 AEBuildLineSegment2(AELineSegment2 *pLS,
                                    AEVec2 *pPt0,
                                    AEVec2 *pPt1)

\brief      Initalize pLS using pPt0 and pPt1. Also computes the 
            normal (unit vector) and the dot product of the normal
            with one of the points.

\warning    The distance between pPt0 and pPt1 should be greater
            than EPSILON.

\param      [out] pLS
            Pointer to AELineSegment2 to be set.

\param      [in] pPt0
            Pointer to AEVec2 for input.

\param      [in] pPt1
            Pointer to AEVec2 for input.

\retval     s32
            Returns 1 if pLS is successfully intialized.
            Else returns 0.
*/
/******************************************************************************/
AE_API s32 AEBuildLineSegment2(AELineSegment2 *pLS, AEVec2 *pPt0, AEVec2 *pPt1);

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // VEC2_H