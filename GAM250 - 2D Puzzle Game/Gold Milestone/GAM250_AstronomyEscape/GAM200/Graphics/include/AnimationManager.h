/*****************************************************************
*\file		 AnimationManager.h
*\brief		 Header file declarations for AnimationManager.

*\author(s)   Lee Jun Jie	11 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include "Animations.h"
class AnimationManager
{
  std::unordered_map<TextureType, std::unique_ptr<Animations>> objectAnimations {};
  /*
    // We store a reference to the current animation so we
  // can quickly update and draw it.
    std::pair<AnimationState,
    std::shared_ptr<Animation>> currentAnimation;
    */
public:
  AnimationManager();

  void AddAnimation( const TextureType, const char * );

  Animations *GetObjectAnimations(TextureType);
};