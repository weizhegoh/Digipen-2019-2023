/******************************************************************************/
/*!
\par        Project: Alpha Engine
\file       AEMath.h

\author     Sun Tjen Fam
\date       January 31, 2008

\brief      Header file for the math library.

\copyright  Copyright (C) 2013 DigiPen Institute of Technology. Reproduction 
            or disclosure of this file or its contents without the prior 
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#ifndef AE_MATH_H
#define AE_MATH_H

// ---------------------------------------------------------------------------

#include "AEVec2.h"             //Vector 2D
#include "AEMtx33.h"            //Matrix 3x3
#include "AELineSegment2.h"     //LineSegment 2D

#include <float.h>

#include "math.h"

// ---------------------------------------------------------------------------
#ifdef __cplusplus

extern "C"
{
#endif

//AE_API void MathInit();

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         f32 AEDegToRad(f32 x)

\brief      Convert an angle from Degree to Radians.

\param      [in] x
            The angle in Degree to be converted.

\retval     f32
            The angle in Radians after conversion.
*/
/******************************************************************************/
AE_API f32 AEDegToRad(f32 x);

/******************************************************************************/
/*!
\fn         f32 AERadToDeg(f32 x)

\brief      Convert an angle from Radians to Degree.

\param      [in] x
            The angle in Radians to be converted.

\retval     f32
            The angle in Degree after conversion.
*/
/******************************************************************************/
AE_API f32 AERadToDeg(f32 x);

/******************************************************************************/
/*!
\fn         f32 AESin(f32 x)

\brief      Calculate the Sine value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of sin(x).
*/
/******************************************************************************/
AE_API f32 AESin	(f32 x);

/******************************************************************************/
/*!
\fn         f32 AECos(f32 x)

\brief      Calculate the Cosine value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of cos(x).
*/
/******************************************************************************/
AE_API f32 AECos	(f32 x);

/******************************************************************************/
/*!
\fn         f32 AETan(f32 x)

\brief      Calculate the Tangent value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of tan(x).
*/
/******************************************************************************/
AE_API f32 AETan	(f32 x);

/******************************************************************************/
/*!
\fn         f32 AEASin(f32 x)

\brief      Calculate the ArcSine value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of asin(x).
*/
/******************************************************************************/
AE_API f32 AEASin	(f32 x);

/******************************************************************************/
/*!
\fn         f32 AEACos(f32 x)

\brief      Calculate the ArcCosine value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of acos(x).
*/
/******************************************************************************/
AE_API f32 AEACos	(f32 x);

/******************************************************************************/
/*!
\fn         f32 AEATan(f32 x)

\brief      Calculate the ArcTangent value of an angle.

\param      [in] x
            The angle in Radians.

\retval     f32
            The value of atan(x).
*/
/******************************************************************************/
AE_API f32 AEATan	(f32 x);

// ---------------------------------------------------------------------------

//@{ 
//! Macros for the trigo functions that take input in degree
    #define AESinDeg(x)		AESin(AEDegToRad(x))
    #define AECosDeg(x)		AECos(AEDegToRad(x))
    #define AETanDeg(x)		AETan(AEDegToRad(x))
    #define AEASinDeg(x)	AERadToDeg(AEASin(x))
    #define AEACosDeg(x)	AERadToDeg(AEACos(x))
    #define AEATanDeg(x)	AERadToDeg(AEATan(x))
//@}

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         u32 AEIsPowOf2(u32 x)

\brief      Check if x is a power of 2.

\details    Powers of 2 are values which can be presented as 2 ^ N,
            where N is any non-negative integer.
            Examples of powers of 2 are:
                - 1     (2 ^ 0)
                - 2     (2 ^ 1)
                - 4     (2 ^ 2)
                - 256   (2 ^ 8)
                - 1024  (2 ^ 10)

\param      [in] x
            The value to be checked.

\retval     u32
            Returns true (non-zero value) if x is a power of 2.
            Else return false (zero).
*/
/******************************************************************************/
AE_API u32		AEIsPowOf2	(u32 x);

