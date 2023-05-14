/*****************************************************************
*\file         Declaration.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

namespace Xivi
{
class Window;

namespace Graphics
{
class AnimationModel;
class StaticModel;
class Texture;
}

namespace Resource
{
template <typename... Ts>
class Manager;
}

using RM = Resource::Manager
<   Graphics::AnimationModel,
  Graphics::StaticModel,
  Graphics::Texture
>;

namespace ECS::World
{
struct Instance;
}

namespace GUI
{
class Manager;
}

namespace Audio
{
class AudioManager;
}

namespace GSM
{
class GSManager;
}
}