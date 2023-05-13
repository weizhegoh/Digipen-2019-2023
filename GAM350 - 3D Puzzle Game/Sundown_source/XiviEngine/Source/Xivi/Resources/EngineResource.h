/*****************************************************************
*\file         EngineResource.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include <rttr/registration.h>
#include <Resource/ILoader.h>

#include "Xivi/Core/Utils/Filesystem/Filesystem.h"

namespace Xivi::KeyResource::Reflect
{
RTTR_REGISTRATION
{
  rttr::registration::class_<Xivi::Resource::ID>( "ResourceKey" )
  .property( "Key", &Xivi::Resource::ID::m_value );
}
}

#include "Xivi/Graphics/Texture/Texture.h"
#include "TextureLoader.h"

//#include "Xivi/Graphics/Shader/glslshader.h"
//#include "ShaderLoader.h"

#include "Xivi/Graphics/Model/Model.h"
#include "ModelLoader.h"

#include "Xivi/Graphics/Model/Model.h"
#include "AnimationModelLoader.h"