/******************************************************************************/
/*!
\fn         u32 AENextPowOf2(u32 x)

\brief      Calculate the next power of 2 that is greater than x.

\details    Powers of 2 are values which can be presented as 2 ^ N,
            where N is any non-negative integer.
            Examples of powers of 2 are:
                - 1     (2 ^ 0)
                - 2     (2 ^ 1)
                - 4     (2 ^ 2)
                - 256   (2 ^ 8)
                - 1024  (2 ^ 10)

\param      [in] x
            The input value.

\retval     u32
            Returns the next power of 2 greater than x.
*/
/******************************************************************************/
AE_API u32		AENextPowOf2(u32 x);

/******************************************************************************/
/*!
\fn         u32 AELogBase2(u32 x)

\brief      Calculate the log2 of x.

\details    Calculate the log2 of x, rounded to the lower integer.

\param      [in] x
            The input value.

\retval     u32
            The log2 of x, rounded to lower integer.
*/
/******************************************************************************/
AE_API u32		AELogBase2	(u32 x);

/******************************************************************************/
/*!
\fn         f32 AEClamp(f32 x, f32 x0, f32 x1)

\brief      Clamp x to between x0 and x1.

\details    If x is lower than the minimum value (x0), return x0.
            If x is higher than the maximum value (x1), return x1.
            Else return x.

\param      [in] x
            The input value.

\param      [in] x0
            The minimum value.

\param      [in] x1
            The maximum value.

\retval     f32
            The clamped value of x.
*/
/******************************************************************************/
AE_API f32		AEClamp		(f32 X, f32 Min, f32 Max);

/******************************************************************************/
/*!
\fn         f32	AEWrap(f32 x, f32 x0, f32 x1)

\brief      Wraparound for x with respect to range (x0 to x1).

\details    If x is lesser than x0, return (x + range).
            If x is higher than x1, return (x - range).

\warning    Wraparound does not work if x is lesser than (x0 - range)
            or if x is greater than (x1 + range).

\param      [in] x
            The input value.

\param      [in] x0
            The lower bound of range

\param      [in] x1
            The upper bound of range.

\retval     f32
            The wraparound value of x with respect to range.
*/
/******************************************************************************/
AE_API f32		AEWrap		(f32 x, f32 x0, f32 x1);

/******************************************************************************/
/*!
\fn         f32	AEMin(f32 x, f32 y)

\brief      Find which of the 2 value is lower.

\details    If x is lower than y, return x.
            If y is lower than x, return y.

\param      [in] x
            The first input value.

\param      [in] y
            The second input value.

\retval     f32
            The lower of the 2 value.
*/
/******************************************************************************/
AE_API f32		AEMin		(f32 x, f32 y);


/******************************************************************************/
/*!
\fn         f32	AEMax(f32 x, f32 y)

\brief      Find which of the 2 value is higher.

\details    If x is higher than y, return x.
            If y is higher than x, return y.

\param      [in] x
            The first input value.

\param      [in] y
            The second input value.

\retval     f32
            The higher of the 2 value.
*/
/******************************************************************************/
AE_API f32		AEMax		(f32 x, f32 y);

/******************************************************************************/
/*!
\fn         s32 AEInRange(f32 x, f32 x0, f32 x1)

\brief      Find if x is in the range (x0 to x1), inclusive.

\details    If x is more than or equal to x0 OR 
            If x is less than or equal to x1, return true.
            Else return false.

\param      [in] x
            The input value.

\param      [in] x0
            The lower bound of range.

\param      [in] x1
            The upper bound of range.

\retval     s32
            Returns true if x is between x0 and x1, inclusive.
            Else return false.
*/
/******************************************************************************/
AE_API s32		AEInRange	(f32 x, f32 x0, f32 x1);

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         f32 AECalcDistPointToCircle	(AEVec2* pPos, 
                                         AEVec2* pCtr, 
                                         f32 radius)

\brief      Calculate the shortest distance from a point to 
            the edge of a circle.

\details    Given a point (pPos) and the center of a circle (pCtr) 
            of size (radius), calculate the shortest distance 
            from the point to the circumference of the circle.

