/*****************************************************************
*\file         GUIInputSystem.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "XiviEngine.h"
#include "GUIInputSystem.h"
#include "HelperFunctions/GameGUIHelperFunction.h"
#include "NEvents.h"
#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "NSystems/GameWorld/Pause/PauseUnpauseSystem.h"

namespace Xivi
{
void GUIInputSystem::OnCreate() noexcept
{
  m_canvas_stack = &Service::Locator::Get<GUI::Manager>();
  m_resource_manager = &Service::Locator::Get<RM>();
  m_keyboard = Service::Locator::Get<Input::Manager>().Keyboard();
}

void SkipCutScene( const bool pressed_skip ) noexcept
{
  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &cs = gsm.cutSceneFunc;
  if ( cs.isActive && pressed_skip )
    gsm.cutSceneFunc.SkipScene();
  #endif
}

void Pause( ECS::World::Instance &world ) noexcept
{
  #ifndef XV_EDITOR
  auto &gsm = Service::Locator::Get<GSM::GSManager>();
  auto &cs = gsm.cutSceneFunc;
  // If cutscene not playing, pause when press escape
  if ( !cs.isActive )
    world.SendGlobalEvent<PauseUnpauseEvent>();
  #else
  world.SendGlobalEvent<PauseUnpauseEvent>();
  #endif
}

void BackToMainMenuOrWinLoseScreen( Ptr<GUIInputSystem> param_this ) noexcept
{
  // Return to main menu
  param_this->SendEvent<UpdateGUIBackToMenuEvent>( param_this );

  // Return to win/lose screen from popup to main menu
  param_this->SendEvent<UpdateGUIBackToWinLoseEvent>( param_this );
}

void UpdateButtonTexture( ECS::World::Instance &world,
                          Ptr<GUI::Manager> canvas_stack,
                          Ptr<RM> resource_manager,
                          const std::string &entity_name,
                          const std::string &texture_normal,
                          const std::string &texture_pressed,
                          const bool triggered,
                          const bool released ) noexcept
{
  Entity button = canvas_stack->GetGUIByName( entity_name );
  if ( !button.Valid() )
    return;

  if ( triggered )
    world.GetEntity( button, [&]( Material &material )
  {
    material.diffuse = resource_manager->Get<Graphics::Texture>( texture_pressed );
    material.specular = resource_manager->Get<Graphics::Texture>( texture_pressed );
  } );
  else if ( released )
    world.GetEntity( button, [&]( Material &material )
  {
    material.diffuse = resource_manager->Get<Graphics::Texture>( texture_normal );
    material.specular = resource_manager->Get<Graphics::Texture>( texture_normal );
  } );
}

void UpdateControllerOrKeyboard( ECS::World::Instance &world,
                                 Ptr<GUI::Manager> canvas_stack,
                                 Ptr<RM> resource_manager,
                                 const std::string &entity_name,
                                 const std::string &texture_keyboard,
                                 const std::string &texture_controller,
                                 const bool controller ) noexcept
{
  UpdateButtonTexture( world, canvas_stack, resource_manager, entity_name, texture_keyboard, texture_controller, controller, !controller );
}

void UpdateAllButtons( ECS::World::Instance &world,
                       Ptr<GUI::Manager> canvas_stack,
                       Ptr<RM> resource_manager,
                       const bool controller ) noexcept
{
  UpdateControllerOrKeyboard( world, canvas_stack, resource_manager, "F button_instance", "Assets/Textures/F_Normal.dds", "Assets/Textures/West_Normal.dds", controller );
  /*UpdateControllerOrKeyboard(world, canvas_stack, resource_manager, "A button_instance", "Assets/Textures/A_normal.dds", "Assets/Textures/BGM_1.dds", controller);
  UpdateControllerOrKeyboard(world, canvas_stack, resource_manager, "D button_instance", "Assets/Textures/D_Normal.dds", "Assets/Textures/Pause_1.dds", controller);
  UpdateControllerOrKeyboard(world, canvas_stack, resource_manager, "W button_instance", "Assets/Textures/W_norms.dds", "Assets/Textures/SelectArrow.dds", controller);
  UpdateControllerOrKeyboard(world, canvas_stack, resource_manager, "S button_instance", "Assets/Textures/S_Norms.dds", "Assets/Textures/JohnRat.dds", controller);*/
}

