/*****************************************************************
*\file         GameGUIHelperFunctions.h
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

#include "XiviEngine.h"
#include "Xivi/Core/Declaration.h"

namespace Xivi::Helper
{
// Get the texture of the number
std::string GetNumberTexture( const i32 ) noexcept;
std::string GetNumberTextureFont2( const i32 number ) noexcept;
// Push into canvas stack using entity
void RecursivePushGUI( ECS::World::Instance &, Entity & ) noexcept;
// Change BGM
void ChangeBGM( Audio::AudioManager &audio_mgr, const std::string old_bgm_file, const std::string new_bgm_file ) noexcept;
// Push into canvas stack using entity's name
void RecurPush( ECS::World::Instance &world, GUI::Manager &canvas_stack, const std::string name ) noexcept;
// Push mainmenu ui into canvas
void PushMainMenuGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept;
// Push game ui into canvas
void PushGameGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept;
// Push pause ui into canvas
void PushPauseGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept;
// Push instruction ui into canvas
void PushInstructionGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept;
// Update GUI according to level
void UpdateGUI( Ptr<ECS::World::Instance> world, Ptr<RM> resource_manager, unsigned int states, i32 collectables_collected ) noexcept;
// Refresh Pages
void RefreshPages( Ptr<ECS::World::Instance> world ) noexcept;
}