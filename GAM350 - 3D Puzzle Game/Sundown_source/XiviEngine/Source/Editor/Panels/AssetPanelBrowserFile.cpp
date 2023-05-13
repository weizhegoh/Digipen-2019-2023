/*****************************************************************
*\file         AssetPanelBrowserFile.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "AssetPanelBrowser.h"

#include <IconsFontAwesome5.h>
#include "Components/EngineComponents.h"
#include "Components/GraphicsComponents.h"
#include "Xivi/Graphics/Manager/GraphicManagers.h"
#include <Resource/Manager.h>
#include "Xivi/Core/Declaration.h"
#include "Editor/ImGui/Helper/ImGuiHelperFunction.h"

namespace Xivi
{
using ArchetypeCreateFunc = Func<Entity( const Position & )>;
using AssetCreateFunc = Func<Entity( const Position &, const std::string & )>;
}

namespace Xivi::Editor
{
using Icon = const char *;
using Extension = std::string;
std::unordered_map<Extension, Icon> g_label_map;
AssetCreateFunc g_animation_create_func;
AssetCreateFunc g_static_create_function_create_func;
Ptr<RM> g_resource_manager;

enum class PopUpType
{
  ConfirmationReplace,
  ConfirmationDelete,
  ConfirmationCut,
  CreateFolder,
  DeleteFolder,
};

Filesystem::fs::path copied;
Filesystem::fs::path to_delete;
PopUpType m_popup_type;
bool m_popup { false };
bool open_file { false };
char folder_to_create[MAX_PATH];

void PrintText( const std::string &str ) noexcept
{
  static const auto size_of_text = 12;

  char text[size_of_text] = "\0";

  auto i = 0;
  if ( str.size() > size_of_text - 4 )
  {
    for ( ; i < size_of_text - 4; ++i )
      text[i] = str[i];
    for ( ; i < size_of_text - 1; ++i )
      text[i] = '.';
  }
  else
  {
    for ( ; i < str.size(); ++i )
      text[i] = str[i];
  }
  text[i] = '\0';

  ImGui::TextWrapped( text );
}

void PasteAsset( Ptr<FileContext> file_context ) noexcept
{
  if ( !copied.empty() )
  {
    auto dest = file_context->selection.path() / copied.filename();
    if ( Filesystem::file_exists( dest ) )
    {
      m_popup_type = PopUpType::ConfirmationReplace;
      m_popup = true;
    }
    else
    {
      Filesystem::copy_file( copied, dest );
      file_context->selection_file = dest.generic_string();
    }
  }
}

void CopyAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept
{
  if ( file_context->selection_file.empty() )
    return;
  if ( Filesystem::file_exists( copied ) )
    Filesystem::delete_file( copied );
  auto dest = Filesystem::temp_dir() / file.filename();
  Filesystem::copy_file( file, dest );
  copied = dest;
}

void CutAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept
{
  if ( file_context->selection_file.empty() )
    return;
  to_delete = file;
  m_popup_type = PopUpType::ConfirmationCut;
  m_popup = true;
}

void DeleteAsset( const Filesystem::fs::path file, Ptr<FileContext> file_context ) noexcept
{
  if ( file_context->selection_file.empty() )
    return;
  to_delete = file;
  m_popup_type = PopUpType::ConfirmationDelete;
  m_popup = true;
}

void FileBrowserInit( ECS::World::Instance &world )noexcept
{
  g_resource_manager = &Service::Locator::Get<RM>();

  g_label_map[".wav"] = ICON_FA_MUSIC;
  g_label_map[".ogg"] = ICON_FA_MUSIC;
  g_label_map[".mp3"] = ICON_FA_MUSIC;
  g_label_map[".bank"] = ICON_FA_BOLD;
  g_label_map[".ttf"] = ICON_FA_FONT;
  g_label_map[".ini"] = ICON_FA_FOLDER_MINUS;
  g_label_map[".frag"] = ICON_FA_FILE;
  g_label_map[".vert"] = ICON_FA_FILE;
  g_label_map[".dat"] = ICON_FA_FILE;
  g_label_map[".obj"] = ICON_FA_FILE;
  g_label_map[".json"] = ICON_FA_FILE_CODE;
  g_label_map[".xv"] = ICON_FA_FILE_CODE;
  g_label_map[".cfg"] = ICON_FA_FILE_CODE;
  g_label_map[".prefab"] = ICON_FA_FILE_CODE;
  g_label_map[".BinaryObj"] = ICON_FA_CUBE;
  g_label_map[".BinaryAnimated"] = ICON_FA_CUBE;
  g_label_map[".BinaryStatic"] = ICON_FA_CUBE;
  g_label_map[".msh"] = ICON_FA_SQUARE_FULL;
  g_label_map[".txt"] = ICON_FA_FILE_ALT;
  g_label_map[".log"] = ICON_FA_FILE_ALT;
  g_label_map[".xvlog"] = ICON_FA_FILE_ALT;

  g_animation_create_func = [&]( const Position &position, const std::string &asset )
  {
    auto &arch = world.GetOrCreateArchetype<AnimationRenderType>( "Assets Animation Mesh" );
    auto entity = arch.CreateEntity( [&]( Position &pos, Transform &xform, AnimatorComponent &animator, AnimationMeshComponent &anim, Material &material, Colour &colour, AABB &aabb, RenderSettings &renderSettings )
    {
      pos = position;
      xform.scale = Vec3f { 50.f, 50.f, 50.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      material.diffuse = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.specular = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.normal = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = Service::Locator::Get<RM>().Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      anim.model = g_resource_manager->Get<Graphics::AnimationModel>( asset );
      anim.unitScales = anim.model->unitScales;
      animator.animator.PlayAnimation( &anim.model->animation );
      material.shinness = 128.f;
      renderSettings.Defualt();
    } );
    auto &entry = world.m_entity_manager.Get( entity );
    entry.m_name = asset;
    return entity;
  };

  g_static_create_function_create_func = [&]( const Position &position, const std::string &asset )
  {
    auto &arch = world.GetOrCreateArchetype<StaticRenderType>( "Assets Static Mesh" );
    auto entity = arch.CreateEntity( [&]( Position &pos, Transform &xform, StaticMeshComponent &mesh, Material &material, Colour &colour, AABB &aabb, RenderSettings &renderSettings )
    {
      pos = position;
      xform.scale = Vec3f { 50.f, 50.f, 50.f };
      aabb.AABB_mMin = 0;
      aabb.AABB_mMax = 0;
      aabb.type = CollisionType::AABB;
      colour.colour = Vec3f { 1.f, 1.f, 1.f };
      material.diffuse = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.specular = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.normal = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      material.bump = g_resource_manager->Get<Graphics::Texture>( "Assets/Textures/default.dds" );
      mesh.model = g_resource_manager->Get<Graphics::StaticModel>( asset );
      mesh.unitScales = mesh.model->unitScales;
      material.shinness = 128.f;
      renderSettings.Defualt();
    } );
    auto &entry = world.m_entity_manager.Get( entity );
    entry.m_name = asset;
    return entity;
  };
}

const char *GetIcon( const std::string &str )
{
  auto it = g_label_map.find( str );
  if ( it != g_label_map.end() )
    return it->second;
  return ICON_FA_EXCLAMATION_CIRCLE;
}

void RenderDirectory( const std::vector<Utils::Directory> &sub_dir,
                      ImGuiTextFilter &filter,
                      const ImVec2 &button_size,
                      std::filesystem::directory_entry &selection,
                      Ptr<const std::vector<Utils::Directory>> &dir_list,
                      Ptr < const std::vector<Filesystem::fs::path>> &file_list ) noexcept
{
  for ( auto &ref : sub_dir )
    if ( filter.PassFilter( ref.cur_dir.path().filename().generic_string().c_str() ) )
    {
      auto str = ref.cur_dir.path().filename().generic_string();
      ImGui::TableNextColumn();
      ImGui::PushID( ref.cur_dir.path().c_str() );
      ImGuiHelper::Group( [&]
      {
        ImGui::Button( ICON_FA_FOLDER, button_size );
        PrintText( ref.cur_dir.path().filename().generic_string() );
        //ImGui::TextWrapped( ref.cur_dir.path().filename().generic_string().c_str() );
      } );
      // Hovored behaviour
      if ( ImGuiHelper::IsItemHovered() )
        ImGuiHelper::Tooltip( [&]
      {
        ImGui::Text( ref.cur_dir.path().generic_string().c_str() );
      } );
    // Double clicked behaviour
      ImGuiHelper::ItemDoubleClicked( [&]
      {
        selection = ref.cur_dir;
        dir_list = &ref.sub_dir;
        file_list = &ref.file_vec;
        filter.Clear();
      } );
      // Right click context menu behaviour
      ImGuiHelper::PopupContextItem( "folder context menu", ImGuiPopupFlags_MouseButtonRight, [&]
      {
        if ( ImGui::Selectable( "Delete Folder" ) )
        {
          if ( Filesystem::fs::is_empty( ref.cur_dir.path() ) )
            Filesystem::delete_dir( ref.cur_dir.path() );
          else
          {
            m_popup_type = PopUpType::DeleteFolder;
            m_popup = true;
          }
        }
      } );

      ImGui::PopID();
    }
}

void RenderFile( const std::vector<Filesystem::fs::path> &file_vec,
                 ImGuiTextFilter &filter,
                 const ImVec2 &button_size,
                 Ptr<FileContext> &file_context ) noexcept
{
  for ( auto &ref : file_vec )
    if ( filter.PassFilter( ref.filename().generic_string().c_str() ) )
    {
      auto ref_str = ref.generic_string();
      ImGui::TableNextColumn();
      ImGui::PushID( ref_str.c_str() );
      ImGuiHelper::Group( [&]
      {
        if ( ref.extension() == ".dds" )
        {
          std::string str_path { ref.generic_string() };
          str_path = str_path.substr( str_path.find( "Assets" ) );
          auto texture_asset = g_resource_manager->Get<Graphics::Texture>( str_path );
          ImGui::ImageButton( reinterpret_cast<void *>( static_cast<u64>( texture_asset->pTex ) ),
                              button_size, { 0, 0 }, { 1, 1 }, 0 );
        }
        else
        {
          bool selected = file_context->selection_file == ref.generic_string();
          if ( selected )
          {
            ImGui::IsWindowFocused( ImGuiFocusedFlags_RootAndChildWindows ) ?
              ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 1.00f, 0.55f, 0.33f, 0.90f ) ) :
              ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.50f, 0.50f, 0.50f, 0.85f ) );
          }
          ImGui::Button( GetIcon( ref.extension().generic_string() ), button_size );
          if ( selected )
          {
            ImGui::PopStyleColor();
          }
        }
        PrintText( ref.filename().generic_string() );

        //ImGui::TextWrapped( ref.filename().generic_string().c_str() );
      } );

      if ( ImGuiHelper::IsItemHovered() )
        ImGuiHelper::Tooltip( [&]
      {
        ImGui::Text( ref_str.c_str() );
      } );
      if ( ImGui::IsItemClicked() )
        file_context->selection_file = ref.generic_string();
      ImGuiHelper::ItemDoubleClicked( [&]
      {
        open_file = true;
        file_context->selection_file = ref.generic_string();
      } );
      ImGuiHelper::PopupContextItem( "folder context menu", ImGuiPopupFlags_MouseButtonRight, [&]
      {
        file_context->selection_file = ref.generic_string();
        if ( ImGui::Selectable( "Delete file" ) )
          DeleteAsset( ref, file_context );
        else if ( ImGui::Selectable( "Copy file" ) )
          CopyAsset( ref, file_context );
        else if ( ImGui::Selectable( "Cut file" ) )
          CutAsset( ref, file_context );
      } );

      ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
        ImGuiDragDropFlags_SourceAllowNullID;

      ImGuiHelper::DragDropSource( src_flags, [&]
      {
        auto &selection_file = file_context->selection_file;
        file_context->selection_file.assign( ref_str );
        if ( ref.extension() == ".prefab" )
          ImGui::SetDragDropPayload( "CreateFromAssetToPrefab", &selection_file, sizeof( std::string ) );
        else
        {
          selection_file = selection_file.substr( selection_file.find( "Assets" ) );
          if ( ref.extension() == ".BinaryAnimated" )
            ImGui::SetDragDropPayload( "CreateFromAsset", &g_animation_create_func, sizeof( AssetCreateFunc ) );
          else if ( ref.extension() == ".BinaryObj" || ref.extension() == ".BinaryStatic" )
            ImGui::SetDragDropPayload( "CreateFromAsset", &g_static_create_function_create_func, sizeof( AssetCreateFunc ) );
          else if ( ref.extension() == ".dds" || ref.extension() == ".png" )
            ImGui::SetDragDropPayload( "UpdateTexture", &selection_file, sizeof( std::string ) );
          else if ( ref.extension() == ".mp3" || ref.extension() == ".wav" || ref.extension() == ".ogg" )
            ImGui::SetDragDropPayload( "CreateFromAssetAudio", &selection_file, sizeof( std::string ) );
        }
        ImGui::Text( ref.filename().generic_string().c_str() );
      } );
      ImGui::PopID();
    }
}

void ConfirmationPopout( std::filesystem::directory_entry &selection,
                         Ptr<FileContext> &file_context ) noexcept
{
  auto flag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize |
    ImGuiWindowFlags_NoMove;

  bool state = true;

  switch ( m_popup_type )
  {
    case PopUpType::ConfirmationReplace:
    {
      ImGui::OpenPopup( "Confirm Replace File" );
      ImGuiHelper::PopupModal( "Confirm Replace File", nullptr, flag, [&]
      {
        auto dest = selection.path() / copied.filename();
        std::string str( "''" + dest.generic_string() + "'' exist, do you want to override? This action cannot be undone." );
        f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
        ImGui::SetNextItemWidth( size );
        ImGui::Dummy( { 1.0f,12.0f } );
        ImGui::SetCursorPosX( 10.0f ); // Bound to break!
        ImGui::Text( str.c_str() );
        ImGui::Dummy( { 1.0f,12.0f } );
        if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
        {
          Filesystem::delete_file( dest );
          Filesystem::copy_file( copied, dest );
          state = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
        {
          state = false;
          ImGui::CloseCurrentPopup();
        }
      }, &state );
      break;
    }
    case PopUpType::ConfirmationDelete:
    {
      ImGui::OpenPopup( "Confirm Delete File" );
      ImGuiHelper::PopupModal( "Confirm Delete File", nullptr, flag, [&]
      {
        std::string str( "Are you sure you want to delete ''" + to_delete.generic_string() + "'' file? This action cannot be undone." );
        f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
        ImGui::SetNextItemWidth( size );
        ImGui::Dummy( { 1.0f,12.0f } );
        ImGui::SetCursorPosX( 10.0f ); // Bound to break!
        ImGui::Text( str.c_str() );
        ImGui::Dummy( { 1.0f,12.0f } );
        if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
        {
          Filesystem::delete_file( to_delete );
          if ( file_context->selection_file == to_delete.generic_string() )
          {
            file_context->selection_file.clear();
            open_file = false;
          }
          to_delete.clear();
          file_context->selection_file.clear();
          state = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
        {
          state = false;
          ImGui::CloseCurrentPopup();
        }
      }, &state );
      break;
    }
    case PopUpType::ConfirmationCut:
    {
      ImGui::OpenPopup( "Confirm Cut File" );
      ImGuiHelper::PopupModal( "Confirm Cut File", nullptr, flag, [&]
      {
        std::string str( "Are you sure you want to cut ''" + to_delete.generic_string() + "'' file? This action cannot be undone." );
        f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
        ImGui::SetNextItemWidth( size );
        ImGui::Dummy( { 1.0f,12.0f } );
        ImGui::SetCursorPosX( 10.0f ); // Bound to break!
        ImGui::Text( str.c_str() );
        ImGui::Dummy( { 1.0f,12.0f } );
        if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
        {
          Filesystem::delete_file( copied );
          auto dest = Filesystem::temp_dir() / to_delete.filename();
          if ( file_context->selection_file == to_delete.generic_string() )
          {
            file_context->selection_file.clear();
            open_file = false;
          }
          Filesystem::copy_file( to_delete, dest );
          Filesystem::delete_file( to_delete );
          copied = dest;
          to_delete.clear();
          file_context->selection_file.clear();
          state = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
        {
          state = false;
          ImGui::CloseCurrentPopup();
        }
      }, &state );
      break;
    }
    case PopUpType::CreateFolder:
    {
      ImGui::OpenPopup( "Creating folder" );
      ImGuiHelper::PopupModal( "Creating folder", nullptr, flag, [&]
      {
        std::string str( "Enter folder name: " );
        f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
        ImGui::SetNextItemWidth( size );
        ImGui::Dummy( { 1.0f,12.0f } );
        ImGui::SetCursorPosX( 10.0f ); // Bound to break!
        ImGui::Text( str.c_str() );
        ImGui::Dummy( { 1.0f,12.0f } );

        ImGui::InputText( "##create_folder", folder_to_create, 255 );
        if ( ImGui::Button( ICON_FA_CHECK, { size * 0.25f + 20.0f,40.0f } ) )
        {
          auto dir = selection.path() / folder_to_create;
          Filesystem::create_dir( dir );
          std::memset( folder_to_create, NULL, MAX_PATH );
          state = false;
          ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
        {
          state = false;
          ImGui::CloseCurrentPopup();
        }
      }, &state );
      break;
    }
    case PopUpType::DeleteFolder:
    {
      ImGui::OpenPopup( "Folder not empty" );
      ImGuiHelper::PopupModal( "Folder not empty", nullptr, flag, [&]
      {
        std::string str( "Folder not empty, please empty the folder before deleting it." );
        f32 size = ImGui::CalcTextSize( str.c_str() ).x + 40.0f;
        ImGui::SetNextItemWidth( size );
        ImGui::Dummy( { 1.0f,12.0f } );
        ImGui::SetCursorPosX( 10.0f ); // Bound to break!
        ImGui::Text( str.c_str() );
        ImGui::Dummy( { 1.0f,12.0f } );

        if ( ImGui::Button( ICON_FA_TIMES, { size * 0.25f + 20.0f,40.0f } ) )
        {
          state = false;
          ImGui::CloseCurrentPopup();
        }
      }, &state );
      break;
    }
  }
  if ( !state )
    m_popup = false;
}

void DrawFileBrowser( std::filesystem::directory_entry &selection,
                      Ptr<const std::vector<Utils::Directory>> &dir_list,
                      Ptr < const std::vector<Filesystem::fs::path>> &file_list,
                      Ptr<FileContext> &file_context ) noexcept
{
  auto region = ImGui::GetContentRegionAvail();

  ImGuiHelper::ChildWindow( "Details", { 0, 0 }, true,
                            ImGuiWindowFlags_AlwaysAutoResize,
                            [&]
  {
    if ( selection.exists() )
    {
      static ImGuiTextFilter filter;
      ImGui::Text( "Filter: " );
      ImGui::SameLine();
      filter.Draw( "##AssetFilter" );
      ImGui::Text( "" );

      static const auto size_of_button = 90.0f;

      int columns = static_cast<int>( ( region.x - ( region.x / 4.0f ) ) / size_of_button );
      columns = columns < 1 ? 1 : columns;
      ImVec2 button_size = { size_of_button, size_of_button };
      auto table_flag = ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_PreciseWidths;
      //if ( ImGui::BeginTable( "##Files", columns, table_flag ) );
      ImGuiHelper::Table( "##Files", columns, table_flag, [&]
      {
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.30f, 0.10f, 0.14f, 0.00f ) );
        //float internal_width = ImGui::GetContentRegionAvail().x * 0.5f;

        RenderDirectory( *dir_list, filter, button_size, selection, dir_list, file_list );
        RenderFile( *file_list, filter, button_size, file_context );
        ImGui::PopStyleColor();
        //ImGui::EndTable();
      } );

  //ImGui::BeginPopupContext
      ImGuiHelper::PopupContextWindow( "asset context menu", ImGuiPopupFlags_MouseButtonRight, false, [&]
      {
        if ( !copied.empty() && ImGui::Selectable( "Paste file" ) )
          PasteAsset( file_context );
        else if ( ImGui::Selectable( "Create Folder" ) )
        {
          m_popup_type = PopUpType::CreateFolder;
          m_popup = true;
        }
      } );
    }
    if ( m_popup )
      ConfirmationPopout( selection, file_context );

    // If open file
    /*if ( open_file && !file_context.selection_file.empty() )
    {
      if ( ImGui::Begin( file_context.selection_file.c_str(), &open_file ) )
        ImGui::Text( "WIP" );
      ImGui::End();
    }
    else if( !open_file )
    {
      file_context.selection_file.clear();
    }*/
  } );
}
}

#endif