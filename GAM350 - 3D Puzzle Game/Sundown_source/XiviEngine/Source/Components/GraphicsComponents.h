/*****************************************************************
*\file         GraphicsComponents.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include "EngineComponents.h"
#include "Graphics/Direction.h"
#include "Graphics/Mesh_c.h"
#include "Graphics/Transform.h"
#include "Graphics/ChildTransform.h"
#include "Graphics/Colour.h"
#include "Graphics/AABB.h"
#include "Graphics/LightComponent.h"
#include "Graphics/AnimationState.h"
#include "Graphics//Material.h"
#include "Graphics/RenderSetting.h"
#include "Graphics/DebugDraw_c.h"
#include "Graphics/Camera_c.h"
#include "Graphics/AnimatorComponent.h"
#include "../Sundown/Source/Components/Player/PlayerControls.h"
#include "../Sundown/Source/Components/Player/PlayerState.h"
#include "GUI/UI.h"

namespace Xivi
{
DefineTagComponent( ChangeAnimation );
DefineTagComponent( CameraInitalized );
DefineTagComponent( CameraSetTarget );
DefineTagComponent( CameraAutoOrbit );
DefineTagComponent( AnimatorInitalized );
DefineTagComponent( CutSceneUI );

using Graphics_CList = CList <
  AnimationState,
  AnimationMeshComponent,
  StaticMeshComponent,
  LightProperty,
  QuadraticFunction,
  SpotLight_cutOff,
  Material,
  SkyBoxMaterial,
  AABB,
  Colour,
  Transform,
  //ChildTransform,
  CameraSetTarget,
  CameraInitalized,
  CameraAutoOrbit,
  AnimatorInitalized,
  CutSceneUI,
  Camera_c,
  Direction,
  RenderSettings,
  DebugDrawComponent,
  AnimatorComponent,
  ChangeAnimation,
  AnimationUpdateOnce,
  LightData,
  GlobalAmbient//,
  /*BrightnessContrast*/ > ;

//SkyBox shader
using SkyBoxType = CList<Transform, StaticMeshComponent, SkyBoxMaterial, RenderSettings>;

//Animation Shader
using AnimationRenderType = CList<Position, Transform, AnimationMeshComponent, AnimationState, AnimatorComponent, AnimatorInitalized, Material, Colour, AABB, RenderSettings, Direction, LightData>;

//Using Texture shader
using StaticRenderType = CList<Position, Transform, StaticMeshComponent, Material, Colour, AABB, RenderSettings, Direction, LightData>;

using ColliderBoxType = CList<Position, Transform, StaticMeshComponent, Colour, AABB, RenderSettings, DebugDrawComponent>;

using UIType = CList<Position, Transform, StaticMeshComponent, Colour, AABB, RenderSettings, DebugDrawComponent, UI, Material>;

//Light Type, render using normal shader
using LightType = CList<Position, Transform, StaticMeshComponent, Colour, LightProperty, QuadraticFunction, RenderSettings, DebugDrawComponent>;
using DirLightType = CList<Position, Transform, StaticMeshComponent, Colour, LightProperty, Direction, RenderSettings, DebugDrawComponent>;
using SpotLightType = CList<LightProperty, QuadraticFunction, SpotLight_cutOff, RenderSettings>;

using MainCharacterType = CList<
  Position,
  Transform,
  AnimationMeshComponent,
  AnimationState,
  AnimatorComponent,
  Material,
  Colour,
  RenderSettings,
  Direction,
  //RigidBody,
  PlayerControls,
  PlayerState,
  AABB,
  Camera_c>;

using CloneCharacterType = CList<
  Position,
  Transform,
  AnimationMeshComponent,
  AnimatorComponent,
  Material,
  Colour,
  RenderSettings,
  Direction,
  Xivi::AABB>;

using StaticClone = CList<
  Position,
  Transform,
  AnimationMeshComponent,
  AnimationState,
  AnimatorComponent,
  Material,
  Colour,
  RenderSettings,
  Direction,
  //RigidBody,
  PlayerControls,
  PlayerState,
  AABB>;
}