\warning    Radius of circle should be a non-negative value.

\param      [in] pPos
            Pointer to AEVec2 containing the positon of the point.

\param      [in] pCtr
            Pointer to AEVec2 containing the position of 
            the center of the circle

\param      [in] radius
            The radius of the circle.

\retval     f32
            Returns the shortest distance from the point to the 
            edge of the circle. This value will be negative if
            the point is inside the circle.
*/
/******************************************************************************/
AE_API f32 AECalcDistPointToCircle		(AEVec2* pPos, AEVec2* pCtr, f32 radius);

/******************************************************************************/
/*!
\fn         f32 AECalcDistPointToRect(AEVec2* pPos, 
                                      AEVec2* pRect, 
                                      f32 sizeX, 
                                      f32 sizeY)

\brief      Calculate the shortest distance from a point to 
            the edge of a rectangle.

\details    Given a point (pPos) and the center of a rect (pRect) 
            of width (sizeX) and height (sizeY), calculate the 
            shortest distance from the point to the perimeter of 
            the rect.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pPos
            Pointer to AEVec2 containing the position of the point.

\param      [in] pRect
            Pointer to AEVec2 containing the position of 
            the center of the rect.

\param      [in] sizeX
            Width of the rect, i.e. the size of the rect 
            along the x-axis.

\param      [in] sizeY
            Height of the rect, i.e. the size of the rect 
            along the y-axis.

\retval     f32
            Returns the shortest distance from the point to the
            edge of the rect. This value will be negative if the
            point is inside the rect.
*/
/******************************************************************************/
AE_API f32 AECalcDistPointToRect		(AEVec2* pPos, AEVec2* pRect, f32 sizeX, f32 sizeY);

/******************************************************************************/
/*!
\fn         f32 AECalcDistPointToLineSeg(AEVec2* pPos, 
                                         AEVec2* pLine0, 
                                         AEVec2* pLine1)

\brief      Calculate the shortest distance from a point to 
            a line segment.

\details    Given a point (pPos) and start (pLine0) and end (pLine1)
            of a line segment, calculate the shortest distance from 
            the point to the line segment.

\param      [in] pPos
            Pointer to AEVec2 containing the position of the point.

\param      [in] pLine0
            Pointer to AEVec2 containing the start of the 
            line segment.

\param      [in] pLine1
            Pointer to AEVec2 containing the end of the 
            line segment.

\retval     f32
            Returns the shortest distance from the point to the
            line segment.
*/
/******************************************************************************/
AE_API f32 AECalcDistPointToLineSeg		(AEVec2* pPos, AEVec2* pLine0, AEVec2* pLine1);

/******************************************************************************/
/*!
\fn         f32 AECalcDistPointToConvexPoly(AEVec2* pPos, 
                                            AEVec2* pVtx, 
                                            u32 vtxNum)

\brief      Calculate the shortest distance from a point to the edge
            of a convex polygon.

\details    Given a point (pPos) and an array of vertices (pVtx)
            of size (vtxNum) making up a convex polygon, calculate 
            the shortest distance from the point to the edge of the
            polygon.

\warning    Function not implemented. Do not use.
            Currently returns -1.0f.

\param      [in] pPos
            Pointer to AEVec2 containing the position of the point.

\param      [in] pVtx
            Pointer to an array of AEVec2 containing the vertices 
            of the polygon.

\param      [in] vtxNum
            The number of vertices in the polygon

\retval     f32
            Returns the shortest distance from the point to the
            edge of the polygon. This value will be negative if the
            point is inside the polygon.
*/
/******************************************************************************/
AE_API f32 AECalcDistPointToConvexPoly	(AEVec2* pPos, AEVec2* pVtx, u32 vtxNum);

