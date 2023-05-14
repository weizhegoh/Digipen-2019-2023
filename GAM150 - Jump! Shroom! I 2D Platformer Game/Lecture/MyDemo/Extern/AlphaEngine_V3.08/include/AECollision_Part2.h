// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	AECollision.h
// Author			:	DigiPen
// Creation Date	:	2008/01/31
// Purpose			:	main header file for the collision library
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef AE_COLLISION_H
#define AE_COLLISION_H

#include "AEVec2.h"

/******************************************************************************/
/*!

	*/
/******************************************************************************/
struct AELineSegment
{
	AEVec2		m_pt0;
	AEVec2		m_pt1;
	AEVec2		m_normal;
};

void AEBuildLineSegment(AELineSegment &lineSegment, 
						const AEVec2 &pos, 
						float scale, 
						float dir);

/******************************************************************************/
/*!

	*/
/******************************************************************************/
struct AECircle
{
	AEVec2  m_center;
	float	m_radius;
};

// Intersection functions
int AECollisionIntersection_CircleLineSegment(const AECircle &circle, 
											  const AEVec2 &ptEnd,
											  const AELineSegment &lineSeg, 
											  AEVec2 &interPt, 
											  AEVec2 &normalAtCollision,
											  float &interTime);

// Response functions
void AECollisionResponse_CircleLineSegment(const AEVec2 &ptInter, 
										   const AEVec2 &normal,
										   AEVec2 &ptEnd, 
										   AEVec2 &reflected);


#endif //AE_COLLISION_H