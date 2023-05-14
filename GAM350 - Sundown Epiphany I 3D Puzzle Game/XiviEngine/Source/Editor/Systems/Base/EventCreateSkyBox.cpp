/*****************************************************************
*\file         EventCreateSkyBox.cpp
*\author(s)    Chng Yong Quan

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include "pch.h"

#ifdef XV_EDITOR
#include "EventCreateSkyBox.h"
#include "Components//GraphicsComponents.h"

namespace Xivi::Editor
{
void EventCreateSkyBoxSystem::Init( ECS::World::Instance &world ) noexcept
{
  m_skybox_archetype = &world.GetOrCreateArchetype<SkyBoxType>();
}

void EventCreateSkyBoxSystem::OnCall( ECS::World::Instance &world, Ptr<RM> resource_manager ) noexcept
{
  m_skybox_archetype->CreateEntity( [&]( Transform &xform, StaticMeshComponent &mesh, SkyBoxMaterial &material ) noexcept
  {
    XV_CORE_TRACE( "Assigning Skybox Value" );
    xform.scale = Vec3f { 10.f,10.f,10.f };
    mesh.model = resource_manager->Get<Graphics::StaticModel>( "Assets/Models/binary/Object/skybox.BinaryObj" );

    material.skybox_LT = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_rt.dds" );
    material.skybox_RT = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_lf.dds" );
    material.skybox_DN = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_up.dds" );
    material.skybox_UP = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_dn.dds" );
    material.skybox_BK = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_bk.dds" );
    material.skybox_FT = resource_manager->Get<Graphics::Texture>( "Assets/Textures/skybox/blood-stain-gorge_ft.dds" );

    //material.skybox_LT = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/rt.dds");
    //material.skybox_RT = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/lf.dds");
    //material.skybox_DN = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/up.dds");
    //material.skybox_UP = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/dn.dds");
    //material.skybox_BK = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/bk.dds");
    //material.skybox_FT = resource_manager->Get<Graphics::Texture>("Assets/Textures/skybox/ft.dds");
  } );
}
}
#endif