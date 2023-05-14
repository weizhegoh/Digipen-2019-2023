/*****************************************************************
*\file		 AnimationManager.cpp
*\brief		 AnimationManager functions definitions.

*\author(s)   Lee Jun Jie	12 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "AnimationManager.h"

AnimationManager::AnimationManager(){}

void AnimationManager::AddAnimation( const TextureType objType,
                                     const char *pFileName )
{
  objectAnimations[objType] = std::make_unique<Animations>(pFileName);
}

Animations* AnimationManager::GetObjectAnimations(TextureType objType )
{
  return objectAnimations[objType].get();
}