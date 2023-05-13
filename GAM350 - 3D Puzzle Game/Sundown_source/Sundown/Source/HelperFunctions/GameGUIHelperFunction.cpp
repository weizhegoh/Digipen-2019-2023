/*****************************************************************
*\file         GameGUIHelperFunctions.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"
#include "GameGUIHelperFunction.h"

#include "Xivi/ECS/XVECS.h"
#include "NComponents.h"
#include "NEvents.h"

#include "Xivi/Graphics/Manager/GameStateManager.h"
#include "NSystems/GameWorld/Pause/PauseUnpauseSystem.h"
#include "NSystems/GameWorld/GameGUI/InstructionPageSystem.h"
#include "NSystems/GameWorld/GameGUI/HowToPlaySystem.h"

namespace Xivi::Helper
{
  // Get the texture respectively to the number
std::string GetNumberTexture( const i32 number ) noexcept
{
  if ( number < 0 || number > 9 )
    XV_WARN( "Invalid number" );

  std::string str = "Assets/Textures/gameui_number";

  switch ( number )
  {
    case 1:
    str += "1.dds";
    break;
    case 2:
    str += "2.dds";
    break;
    case 3:
    str += "3.dds";
    break;
    case 4:
    str += "4.dds";
    break;
    case 5:
    str += "5.dds";
    break;
    case 6:
    str += "6.dds";
    break;
    case 7:
    str += "7.dds";
    break;
    case 8:
    str += "8.dds";
    break;
    case 9:
    str += "9.dds";
    break;
    default:
    str += "0.dds";
    break;
  }

  return str;
}
std::string GetNumberTextureFont2( const i32 number ) noexcept
{
  if ( number < 0 || number > 9 )
    XV_WARN( "Invalid number" );

  std::string str = "Assets/Textures/Summary_";

  switch ( number )
  {
    case 1:
    str += "1.dds";
    break;
    case 2:
    str += "2.dds";
    break;
    case 3:
    str += "3.dds";
    break;
    case 4:
    str += "4.dds";
    break;
    case 5:
    str += "5.dds";
    break;
    case 6:
    str += "6.dds";
    break;
    case 7:
    str += "7.dds";
    break;
    case 8:
    str += "8.dds";
    break;
    case 9:
    str += "9.dds";
    break;
    default:
    str += "0.dds";
    break;
  }

  return str;
}

// Recursive push UI into stack
void RecursivePushGUI( ECS::World::Instance &world, Entity &entity ) noexcept
{
  Service::Locator::Get<GUI::Manager>().Push( entity );
  if ( world.HaveComponent<Child>( entity ) )
    for ( auto &child : world.GetComponent<Child>( entity ).m_entities )
      RecursivePushGUI( world, child );
}

void ChangeBGM( Audio::AudioManager &audio_mgr, const std::string old_bgm_file, const std::string new_bgm_file ) noexcept
{
  audio_mgr.StopSpecificLoop( old_bgm_file );
  audio_mgr.playLoop( new_bgm_file, false );
}

void RecurPush( ECS::World::Instance &world, GUI::Manager &canvas_stack, const std::string name ) noexcept
{
  ECS::Query::Instance gui_window;
  gui_window.AllOf<Root, GUI::WindowTag>();
  world.ForEach( gui_window, [&]( Entity &entity, Child &childs )
  {
    for ( auto &child : childs.m_entities )
    {
      auto &entry = world.m_entity_manager.Get( child );
      if ( entry.m_name == name )
      {
        RecursivePushGUI( world, child );
        return true;
      }
    }
    return false;
  } );
  canvas_stack.Sort();
}

// Push mainmenu ui into canvas
void PushMainMenuGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept
{
  world.ForEach( [&]( Entity &entity, Child &childs )
  {
    for ( auto &child : childs.m_entities )
    {
      if ( world.HaveComponents<GUI::MainMenuCanvasTag, UI>( child ) )
      {
        RecursivePushGUI( world, child );
        return true;
      }
    }
    return false;
  } );
  canvas_stack.Sort();
}

// Push game ui into canvas
void PushGameGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept
{
  RecurPush( world, canvas_stack, "MoveCountText_instance" );
  RecurPush( world, canvas_stack, "MoveCountNumberOnes_instance" );
  RecurPush( world, canvas_stack, "MoveCountNumberTens_instance" );
  RecurPush( world, canvas_stack, "MoveCountNumberHundreds_instance" );
  RecurPush( world, canvas_stack, "CollectableCountText_instance" );
  RecurPush( world, canvas_stack, "CollectableCountNumber_instance" );
  ECS::Query::Instance game_interatable_popup;
  game_interatable_popup.AllOf<GUI::GameInteratablePopup>();
  world.ForEach( game_interatable_popup, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );
  ECS::Query::Instance game_cheese_ui;
  game_cheese_ui.AllOf<GUI::GUICheeseGameMenu>();
  world.ForEach( game_cheese_ui, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );
  ECS::Query::Instance tutorial_button_prompt;
  tutorial_button_prompt.AllOf<GUI::TutorialButtonUITag>();
  world.ForEach( tutorial_button_prompt, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );

  ECS::Query::Instance dialogue_query;
  dialogue_query.AnyOf<GUI::SwitchPopupDialogue, GUI::PressurePlatePopupDialogue, GUI::CheesePopupDialogue, GUI::HolePopupDialogue, GUI::ObstaclePopupDialogue, GUI::CartPopupDialogue, GUI::LevelNumberGameUITag>();
  world.ForEach( dialogue_query, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );
}

// Push pause ui into canvas
void PushPauseGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept
{
  ECS::Query::Instance pause_canvas;
  pause_canvas.AllOf<GUI::PauseCanvasTag>();
  world.ForEach( pause_canvas, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );
  canvas_stack.Sort();
}

void PushInstructionGUI( ECS::World::Instance &world, GUI::Manager &canvas_stack ) noexcept
{
  // RecurPush(world, canvas_stack, "Instruction_instance");
  ECS::Query::Instance instruction_canvas;
  instruction_canvas.AllOf<GUI::InstructionCanvasTag>();
  world.ForEach( instruction_canvas, [&]( Entity &entity )
  {
    RecursivePushGUI( world, entity );
  } );
  canvas_stack.Sort();
}

void UpdateLevelNumber( Ptr<ECS::World::Instance> world, Ptr<RM> resource_manager, GUI::Manager &canvas_stack, const std::string &level_tex ) noexcept
{
  PushGameGUI( *world, canvas_stack );
  ECS::Query::Instance level;
  level.AllOf<GUI::LevelNumberGameUITag>();
  world->ForEach( level, [&]( Material &material )
  {
    material.diffuse = resource_manager->Get<Graphics::Texture>( "Assets/Textures/" + level_tex );
    material.specular = resource_manager->Get<Graphics::Texture>( "Assets/Textures/" + level_tex );
  } );
}

void UpdateGUI( Ptr<ECS::World::Instance> world, Ptr<RM> resource_manager, unsigned int state, i32 collectables_collected ) noexcept
{
  // Update GUI according to value
  world->ForEach( [&]( Player &player )
  {
    world->SendGlobalEvent<UpdateGUIMoveEvent>( player.moves_total );
    world->SendGlobalEvent<UpdateGUICollectableEvent>( player.collectibles_total );
  } );

  auto &canvas_stack = Service::Locator::Get<GUI::Manager>();
  if ( !canvas_stack.Empty() )
    canvas_stack.PopAll();
  switch ( static_cast<GSM::GS_STATES>( state ) )
  {
    case GSM::MAINMENU:
    case GSM::MAINMENU_NOLOGO:
    Helper::PushMainMenuGUI( *world, canvas_stack );
    break;
    case GSM::LEVEL1_STAGE1:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L1-1.dds" );
      break;
    }
    case GSM::LEVEL1_STAGE2:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L1-2.dds" );
      break;
    }
    case GSM::LEVEL1_STAGE3:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L1-3.dds" );
      break;
    }
    case GSM::LEVEL1_STAGE4:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L1-4.dds" );
      break;
    }
    case GSM::LEVEL2_STAGE1:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L2-1.dds" );
      break;
    }
    case GSM::LEVEL2_STAGE2:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_L2-2.dds" );
      break;
    }
    case GSM::TUTORIAL_1:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_T1-1.dds" );
      break;
    }
    case GSM::TUTORIAL_2:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      UpdateLevelNumber( world, resource_manager, canvas_stack, "GameUI_T1-2.dds" );
      break;
    }
    case GSM::WIN:
    {
      std::string str = "Assets/Textures/";
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::WinCanvasTag>();
      switch ( collectables_collected )
      {
        case 0:
        str += "0star_goal.dds";
        break;
        case 1:
        str += "1star_goal.dds";
        break;
        case 2:
        str += "2star_goal.dds";
        break;
        case 3:
        str += "3star_goal.dds";
        break;
        default:
        break;
      }
      world->ForEach( canvas, [&]( Entity &entity, Material &material )
      {
        material.diffuse = resource_manager->Get<Graphics::Texture>( str );
        material.specular = resource_manager->Get<Graphics::Texture>( str );
        RecursivePushGUI( *world, entity );
      } );
      break;
    }
    case GSM::WIN2:
    {
      #ifndef XV_EDITOR
      auto &gsm = Service::Locator::Get<GSM::GSManager>();

      if ( gsm.collectables_collected == 0 )
        break;

      ECS::Query::Instance not_game_menu_cheese;
      not_game_menu_cheese.NoneOf<GUI::GUICheeseGameMenu>();
      world->ForEach( not_game_menu_cheese, [&]( Entity &entity, GUI::UICheese &cheese, RenderSettings &render_settings )
      {
        render_settings.transparent = gsm.collectables_collected >= cheese.m_id ?
          1.0f :
          0.0f;
      } );
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::WinCanvasTag>();
      world->ForEach( canvas, [&]( Entity &entity )
      {
        RecursivePushGUI( *world, entity );
      } );
      #endif
      break;
    }
    case GSM::GAMEOVER: [[fallthrough]];
    case GSM::GAMEOVER2:
    {
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::CanvasTag, GUI::LoseCanvasTag>();
      world->ForEach( canvas, [&]( Entity &entity )
      {
        RecursivePushGUI( *world, entity );
      } );
      break;
    }
    case GSM::END:
    {
      world->GetSystem<GUIInputSystem>().ShowKeyboardorController();
      ECS::Query::Instance canvas;
      canvas.AllOf<GUI::SummaryCanvasTag>();
      world->ForEach( canvas, [&]( Entity &entity )
      {
        RecursivePushGUI( *world, entity );
      } );
      world->SendGlobalEvent<UpdateGUISummaryEvent>();
      break;
    }
    default:
    break;
  }
}

void RefreshPages( Ptr<ECS::World::Instance> world ) noexcept
{
  world->GetSystem<InstructionPageSystem>().m_cur_pg = 0;
  world->GetSystem<HowToPlaySystem>().m_cur_pg = 0;
}
}