/******************************************************************************/
/*!
\fn         f32 AECalcDistCircleToCircle(AEVec2* pCtr0, 
                                         f32 radius0, 
                                         AEVec2* pCtr1, 
                                         f32 radius1)

\brief      Calculate the shortest distance between the edges of 
            two circles.

\details    Given the center of circle0 (pCtr0) of size (radius0)
            and the center of circle1 (pCtr1) of size (radius1),
            calculate the shortest distance between their edges.

\warning    radius of circles should be a non-negative value.

\param      [in] pCtr0
            Pointer to AEVec2 containing the center of circle0.

\param      [in] radius0
            The radius of circle0.

\param      [in] pCtr1
            Pointer to AEVec2 containing the center of circle1.

\param      [in] radius1
            The radius of circle1.

\retval     f32
            Returns the shortest distance between the edge of both
            circles. This value will be negative if the circles
            are overlapping.
*/
/******************************************************************************/
AE_API f32 AECalcDistCircleToCircle		(AEVec2* pCtr0, f32 radius0, AEVec2* pCtr1, f32 radius1);

/******************************************************************************/
/*!
\fn         f32 AECalcDistCircleToRect(AEVec2* pCtr, 
                                       f32 radius, 
                                       AEVec2* pRect, 
                                       f32 sizeX, 
                                       f32 sizeY)

\brief      Calculate the shortest distance between the edges of 
            a circle and a rectangle.

\details    Given the center of a circle (pCtr) of size (radius)
            and the center of a rect (pRect) of width (sizeX) and
            height (sizeY), calculate the shortest distance between
            their edges.

\warning    radius of circles should be a non-negative value.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pCtr
            Pointer to AEVec2 containing the center of the circle.

\param      [in] radius
            The radius of circle.

\param      [in] pRect
            Pointer to AEVec2 containing the center of the rect.

\param      [in] sizeX
            The width of the rect.

\param      [in] sizeY
            The height of the rect.

\retval     f32
            Returns the shortest distance between the edges of the
            circle and the rect. This value will be negative if the
            circle and rect are overlapping.
*/
/******************************************************************************/
AE_API f32 AECalcDistCircleToRect		(AEVec2* pCtr, f32 radius, AEVec2* pRect, f32 sizeX, f32 sizeY);

/******************************************************************************/
/*!
\fn         f32 AECalcDistRectToRect(AEVec2* pRect0, 
                                     f32 sizeX0, 
                                     f32 sizeY0, 
                                     AEVec2* pRect1, 
                                     f32 sizeX1, 
                                     f32 sizeY1, 
                                     AEVec2* pNormal)

\brief      Calculate the shortest distance between the edges of 
            two rectangles.

\details    Given the center of rect0 (pRect0) of width (sizeX0) and
            height (sizeY0) and the center of rect1 (pRect1) of
            width (sizeX1) and height (sizeY1), calculate the 
            shortest distance between their edges.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pRect0
            Pointer to AEVec2 containing the center of rect0.

\param      [in] sizeX0
            The width of rect0.

\param      [in] sizeY0
            The height of rect0.

\param      [in] pRect1
            Pointer to AEVec2 containing the center of rect1.

\param      [in] sizeX1
            The width of rect1.

\param      [in] sizeY1
            The height of rect1.

\param      [out] pNormal
            Pointer to AEVec2 where the direction from rect1 to rect0
            is stored. May be left null if not needed.

\retval     f32
            Returns the shortest distance between the edges of both
            rects. This value will be negative if they are 
            overlapping.
*/
/******************************************************************************/
AE_API f32 AECalcDistRectToRect			(AEVec2* pRect0, f32 sizeX0, f32 sizeY0, AEVec2* pRect1, f32 sizeX1, f32 sizeY1, AEVec2* pNormal);

/* Functions not defined
AE_API f32 AECalcDistCircleToLineSeg	(AEVec2* pPos, AEVec2* pLine0, AEVec2* pLine1);
AE_API f32 AECalcDistCircleToConvexPoly	(AEVec2* pPos, AEVec2* pVtx, u32 vtxNum);
*/

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         s32 AETestPointToCircle(AEVec2* pPos, 
                                    AEVec2* pCtr, 
                                    f32 radius)

\brief      Test if a point is inside a circle.

\details    Given a point (pPos) and the center of a circle (pCtr)
            of size (radius), check if the point is in the
            circle.

\warning    Radius of circle should be a non-negative value.