void GUIInputSystem::ShowKeyboardorController() noexcept
{
  ECS::Query::Instance keyboard_query;
  keyboard_query.AllOf<GUI::KeyboardButtonUITag>();
  ECS::Query::Instance controller_query;
  controller_query.AllOf<GUI::ControllerButtonUITag>();

  ECS::Query::Instance keyboard_cutscene_query;
  keyboard_cutscene_query.AllOf<GUI::KeyboardButtonUITag, CutSceneUI>();
  ECS::Query::Instance controller_cutscene_query;
  controller_cutscene_query.AllOf<GUI::ControllerButtonUITag, CutSceneUI>();

  if ( m_is_controller )
  {
    ForEach( keyboard_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 0.0f;
    } );
    ForEach( controller_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 1.0f;
    } );

    ForEach( keyboard_cutscene_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 0.0f;
    } );
    ForEach( controller_cutscene_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 1.0f;
    } );
  }
  else
  {
    ForEach( keyboard_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 1.0f;
    } );
    ForEach( controller_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 0.0f;
    } );

    ForEach( keyboard_cutscene_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 1.0f;
    } );
    ForEach( controller_cutscene_query, [&]( RenderSettings &render_settings )
    {
      render_settings.transparent = 0.0f;
    } );
  }
}

void GUIInputSystem::OnUpdate() noexcept
{
  if ( m_paused )
    return;

  static bool previous_controller = false;

// ------------------------------------------ Controller ------------------------------------------ //
  auto device1 = Service::Locator::Get<Input::Manager>().GetDevice( "Gamepad[0]" );
  bool triggered_interactable = false;
  bool released_interactable = false;
  bool triggered_viewgoal = false;
  bool released_viewgoal = false;
  bool triggered_walk_front = false;
  bool released_walk_front = false;
  bool triggered_walk_back = false;
  bool released_walk_back = false;
  bool triggered_rot_left = false;
  bool released_rot_left = false;
  bool triggered_rot_right = false;
  bool released_rot_right = false;
  bool triggered_cam_rot_right = false;
  bool released_cam_rot_right = false;
  bool triggered_cam_rot_left = false;
  bool released_cam_rot_left = false;
  bool triggered_select = false;
  if ( device1 )
  {
    auto &gamepad = *static_cast<Ptr<Input::Device::Gamepad>>( device1 );

    if ( gamepad.North().Triggered() || gamepad.South().Triggered() || gamepad.East().Triggered() || gamepad.West().Triggered() ||
         gamepad.DPadUp().Triggered() || gamepad.DPadDown().Triggered() || gamepad.DPadLeft().Triggered() || gamepad.DPadRight().Triggered() ||
         gamepad.LeftShoulder().Triggered() || gamepad.RightShoulder().Triggered() )
    {
      m_is_controller = true;
      UpdateAllButtons( m_world, m_canvas_stack, m_resource_manager, m_is_controller );
      ShowKeyboardorController();
    }

    /*if (gamepad.AnyKey().Value() && !m_is_controller)
      m_is_controller = true;*/
    // If press 'O'(PS4) or 'B'
    if ( gamepad.East().Triggered() )
    {
      BackToMainMenuOrWinLoseScreen( this );
      if ( GetSystem<PauseUnpauseSystem>().m_paused )
        Pause( m_world );
    }

    if ( gamepad.DPadUp().Triggered() )
      triggered_walk_front = true;
    else if ( gamepad.DPadUp().Released() )
      released_walk_front = true;

    if ( gamepad.DPadDown().Triggered() )
      triggered_walk_back = true;
    else if ( gamepad.DPadDown().Released() )
      released_walk_back = true;

    if ( gamepad.DPadLeft().Triggered() )
      triggered_rot_left = true;
    else if ( gamepad.DPadLeft().Released() )
      released_rot_left = true;

    if ( gamepad.DPadRight().Triggered() )
      triggered_rot_right = true;
    else if ( gamepad.DPadRight().Released() )
      released_rot_right = true;

    if ( gamepad.West().Triggered() )
      triggered_interactable = true;
    else if ( gamepad.West().Released() )
      released_interactable = true;

    if ( gamepad.North().Triggered() )
      triggered_viewgoal = true;
    else if ( gamepad.North().Released() )
      released_viewgoal = true;

    if ( gamepad.LeftShoulder().Triggered() )
      triggered_cam_rot_left = true;
    else if ( gamepad.LeftShoulder().Released() )
      released_cam_rot_left = true;

    if ( gamepad.RightShoulder().Triggered() )
      triggered_cam_rot_right = true;
    else if ( gamepad.RightShoulder().Released() )
      released_cam_rot_right = true;

    // If press 'Option'(PS4) or 'Start'
    if ( gamepad.Start().Triggered() )
    {
      Pause( m_world );
      m_is_controller = true;
      UpdateAllButtons( m_world, m_canvas_stack, m_resource_manager, m_is_controller );
      ShowKeyboardorController();
    }

    if ( gamepad.DPadLeft().Triggered() || gamepad.DPadRight().Triggered() )
    {
      SendEvent<UpdateGUIInstructionPageEvent>( this, gamepad.DPadLeft().Triggered(), gamepad.DPadRight().Triggered() );
      SendEvent<UpdateGUIHowToPlayPageEvent>( this, gamepad.DPadLeft().Triggered(), gamepad.DPadRight().Triggered() );
    }

    triggered_select = gamepad.South().Triggered();
  }

// ----------------------------------------- Keyboard ----------------------------------------- //
    /*if (m_keyboard->AnyKey().Value() && m_is_controller)
      m_is_controller = false;*/

  if ( m_keyboard->W().Triggered() || m_keyboard->A().Triggered() || m_keyboard->S().Triggered() || m_keyboard->D().Triggered() ||
       m_keyboard->Tab().Triggered() || m_keyboard->F().Triggered() || m_keyboard->Enter().Triggered() || m_keyboard->Space().Triggered() ||
       m_keyboard->Left().Triggered() || m_keyboard->Right().Triggered() || m_keyboard->Up().Triggered() || m_keyboard->Down().Triggered() )
  {
    m_is_controller = false;
    UpdateAllButtons( m_world, m_canvas_stack, m_resource_manager, m_is_controller );
    ShowKeyboardorController();
  }

  // Triggered Escape Key
  if ( m_keyboard->Escape().Triggered() )
  {
    Pause( m_world );
    BackToMainMenuOrWinLoseScreen( this );
    m_is_controller = false;
    UpdateAllButtons( m_world, m_canvas_stack, m_resource_manager, m_is_controller );
    ShowKeyboardorController();
  }

  if ( m_keyboard->Left().Triggered() || m_keyboard->Right().Triggered() )
  {
    SendEvent<UpdateGUIInstructionPageEvent>( this, m_keyboard->Left().Triggered(), m_keyboard->Right().Triggered() );
    SendEvent<UpdateGUIHowToPlayPageEvent>( this, m_keyboard->Left().Triggered(), m_keyboard->Right().Triggered() );
  }

  if ( !m_is_controller )
  {
    if ( m_keyboard->F().Triggered() )
    {
      triggered_interactable = true;
    }
    else if ( m_keyboard->F().Released() )
      released_interactable = true;

    if ( m_keyboard->Tab().Triggered() )
      triggered_viewgoal = true;
    else if ( m_keyboard->Tab().Released() )
      released_viewgoal = true;

    if ( m_keyboard->W().Triggered() )
      triggered_walk_front = true;
    else if ( m_keyboard->W().Released() )
      released_walk_front = true;

    if ( m_keyboard->S().Triggered() )
      triggered_walk_back = true;
    else if ( m_keyboard->S().Released() )
      released_walk_back = true;

    if ( m_keyboard->A().Triggered() )
      triggered_rot_left = true;
    else if ( m_keyboard->A().Released() )
      released_rot_left = true;

    if ( m_keyboard->D().Triggered() )
      triggered_rot_right = true;
    else if ( m_keyboard->D().Released() )
      released_rot_right = true;

    if ( m_keyboard->Left().Triggered() )
      triggered_cam_rot_left = true;
    else if ( m_keyboard->Left().Released() )
      released_cam_rot_left = true;

    if ( m_keyboard->Right().Triggered() )
      triggered_cam_rot_right = true;
    else if ( m_keyboard->Right().Released() )
      released_cam_rot_right = true;
  }

  if ( triggered_interactable || released_interactable )
  {
    std::string texutre_str_normal, texutre_str_triggered;
    texutre_str_normal = m_is_controller ?
      "Assets/Textures/West_Normal.dds" :
      "Assets/Textures/F_Normal.dds";

    texutre_str_triggered = m_is_controller ?
      "Assets/Textures/West_Pressed.dds" :
      "Assets/Textures/F_pressed.dds";

    // Update F key button ui texture
    UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "F button_instance", texutre_str_normal, texutre_str_triggered, triggered_interactable, released_interactable );
  }

  if ( !m_is_controller )
  {
    if ( triggered_interactable || released_interactable )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "F button1_instance", "Assets/Textures/F_Normal.dds", "Assets/Textures/F_pressed.dds", triggered_interactable, released_interactable );
    if ( triggered_viewgoal || released_viewgoal )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "Tab button_instance", "Assets/Textures/Tab_Normal.dds", "Assets/Textures/Tab_Pressed.dds", triggered_viewgoal, released_viewgoal );
    if ( triggered_walk_front || released_walk_front )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "W button_instance", "Assets/Textures/W_norms.dds", "Assets/Textures/W_pressed.dds", triggered_walk_front, released_walk_front );
    if ( triggered_walk_back || released_walk_back )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "S button_instance", "Assets/Textures/S_Norms.dds", "Assets/Textures/S_Pressed.dds", triggered_walk_back, released_walk_back );
    if ( triggered_rot_left || released_rot_left )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "A button_instance", "Assets/Textures/A_normal.dds", "Assets/Textures/A_Pressed.dds", triggered_rot_left, released_rot_left );
    if ( triggered_rot_right || released_rot_right )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "D button_instance", "Assets/Textures/D_Normal.dds", "Assets/Textures/D_Pressed.dds", triggered_rot_right, released_rot_right );
    if ( triggered_cam_rot_left || released_cam_rot_left )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "ArrowLeft button_instance", "Assets/Textures/LeftArrowNormal.dds", "Assets/Textures/LeftArrowPressed.dds", triggered_cam_rot_left, released_cam_rot_left );
    if ( triggered_cam_rot_right || released_cam_rot_right )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, "ArrowRight button_instance", "Assets/Textures/RightArrowNormal.dds", "Assets/Textures/RightArrowPressed.dds", triggered_cam_rot_right, released_cam_rot_right );
  }
  else
  {
    static const std::string dpad = "ControllerDPadPrompt_instance";
    static const std::string control_button = "ControllerButtonPrompt_instance";
    static const std::string control_shoulder = "ControllerShoulderPrompt_instance";
    static const std::string control_button_normal_tex = "Assets/Textures/ControllerBtn.dds";
    static const std::string control_dpad_normal_tex = "Assets/Textures/Dpad.dds";
    static const std::string control_shoulder_normal_tex = "Assets/Textures/Controller_Cam.dds";
    if ( triggered_interactable || released_interactable )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, control_button, control_button_normal_tex, "Assets/Textures/InteractPressed.dds", triggered_interactable, released_interactable );
    if ( triggered_viewgoal || released_viewgoal )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, control_button, control_button_normal_tex, "Assets/Textures/viewgoalpressed.dds", triggered_viewgoal, released_viewgoal );
    if ( triggered_walk_front || released_walk_front )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, dpad, control_dpad_normal_tex, "Assets/Textures/Dpad_forward.dds", triggered_walk_front, released_walk_front );
    if ( triggered_walk_back || released_walk_back )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, dpad, control_dpad_normal_tex, "Assets/Textures/DpadBackwards.dds", triggered_walk_back, released_walk_back );
    if ( triggered_rot_left || released_rot_left )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, dpad, control_dpad_normal_tex, "Assets/Textures/Dpad_left.dds", triggered_rot_left, released_rot_left );
    if ( triggered_rot_right || released_rot_right )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, dpad, control_dpad_normal_tex, "Assets/Textures/Dpad_right.dds", triggered_rot_right, released_rot_right );
    if ( triggered_cam_rot_left || released_cam_rot_left )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, control_shoulder, control_shoulder_normal_tex, "Assets/Textures/Controller_L_Pressed.dds", triggered_cam_rot_left, released_cam_rot_left );
    if ( triggered_cam_rot_right || released_cam_rot_right )
      UpdateButtonTexture( m_world, m_canvas_stack, m_resource_manager, control_shoulder, control_shoulder_normal_tex, "Assets/Textures/Controller_R_Pressed.dds", triggered_cam_rot_right, released_cam_rot_right );
  }

  if ( triggered_walk_front || triggered_walk_back )
  {
    SendGlobalEvent<CloseHolePopupDialogue>();
    SendGlobalEvent<CloseObstaclePopupDialogue>();
  }
  if ( triggered_interactable )
  {
    //SendGlobalEvent<CloseCartPopupDialogue>();
    //SendGlobalEvent<ClosePressurePlatePopupDialogue>();
    //SendGlobalEvent<CloseSwitchPopupDialogue>();
    //SendGlobalEvent<CloseCartPopupDialogue>();
  }

  // UI button movement
  SendEvent<UIButtonMovementEvent>( this );

  // Skip cutscene
  SkipCutScene( m_keyboard->Enter().Triggered() || m_keyboard->Escape().Triggered() || triggered_select );
}
}