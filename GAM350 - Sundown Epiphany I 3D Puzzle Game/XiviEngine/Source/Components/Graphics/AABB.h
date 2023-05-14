/*****************************************************************
*\file         AABB.h
*\author(s)    Kenric Tan Wei Liang

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "../XiviEngine/Source/Xivi/Graphics/Shapes/Shapes.h"
#include "Transform.h"
#include "Xivi/Core/Utils/Math.h"
namespace Xivi
{
enum class CollisionType
{
  AABB = 0,
  OBB,
  SPHERE
};

struct AABB
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "AABB"
  };
  CollisionType type;

  //AABB
  Xivi::Math::Vector3 AABB_mMin;
  Xivi::Math::Vector3 AABB_mMax;
  Xivi::Math::Vector3 AABB_center;
  float AABB_radius;

  Xivi::Math::Vector3 minScale = Xivi::Math::Vector3( 0.5f, 0.5f, 0.5f );
  Xivi::Math::Vector3 maxScale = Xivi::Math::Vector3( 0.5f, 0.5f, 0.5f );

  //Sphere
  Xivi::Math::Vector3 Sphere_mCenter;
  float Sphere_mRadius;

  bool isOnFrustum( Xivi::Graphics::Shapes::Frustum camFrustum, Transform transform )
  {
      //Get global scale thanks to our transform
      //Xivi::Math::Vector3 globalCenter{ transform.LocalToWorld * Xivi::Math::Vector4(AABB_center.getX(),AABB_center.getY(),AABB_center.getZ(), 1.f) };

      // Scaled orientation
      //const glm::vec3 right = transform.getRight() * extents.x;
      //const glm::vec3 up = transform.getUp() * extents.y;
      //const glm::vec3 forward = transform.getForward() * extents.z;

      //const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
      //    std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
      //    std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

      //const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
      //    std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
      //    std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

      //const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
      //    std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
      //    std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

      ////We not need to divise scale because it's based on the half extention of the AABB
      //const AABB globalAABB(globalCenter, newIi, newIj, newIk);

            //return (isOnOrForwardPlan(camFrustum.leftFace) &&
      //   isOnOrForwardPlan(camFrustum.rightFace) &&
      //   isOnOrForwardPlan(camFrustum.topFace) &&
      //   isOnOrForwardPlan(camFrustum.bottomFace) &&
      //   isOnOrForwardPlan(camFrustum.nearFace) &&
      //   isOnOrForwardPlan(camFrustum.farFace));

      //for sphere

      //Xivi::Math::Vector3 center = pos.m_position;
    float radius = ( AABB_mMax - AABB_mMin ).length() * 0.25f;

     //Get global scale is computed by doing the magnitude of
    //X, Y and Z model matrix's column.
    Xivi::Math::Vector3 globalScale = transform.scale;

    //Get our global center with process it with the global model matrix of our transform
    //Xivi::Math::Vector3 globalCenter{ transform.LocalToWorld * Xivi::Math::Vector4(AABB_center.getX(),AABB_center.getY(),AABB_center.getZ(), 1.f) };
    Xivi::Math::Vector3 globalCenter = AABB_center;
    //To wrap correctly our shape, we need the maximum scale scalar.
    const float _maxScale = std::max( std::max( globalScale.getX(), globalScale.getY() ), globalScale.getZ() );

    //Max scale is assuming for the diameter. So, we need the half to apply it to our radius
    //Check Firstly the result that have the most chance
    //to faillure to avoid to call all functions.
    //return (isOnOrForwardPlan(camFrustum.leftFace,globalCenter, (radius * (_maxScale * 0.5f))) &&
    //   isOnOrForwardPlan(camFrustum.rightFace, globalCenter, (radius * (_maxScale * 0.5f))) &&
    //   isOnOrForwardPlan(camFrustum.topFace, globalCenter, (radius * (_maxScale * 0.5f))) &&
    //   isOnOrForwardPlan(camFrustum.bottomFace, globalCenter, (radius * (_maxScale * 0.5f))) &&
    //   isOnOrForwardPlan(camFrustum.nearFace, globalCenter, (radius * (_maxScale * 0.5f))) &&
    //   isOnOrForwardPlan(camFrustum.farFace, globalCenter, (radius * (_maxScale * 0.5f))));

    return ( isOnOrForwardPlan( camFrustum.LeftPlane(), globalCenter, radius ) &&
             isOnOrForwardPlan( camFrustum.RightPlane(), globalCenter, radius ) &&
             isOnOrForwardPlan( camFrustum.TopPlane(), globalCenter, radius ) &&
             isOnOrForwardPlan( camFrustum.BottomPlane(), globalCenter, radius ) &&
             isOnOrForwardPlan( camFrustum.NearPlane(), globalCenter, radius ) &&
             isOnOrForwardPlan( camFrustum.FarPlane(), globalCenter, radius ) );
  };

  bool isOnOrForwardPlan( const Xivi::Graphics::Shapes::Plane &plane, Xivi::Math::Vector3 center, float radius ) const
  {
      //AABB_mMin;
      //AABB_mMax;
      //AABB_center;

      //Xivi::Math::Vector3 extents = Xivi::Math::Vector3(AABB_mMax.getX() - AABB_center.getX(), AABB_center.getY() - AABB_center.getY(), AABB_mMax.getZ() - AABB_center.getZ());

      //// Compute the projection interval radius of b onto L(t) = b.c + t * p.n
      //const float r = extents.getX() * std::abs(plane.normal.getX()) +
      //    extents.getY() * std::abs(plane.normal.getY()) + extents.getZ() * std::abs(plane.normal.getZ());

      //return -r <= plane.getSignedDistanceToPlan(AABB_center);

    return plane.getSignedDistanceToPlan( center ) > -radius;
  }
};
}

namespace Xivi::Components::AABB::Reflect
{
RTTR_REGISTRATION
{
// Collision Type
rttr::registration::enumeration<CollisionType>( "Collision Type" )
(
rttr::value( "AABB", CollisionType::AABB ),
rttr::value( "OBB", CollisionType::OBB ),
rttr::value( "Sphere", CollisionType::SPHERE )
);

// AABB
rttr::registration::class_<Xivi::AABB>( std::string( Xivi::AABB::info.m_name ) )
( rttr::metadata( "GUID", Xivi::ECS::Component::info<Xivi::AABB>.m_id ) )
.property( "AABB minScale", &Xivi::AABB::minScale )( rttr::policy::prop::as_reference_wrapper )
.property( "AABB maxScale", &Xivi::AABB::maxScale )( rttr::policy::prop::as_reference_wrapper )
.property( "Collision Type", &Xivi::AABB::type )/*(rttr::policy::prop::as_reference_wrapper)*/
.property( "AABB min", &Xivi::AABB::AABB_mMin )( rttr::policy::prop::as_reference_wrapper )
.property( "AABB max", &Xivi::AABB::AABB_mMax )( rttr::policy::prop::as_reference_wrapper )
.property( "AABB center", &Xivi::AABB::AABB_center )( rttr::policy::prop::as_reference_wrapper );
//.property("AABB radius", &Xivi::AABB::AABB_radius)(rttr::policy::prop::as_reference_wrapper)
//.property( "Sphere center", &Xivi::AABB::Sphere_mCenter )( rttr::policy::prop::as_reference_wrapper )
//.property( "Sphere radius", &Xivi::AABB::Sphere_mRadius )( rttr::policy::prop::as_reference_wrapper );
}
}