\param      [in] pPos
            Pointer to AEVec2 containing the position of the point.

\param      [in] pCtr
            Pointer to AEVec2 containing the center of the circle.

\param      [in] radius
            The radius of circle0.

\retval     s32
            Returns true if the point is inside the circle.
            Else return false.
*/
/******************************************************************************/
AE_API s32 AETestPointToCircle			(AEVec2* pPos, AEVec2* pCtr, f32 radius);

/******************************************************************************/
/*!
\fn         s32 AETestPointToRect(AEVec2* pPos, 
                                  AEVec2* pRect, 
                                  f32 sizeX, 
                                  f32 sizeY)

\brief      Test if a point is inside a rectangle.

\details    Given a point (pPos) and the center of a rect (pRect)
            of width (sizeX) and height (sizeY), check if the 
            point is in the rect.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pPos
            Pointer to AEVec2 containing the position of the point.

\param      [in] pRect
            Pointer to AEVec2 containing the center of the rect.

\param      [in] sizeX
            The width of the rect.

\param      [in] sizeY
            The height of the rect.

\retval     s32
            Returns true if the point is inside the rect.
            Else return false.
*/
/******************************************************************************/
AE_API s32 AETestPointToRect			(AEVec2* pPos, AEVec2* pRect, f32 sizeX, f32 sizeY);

/******************************************************************************/
/*!
\fn         s32 AETestCircleToCircle(AEVec2* pCtr0, 
                                     f32 radius0, 
                                     AEVec2* pCtr1, 
                                     f32 radius1)

\brief      Test for collision between two circles.

\details    Given the center of circle0 (pCtr0) of size (radius0)
            and the center of circle1 (pCtr1) of size (radius1),
            check if they are colliding.

\warning    Radius of circle should be a non-negative value.

\param      [in] pCtr0
            Pointer to AEVec2 containing the center of circle0.

\param      [in] radius0
            The radius of circle0.

\param      [in] pCtr1
            Pointer to AEVec2 containing the center of circle1.

\param      [in] radius1
            The radius of circle1.

\retval     s32
            Returns true if the circles are colliding.
            Else return false.
*/
/******************************************************************************/
AE_API s32 AETestCircleToCircle			(AEVec2* pCtr0, f32 radius0, AEVec2* pCtr1, f32 radius1);

/******************************************************************************/
/*!
\fn         s32 AETestCircleToRect(AEVec2* pCtr, 
                                   f32 radius, 
                                   AEVec2* pRect, 
                                   f32 sizeX, 
                                   f32 sizeY);

\brief      Test for collision between a circle and a rectangle.

\details    Given the center of a circle (pCtr) of size (radius)
            and the center of a rect (pRect) of width (sizeX) and
            height (sizeY), check if they are colliding.

\warning    radius of circles should be a non-negative value.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pCtr
            Pointer to AEVec2 containing the center of the circle.

\param      [in] radius
            The radius of circle.

\param      [in] pRect
            Pointer to AEVec2 containing the center of the rect.

\param      [in] sizeX
            The width of the rect.

\param      [in] sizeY
            The height of the rect.

\retval     s32
            Returns true if the circle and the rect are colliding.
            Else return false.
*/
/******************************************************************************/
AE_API s32 AETestCircleToRect			(AEVec2* pCtr, f32 radius, AEVec2* pRect, f32 sizeX, f32 sizeY);

/******************************************************************************/
/*!
\fn         s32 AETestRectToRect(AEVec2* pRect0, 
                                 f32 sizeX0, 
                                 f32 sizeY0, 
                                 AEVec2* pRect1, 
                                 f32 sizeX1, 
                                 f32 sizeY1)

\brief      Test for collision between two rectangles.

\details    Given the center of rect0 (pRect0) of width (sizeX0) and
            height (sizeY0) and the center of rect1 (pRect1) of
            width (sizeX1) and height (sizeY1), check if they are
            colliding.

\warning    The width and height of the rect should be 
            non-negative values.

\warning    Function will only work if the rect is not rotated,
            i.e. the sides of the rect are parallel to the axis.

\param      [in] pRect0
            Pointer to AEVec2 containing the center of rect0.

\param      [in] sizeX0
            The width of rect0.

\param      [in] sizeY0
            The height of rect0.

\param      [in] pRect1
            Pointer to AEVec2 containing the center of rect1.

\param      [in] sizeX1
            The width of rect1.

\param      [in] sizeY1
            The height of rect1.

\retval     s32
            Returns true if the rects are colliding.
            Else return false.
*/
/******************************************************************************/
AE_API s32 AETestRectToRect				(AEVec2* pRect0, f32 sizeX0, f32 sizeY0, AEVec2* pRect1, f32 sizeX1, f32 sizeY1);

