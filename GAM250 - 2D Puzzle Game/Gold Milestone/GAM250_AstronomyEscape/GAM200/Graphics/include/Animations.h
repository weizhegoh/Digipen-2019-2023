/*****************************************************************
*\file		 Animations.h
*\brief		 Header file declarations for Animations.

*\author(s)   Lee Jun Jie	14 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include <EnginePch.h>
#include <GameTypes.h>
struct FrameData
{
  glm::vec2 offset;
  float frameTime; // frame display time.;
};

struct Animations
{
  std::unordered_map<ObjectState, std::vector<FrameData>> animationList {};
  std::map<ObjectState, float> frameOffset{};
  Animations(const char *);
};