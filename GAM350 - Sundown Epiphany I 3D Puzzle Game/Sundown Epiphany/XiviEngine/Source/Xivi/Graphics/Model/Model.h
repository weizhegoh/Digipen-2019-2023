/*****************************************************************
*\file         Model.h
*\author(s)    Lee Liang Ping

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "Mesh.h"
#include "Animation/AnimationModel.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "../Gfx/Gfx.h"
namespace Xivi::Graphics
{
class StaticModel
{
public:
  StaticModel() = default;
  StaticModel( const std::string );
  ~StaticModel();
  std::vector<Mesh> mesh;
  Math::Vector3 unitScales;
  bool hasTanBi = false;
  void Draw();
};

class AnimationModel
{
public:
  AnimationModel() = default;
  AnimationModel( const std::string );
  ~AnimationModel();
  //void operator=(const std::string);
  AnimatedModel model;
  Animation animation;
  Math::Vector3 unitScales = Math::Vector3( 1.f );
  //Animator animatior;
};

using AnimationModelPtr = AnimationModel *;
using StaticModelPtr = StaticModel *;
}
