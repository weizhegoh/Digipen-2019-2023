/*****************************************************************
*\file         XiviEngine.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once

#include "Xivi/Core/Application.h" // Core, Config, XVECS

#include "Xivi/Graphics/Model/Model.h"
#include "Xivi/Graphics/Texture/Texture.h"
#include "Xivi/Graphics/Model/Animation/Animation.h"
#include <Resource/Manager.h>
#include "Events/EngineEvents.h"
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Components/GUIComponents.h"
#include "Components/AudioComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include "Xivi/GUI/GUIManager.h"
#include "Systems/Audio/Audio.h"

#include "Xivi/Platform/Windows/GLFW/GLFW_Window.h"
#include "Events/GUIEvents.h"