// ---------------------------------------------------------------------------

/******************************************************************************/
/*!
\fn         f32 AEStaticPointToStaticLineSegment(AEVec2 *pPos, 
                                                 AELineSegment2 *pLine)

\brief      Calculate the shortest distance from a point to a line.

\details    Given a point (pPos) and a line (pLine), calculate the
            shortest distance from the point to the line.

\warning    Line is assumed to stretch to infinity.

\param      [in] pPos
            Pointer to AEVec2 containing the point.

\param      [in] pLine
            Pointer to AELineSegment2 containing the line.

\retval     f32
            Returns the distance from the point to the line.
            Negative if the point is in the line's inside half plane.
            Positive if the point is in the line's outside half plane.
            Otherwise zero if the point is on the line.
*/
/******************************************************************************/
AE_API f32 AEStaticPointToStaticLineSegment(AEVec2 *pPos, AELineSegment2 *pLine);

/******************************************************************************/
/*!
\fn         f32 AEAnimatedPointToStaticLineSegment(AEVec2 *pStart, 
                                                   AEVec2 *pEnd, 
                                                   AELineSegment2 *pLine, 
                                                   AEVec2 *pInter)

\brief      Calculate the collision between a moving point with
            a line.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving point and a line (pLine), calculate the time 
            and point of intersection.

\warning    Line is assumed to stretch to infinity.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the point.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the point.

\param      [in] pLine
            Pointer to AELineSegment2 containing the line.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEAnimatedPointToStaticLineSegment(AEVec2 *pStart, AEVec2 *pEnd, AELineSegment2 *pLine, AEVec2 *pInter);

/******************************************************************************/
/*!
\fn         f32 AEAnimatedCircleToStaticLineSegment(AEVec2 *pStart, 
                                                    AEVec2 *pEnd, 
                                                    f32 radius, 
                                                    AELineSegment2 *pLine, 
                                                    AEVec2 *pInter)

\brief      Calculate the collision between a moving circle with
            a line.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving circle of size (radius) and a line (pLine), 
            calculate the time and point of intersection.

\warning    Line is assumed to stretch to infinity.

\warning    Radius of circle should be a non-negative value.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the circle.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the circle.

\param      [in] radius
            The radius of the circle.

\param      [in] pLine
            Pointer to AELineSegment2 containing the line.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEAnimatedCircleToStaticLineSegment(AEVec2 *pStart, AEVec2 *pEnd, f32 radius, AELineSegment2 *pLine, AEVec2 *pInter);

/******************************************************************************/
/*!
\fn         f32 AEReflectAnimatedPointOnStaticLineSegment(AEVec2 *pStart, 
                                                          AEVec2 *pEnd, 
                                                          AELineSegment2 *pLine, 
                                                          AEVec2 *pInter, 
                                                          AEVec2 *pReflect)

\brief      Calculate the collision between a moving point with
            a line and the reflected path of the point.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving point and a line (pLine), calculate the time,
            point of intersection and reflected path. 

\warning    Line is assumed to stretch to infinity.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the point.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the point.

\param      [in] pLine
            Pointer to AELineSegment2 containing the line.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\param      [out] pReflect
            Pointer to AEVec2 for storing the reflected path.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEReflectAnimatedPointOnStaticLineSegment(AEVec2 *pStart, AEVec2 *pEnd, AELineSegment2 *pLine, AEVec2 *pInter, AEVec2 *pReflect);

/******************************************************************************/
/*!
\fn         f32 AEReflectAnimatedCircleOnStaticLineSegment(AEVec2 *pStart, 
                                                           AEVec2 *pEnd, 
                                                           f32 radius, 
                                                           AELineSegment2 *pLine, 
                                                           AEVec2 *pInter, 
                                                           AEVec2 *pReflect)

\brief      Calculate the collision between a moving circle with
            a line and the reflected path of the circle.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving circle of size (radius) and a line (pLine), 
            calculate the time, point of intersection and reflected
            path.

\warning    Line is assumed to stretch to infinity.

\warning    Radius of circle should be a non-negative value.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the circle.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the circle.

\param      [in] radius
            The radius of the circle.

\param      [in] pLine
            Pointer to AELineSegment2 containing the line.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\param      [out] pReflect
            Pointer to AEVec2 for storing the reflected path.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEReflectAnimatedCircleOnStaticLineSegment(AEVec2 *pStart, AEVec2 *pEnd, f32 radius, AELineSegment2 *pLine, AEVec2 *pInter, AEVec2 *pReflect);

/******************************************************************************/
/*!
\fn         f32 AEAnimatedPointToStaticCircle(AEVec2 *pStart, 
                                              AEVec2 *pEnd, 
                                              AEVec2 *pCtr, 
                                              f32 radius, 
                                              AEVec2 *pInter)

\brief      Calculate the collision between a moving point with
            a circle.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving point and the center of a circle (pCtr) of
            size (radius), calculate the time and point of 
            intersection.

\warning    Radius of circle should be a non-negative value.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the point.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the point.

\param      [in] pCtr
            Pointer to AEVec2 containing the center of the circle.

\param      [in] radius
            The radius of the circle.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEAnimatedPointToStaticCircle(AEVec2 *pStart, AEVec2 *pEnd, AEVec2 *pCtr, f32 radius, AEVec2 *pInter);

/******************************************************************************/
/*!
\fn         f32 AEReflectAnimatedPointOnStaticCircle(AEVec2 *pStart, 
                                                     AEVec2 *pEnd, 
                                                     AEVec2 *pCtr, 
                                                     f32 radius, 
                                                     AEVec2 *pInter,
                                                     AEVec2 *pReflect)

\brief      Calculate the collision between a moving point with
            a circle and the reflected path of the point.

\details    Given the start position (pStart) and end position (pEnd) 
            of a moving point and the center of a circle (pCtr) of
            size (radius), calculate the time, point of intersection
            and reflected path.

\warning    Radius of circle should be a non-negative value.

\param      [in] pStart
            Pointer to AEVec2 containing start pos of the point.

\param      [in] pEnd
            Pointer to AEVec2 containing end pos of the point.

\param      [in] pCtr
            Pointer to AEVec2 containing the center of the circle.

\param      [in] radius
            The radius of the circle.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\param      [out] pReflect
            Pointer to AEVec2 for storing the reflected path.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEReflectAnimatedPointOnStaticCircle(AEVec2 *pStart, AEVec2 *pEnd, AEVec2 *pCtr, f32 radius, AEVec2 *pInter, AEVec2 *pReflect);

/******************************************************************************/
/*!
\fn         f32 AEAnimatedCircleToStaticCircle(AEVec2 *pCtr0s, 
                                               AEVec2 *pCtr0e, 
                                               f32 radius0, 
                                               AEVec2 *pCtr1, 
                                               f32 radius1, 
                                               AEVec2 *pInter)

\brief      Calculate the collision between a moving circle with
            a static circle.

\details    Given the start position (pCtr0s) and end position 
            (pCtr0e) of moving circle0 of size (radius0) and the 
            center of static circle1 (pCtr1) of size (radius1), 
            calculate the time and point of intersection.

\warning    Radius of circles should be a non-negative value.

\param      [in] pCtr0s
            Pointer to AEVec2 containing start pos of circle0.

\param      [in] pCtr0e
            Pointer to AEVec2 containing end pos of the circle0.

\param      [in] radius0
            The radius of circle0.

\param      [in] pCtr1
            Pointer to AEVec2 containing the center of circle1.

\param      [in] radius1
            The radius of the circle1.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEAnimatedCircleToStaticCircle(AEVec2 *pCtr0s, AEVec2 *pCtr0e, f32 radius0, AEVec2 *pCtr1, f32 radius1, AEVec2 *pInter);

/******************************************************************************/
/*!
\fn         f32 AEReflectAnimatedCircleOnStaticCircle(AEVec2 *pCtr0s, 
                                                      AEVec2 *pCtr0e, 
                                                      f32 radius0, 
                                                      AEVec2 *pCtr1, 
                                                      f32 radius1, 
                                                      AEVec2 *pInter, 
                                                      AEVec2 *pReflect)

\brief      Calculate the collision between a moving circle with
            a static circle and the reflected path of moving circle.

\details    Given the start position (pCtr0s) and end position 
            (pCtr0e) of moving circle0 of size (radius0) and the 
            center of static circle1 (pCtr1) of size (radius1), 
            calculate the time, point of intersection and reflected
            path of circle0.

\warning    Radius of circles should be a non-negative value.

\param      [in] pCtr0s
            Pointer to AEVec2 containing start pos of circle0.

\param      [in] pCtr0e
            Pointer to AEVec2 containing end pos of the circle0.

\param      [in] radius0
            The radius of circle0.

\param      [in] pCtr1
            Pointer to AEVec2 containing the center of circle1.

\param      [in] radius1
            The radius of the circle1.

\param      [out] pInter
            Pointer to AEVec2 for storing the point of intersection.
            Will not be used if there is no collision.

\param      [out] pReflect
            Pointer to AEVec2 for storing the reflected path.
            Will not be used if there is no collision.

\retval     f32
            Returns the time of collision, if there is one.
            Else return -1.0f.
*/
/******************************************************************************/
AE_API f32 AEReflectAnimatedCircleOnStaticCircle(AEVec2 *pCtr0s, AEVec2 *pCtr0e, f32 radius0, AEVec2 *pCtr1, f32 radius1, AEVec2 *pInter, AEVec2 *pReflect);



