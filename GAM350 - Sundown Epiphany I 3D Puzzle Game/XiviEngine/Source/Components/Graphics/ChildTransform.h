/*****************************************************************
*\file         ChildTransform.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Xivi/Core/Utils/Math.h"

namespace Xivi
{
struct ChildTransform
{
  constexpr static auto info = Xivi::ECS::Component::Type::Data
  {
    .m_name = "Child Transform"
  };
  //allow change
  Vec3f scale { 5.f, 5.f, 5.f };
  Vec3f axis { 0.f, 0.f, 0.f };
  //dont allow change
  Math::SIMD::Mat4x4 model = Math::SIMD::Mat4x4( 1.0 );
};
}

namespace Xivi::Components::Transform::Reflect
{
  //RTTR_REGISTRATION
  //{
  //	// Transform
  //	  rttr::registration::class_<Xivi::ChildTransform>(std::string(Xivi::ChildTransform::info.m_name))
  //	(rttr::metadata("GUID", Xivi::ECS::Component::info<Xivi::ChildTransform>.m_id))
  //	.property("Scale", &Xivi::ChildTransform::scale)(rttr::policy::prop::as_reference_wrapper)
  //	.property("Axis", &Xivi::ChildTransform::axis)(rttr::policy::prop::as_reference_wrapper)
  //	.property("Model", &Xivi::ChildTransform::model)(rttr::metadata("NoSerialise", true), rttr::metadata("NoEditor", true));
  //}
}