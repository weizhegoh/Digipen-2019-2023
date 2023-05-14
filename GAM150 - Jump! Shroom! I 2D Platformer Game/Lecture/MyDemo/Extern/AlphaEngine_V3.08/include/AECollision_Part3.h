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

	// Extra credits
	float   m_mass{ 1.0f };
};

struct AERay
{
	AEVec2	m_pt0;
	AEVec2	m_dir;
};


// INTERSECTION FUNCTIONS
int AECollisionIntersection_CircleLineSegment(const AECircle &circle, 
											  const AEVec2 &ptEnd,
											  const AELineSegment &lineSeg, 
											  AEVec2 &interPt, 
											  AEVec2 &normalAtCollision,
											  float &interTime,
											  bool & checkLineEdges);//the last parameter is new - for Extra Credits

// Extra credits
int AECheckMovingCircleToLineEdge(bool withinBothLines,
									const AECircle &circle,
									const AEVec2 &ptEnd,
									const AELineSegment &lineSeg,
									AEVec2 &interPt,
									AEVec2 &normalAtCollision,
									float &interTime);


// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
// In the case of "dynamic circle vs static pillar", velB will be 0.0f
int AECollisionIntersection_CircleCircle(const AECircle &circleA,
									   const AEVec2 &velA,
									   const AECircle &circleB,
									   const AEVec2 &velB,
									   AEVec2 &interPtA,
									   AEVec2 &interPtB,
									   float &interTime);

// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
// this is a helper function to be used inside "CollisionIntersection_CircleCircle" function
// it is checking collision a moving dot vs a static circle
int AECollisionIntersection_RayCircle(const AERay &ray, 
									const AECircle &circle, 
									float &interTime);






// RESPONSE FUNCTIONS
void AECollisionResponse_CircleLineSegment(const AEVec2 &ptInter, 
										   const AEVec2 &normal,
										   AEVec2 &ptEnd, 
										   AEVec2 &reflected);

void AECollisionResponse_CirclePillar(const AEVec2 &normal,
									const float &interTime, 
									const AEVec2 &ptStart,
									const AEVec2 &ptInter, 									
									AEVec2 &ptEnd, 
									AEVec2 &reflectedVectorNormalized);

// Extra credits
void AECollisionResponse_CircleCircle(AEVec2 &normal, 
									const float interTime,
									AEVec2 &velA,
									const float &massA,
									AEVec2 &interPtA,
									AEVec2 &velB,
									const float &massB,
									AEVec2 &interPtB,
									AEVec2 &reflectedVectorA,
									AEVec2 &ptEndA,
									AEVec2 &reflectedVectorB,
									AEVec2 &ptEndB);
	

#endif //AE_COLLISION_H