/*
// sweep a circle with radius 'radius' from ctr0 -> ctr1 againts a point
// * return -ve if circle does not touch the point at any time
AE_API f32 AESweepCircleToPoint	(AEVec2* pCtr0, AEVec2* pCtr1, f32 radius, AEVec2* pP);

// sweep a circle with radius 'radius' from ctr0 -> ctr1 againts a line segment
// * return -ve if circle does not intersect the line segment at any time
AE_API f32 AESweepCircleToLineSeg	(AEVec2* pCtr0, AEVec2* pCtr1, f32 radius, AEVec2* pP0, AEVec2* pP1, AEVec2* pN);


//TO TEST
//Sweeps a moving point against a static line
AE_API f32 AESweepPointToLine		(AEVec2 *pPos, AEVec2 *pVel, AEVec2 *pPnt, AEVec2 *pDirection);

//TO TEST
//Sweeps a moving circle against a static line
AE_API f32 AESweepCircleToLine		(AEVec2 *pCtr, f32 radius, AEVec2 *pVel, AEVec2 *pPnt, AEVec2 *pDirection);

//TO TEST
//Reflects a moving point on a static line. Returns 0 if there is no
//collision between the point and the line.
AE_API s32 AEReflectPointOnLine	(AEVec2 *pPos, AEVec2 *pVel, AEVec2 *pPnt, AEVec2 *pDirection, AEVec2 *pNewPosition, AEVec2 *pNewVelocity);


//TO TEST
//Reflects a moving circle on a static line. Returns 0 if there is no
//collision between the circle and the line.
AE_API s32 AEReflectCircleOnLine	(AEVec2 *pCtr, f32 radius, AEVec2 *pVel, AEVec2 *pPnt, AEVec2 *pDirection, AEVec2 *pNewPosition, AEVec2 *newVelocity);
*/
// ---------------------------------------------------------------------------

#ifdef __cplusplus 
}
#endif

// ---------------------------------------------------------------------------

#endif // AE_MATH_H
