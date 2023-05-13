/*****************************************************************
*\file		 InventorySystem.cpp
*\brief		 InventorySystem functions definitions.

*\author(s)   Kenric Tan Wei Liang	127 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <InventorySystem.h>
#include <FakeEngine.h>
#include <SystemList.h>
#include <EntityCreation.h>
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void InventorySystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	gFakeEngine->SubscribeEvent(this, &InventorySystem::PickUp, EventID::PickUpEvent);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	signature.set(mGameECSmanager.GetComponentType<Inventory>());
	mGameECSmanager.SetSystemBitSignature<InventorySystem>(signature);
}

void InventorySystem::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	GlobalVar& globalvar = gFakeEngine->GetGameManager<GlobalVar>();
	for (auto const& entity : mEntities)
	{
		Flags& flag = mGameECSmanager.GetComponent<Flags>(entity);
		if (0 == (flag.activeFlag & FLAG_ACTIVE))
			continue;
		Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(entity);
		if (globalvar.level_door_opened)
		{
			inventory.timer += g_dt;
			if (inventory.timer >= 3.f)
			{
				Position& position = mGameECSmanager.GetComponent<Position>(globalvar.level_doorID);
				gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
					position.pos, 35.f, ObjectDirection::CIRCLE,
					ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
					std::make_pair<float, float>(150.f, 150.f),
					std::make_pair<float, float>(0.f, 0.f),
					std::make_pair<float, float>(7.f, 9.f),
					std::make_pair<float, float>(1.2f, 1.2f),
					std::vector<ParticleAction>(),
					{ 1.f, 1.f, 0.f }, { 1.f,1.f,0.f,0.9 }, 0.7f, ShaderOption::FULL_TEXTURE,
					ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
					globalvar.level_doorID, 40, false, PhysicFlag{ P_NORMAL })
				);
				inventory.timer = 0.f;
			}

		}
	}
}

void InventorySystem::PickUp(PickUpEvent* pickupevent)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(pickupevent->main_characterID);
	EntityType& type = mGameECSmanager.GetComponent<EntityType>(pickupevent->itemID);
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

	SoundType soundtype[3] = { SoundType::PICKUP_1 , SoundType::PICKUP_2 , SoundType::PICKUP_3};

	if (type.objType == ObjectType::KEY_OBJECTIVE)
	{
		inventory._currObjective++;
		globalVar.exit_door_opened = true;
		std::cout << "Objective " << inventory._currObjective << std::endl;
		if (mGameECSmanager.GetComponent<CollisionFlag>(pickupevent->itemID).audioActivate)
		{
			gAudioEngine->PlaySound(gAudioEngine->TypeToString(soundtype[inventory.audioPickupIndex++]), FMOD_LOOP_OFF, 0.5f);
			if (inventory.audioPickupIndex >= sizeof(soundtype)/ sizeof(SoundType))
				inventory.audioPickupIndex = 0;

		}
			//gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::PICKUP_1), FMOD_LOOP_OFF, 0.5f);


		auto& mLightECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::LIGHT_ENTITY);
		auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();

		for (const auto& light_entity : mLightSystem.keyObjectives_lightSystem->mEntities)
		{
			ET_Light_KeyObjectives& et_Light_ko = mLightECSmanager.GetComponent<ET_Light_KeyObjectives>(light_entity);
			if (et_Light_ko.id.id == pickupevent->itemID && et_Light_ko.state == TypeState::Active)
			{
				et_Light_ko.state = TypeState::Removing;
				Position& position = mGameECSmanager.GetComponent<Position>(pickupevent->itemID);
				{
					std::vector<ParticleAction> actions;
					actions.push_back(ParticleAction::PULSE);
					actions.push_back(ParticleAction::TOWARDS_LEVEL_DOOR);
					actions.push_back(ParticleAction::END);

					gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
						position.pos, 35.f, ObjectDirection::CIRCLE,
						ParticleTypesEnum::PARTICLE_PICKUP,
						std::make_pair<float, float>(100.f, 300.f),
						std::make_pair<float, float>(0.f, 0.f),
						std::make_pair<float, float>(7.f, 9.f),
						std::make_pair<float, float>(1.2f, 1.2f),
						actions,
						{ 1.f, 1.f, 0.f }, { 1.f,1.f,1.f,0.9 }, 0.3f, ShaderOption::FULL_TEXTURE,
						ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
						globalVar.level_doorID, 100, true, light_entity, PhysicFlag{ P_NORMAL | P_FRICTION })
					);
				}
				break;
			}

		}
	}

	if (type.objType == ObjectType::COLLECTIBLES)
	{
		if (mGameECSmanager.GetComponent<CollisionFlag>(pickupevent->itemID).audioActivate)
		{
			gAudioEngine->PlaySound(gAudioEngine->TypeToString(soundtype[inventory.audioPickupIndex++]), FMOD_LOOP_OFF, 0.5f);
			if (inventory.audioPickupIndex >= sizeof(soundtype) / sizeof(SoundType))
				inventory.audioPickupIndex = 0;
		}

		Position& position = mGameECSmanager.GetComponent<Position>(pickupevent->itemID);
		{
			std::vector<ParticleAction> actions;
			actions.push_back(ParticleAction::PULSE);
			actions.push_back(ParticleAction::TOWARDS_MC);
			actions.push_back(ParticleAction::END);

			gFakeEngine->PublishEvent(new CreatePulseParticleEvent(
				position.pos, 35.f, ObjectDirection::CIRCLE,
				ParticleTypesEnum::PARTICLE_PULSE_OUTWARDS,
				std::make_pair<float, float>(100.f, 300.f),
				std::make_pair<float, float>(0.f, 0.f),
				std::make_pair<float, float>(7.f, 9.f),
				std::make_pair<float, float>(0.5f, 1.2f),
				actions,
				{ 1.f, 1.f, 0.f }, { 1.f,1.f,1.f,0.9 }, 0.3f, ShaderOption::FULL_TEXTURE,
				ObjectShapes::SQUARE, TextureType::GLOWINGBALL,
				globalVar.mainCharID, 100, false, PhysicFlag{ P_NORMAL | P_FRICTION }));
		}
	}

	//ADD SCORE IF PICKEDUp
	globalVar.score += 1000;
}