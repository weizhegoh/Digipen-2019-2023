/*****************************************************************
*\file         GraphicsSystems.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Systems/Particles/ParticleSystem.h"
#include "Systems/Graphics/Transform/LightTransformSystem.h"
#include "Systems/Graphics/Transform/TransformSystem.h"
#include "Systems/Graphics/Transform/SingleTransformSystem.h"
#include "Systems/Graphics/Transform/FamilyTransformSystem.h"
#include "Systems/Graphics/Transform/UITransformSystem.h"
#include "Systems/Graphics/Transform/InterpolateSystem.h"
#include "Systems/Graphics/Camera/CameraSystem.h"
#include "Systems/Graphics/Camera/CameraCollisionSystem.h"
#include "Systems/Graphics/Rendering/RendererSystem.h"
#include "Systems/Graphics/Rendering/ShadowRendererSystem.h"
#include "Systems/Graphics/Rendering/StaticRendererSystem.h"
#include "Systems/Graphics/Rendering/AnimationRendererSystem.h"
#include "Systems/Graphics/Rendering/SkyBoxRendererSystem.h"
#include "Systems/Graphics/Rendering/NormalRendererSystem.h"
#include "Systems/Graphics/Rendering/WireFrameRendererSystem.h"
#include "Systems/Graphics/Rendering/DebugDrawRendererSystem.h"
#include "Systems/Graphics/Rendering/UIRendererSystem.h"
#include "Systems/Graphics/AnimationStateSystem.h"
#include "Systems/Graphics/Rendering/GridRendererSystem.h"
#include "Systems/Graphics/Rendering/ParticleRendererSystem.h"
#include "Systems/Particles/ParticleTransformSystem.h"

#include "Systems/Graphics/EventRelinkComponent.h"

namespace Xivi
{
using Notifier_Graphics_SList = SList<>;

using Global_Graphics_SList = SList<EventRelinkComponentSystem>;

using Update_Graphics_SList = SList <
  Particle::ParticleSystem,
  Graphics::LightTransformSystem,
  Graphics::InterpolateSystem,
  Graphics::AnimationStateSystem,
  Graphics::CameraSystem,
  Graphics::CameraCollisionSystem,
  Particle::ParticleTransformSystem,
  Graphics::TransformSystem,
  Graphics::SingleTransformSystem,
  Graphics::FamilyTransformSystem,
  Graphics::UITransformSystem,
  Graphics::RendererSystem,
  Graphics::ShadowRendererSystem,
  Graphics::SkyBoxRendererSystem,
  Graphics::StaticRendererSystem,
  Graphics::AnimationRendererSystem,
  Graphics::NormalRendererSystem,
  Graphics::WireFrameRendererSystem,
  Graphics::ParticleRendererSystem,
  Graphics::UIRendererSystem,
  Graphics::GridRendererSystem,
  Graphics::DebugDrawRendererSystem

> ;
}