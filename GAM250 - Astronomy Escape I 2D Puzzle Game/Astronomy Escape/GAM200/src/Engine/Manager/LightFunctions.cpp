/*****************************************************************
*\file		 LightFunctions.cpp
*\brief		 Header file declarations for LightFunction.

*\author(s)   Lee Liang Ping	181 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <LightFunctions.h>
#include <FakeEngine.h> 
#include <ComponentsHeader.h>
#include <EntityCreation.h>
#include <Collision.h>
#include <SystemList.h>

constexpr float MC_MaxRadient = 100.f;
constexpr float MC_DEATHEXPLODESPEED = 10000.f;
constexpr float KeyRemovingSPEED = 1000.f;

constexpr float KeyObjectiveRadient = 2000.1f;

//mathEngine::Vector3D KeyObjectiveAttenuationEq = mathEngine::Vector3D(0.2126, 0.7152, 0.0722);
mathEngine::Vector3D KeyObjectiveAttenuationEq = mathEngine::Vector3D(0.f, 0.006f, 0.f);

constexpr int keyObjectivesPartsLightAmount = 5;


constexpr std::pair<float, float> NormalLightRandient = std::make_pair(600.f, 600.f);
constexpr std::pair<float, float> NormalLightSpeed = std::make_pair(400.2f, 500.5f);


void LightEffects::Init(const size_t& amount)
{
	(void)amount;
	Create_NormalLight(5);
	Create_MainCharLight();
	Create_KeyObjectiveLight();
}

void  LightEffects::RandomGenerate_NormalLight(Entity entity)
{
	//auto& mLightECSmanager = gFakeEngine->GetECSLightManager<ECSManager>();
	//LifeTime& lifeTime = mLightECSmanager.GetComponent<LifeTime>(entity);
	//lifeTime.Random(1.f, 1.5f);
	//Colour& colour = mLightECSmanager.GetComponent<Colour>(entity);
	//colour.Random();
	EntityCreation::AssignEntity(EntitySystemOption::LIGHT_ENTITY, entity,
		RandomPointLight(NormalLightRandient),
		Colour{{1.f, 1.f, 1.f}}
	);
}

void LightEffects::Create_NormalLight(size_t amount)
{
	(void)(amount);
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	//for (size_t i = 0; i < amount; ++i)
	for (const auto& x: serializationVar.tutorialMap)
	{
		auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
		Position& position = mGameECSmanager.GetComponent<Position>(x.first);

		EntityCreation::CreateEntity(EntitySystemOption::LIGHT_ENTITY,
			ET_LightNormal(),
			Flags{ {FLAG_ACTIVE} },
			position,
			Rigidbody{
				{0.f , 0.f, mathEngine::RandNumberBetween(NormalLightSpeed.first, NormalLightSpeed.second)}
			},
			Colour
			{
				{mathEngine::RandNumberBetween(0.f, 1.f),
				mathEngine::RandNumberBetween(0.f, 1.f),
				mathEngine::RandNumberBetween(0.f, 1.f)}
			},
			PointLight{ RandomPointLight(NormalLightRandient) },
			LifeTime(),
			Transform(),
			Render(),
			PhysicFlag{ P_NORMAL }
		);
	}
}

void  LightEffects::Create_MainCharLight()
{
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	EntityCreation::CreateEntity(EntitySystemOption::LIGHT_ENTITY,
		ET_Light_MainChar(),
		Flags{ {FLAG_ACTIVE} },
		Position{ gFakeEngine->GetECSGameManager<ECSManager>().GetComponent<Position>(globalVar.mainCharID) },
		Rigidbody{ {0.f , 0.f, MC_DEATHEXPLODESPEED} },
		Colour{ {1.f, 1.f, 1.f} },
		PointLight
		{ 
			{0.1f, 0.007f, 0.f},
			MC_MaxRadient,
			0.f, MC_MaxRadient
		},
		Transform(),
		Render(),
		PhysicFlag{ P_NORMAL }
	);
}

void  LightEffects::Create_KeyObjectiveLight()
{
	auto& mGameSystem = gFakeEngine->GetECSGameManager<GameSystemsList>();
	
	float radient = KeyObjectiveRadient / mGameSystem.keyObjectiveSystem->mEntities.size();

	for (auto const& entity : mGameSystem.keyObjectiveSystem->mEntities)
	{
		EntityCreation::CreateEntity(EntitySystemOption::LIGHT_ENTITY,
			ET_Light_KeyObjectives{ entity },
			Flags{ {FLAG_ACTIVE} },
			Position{ gFakeEngine->GetECSGameManager<ECSManager>().GetComponent<Position>(entity) },
			Rigidbody{ {0.f , 0.f, KeyRemovingSPEED} },
			Colour{ {1.f,1.f,1.f} },
			PointLight{ {KeyObjectiveAttenuationEq}, radient,1.f, radient },
			Transform(),
			Render(),
			PhysicFlag{ P_NORMAL }
		);

		for (int i = 0; i < keyObjectivesPartsLightAmount; ++i)
		{
			EntityCreation::CreateEntity(EntitySystemOption::LIGHT_ENTITY,
				ET_Light_KeyObjectives_Parts{ entity },
				Flags(),
				Position{ gFakeEngine->GetECSGameManager<ECSManager>().GetComponent<Position>(entity) },
				Rigidbody(),
				Colour{ {1.f,1.f,1.f} },
				PointLight{ RandomPointLight(NormalLightRandient) },
				Transform(),
				Render()
			);
		}
	}
}

Position LightEffects::RandomPosition(float x, float y)
{
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	float RndBetweenX = x;
	float RndBetweenY = y;

	Position setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
	//Position setPos = Position{ { RndBetweenX, RndBetweenY, 0.f } };
	while (true)
	{
		if (globalVar.GetAndConvertCellValue(setPos.pos) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue(setPos.pos - 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue(setPos.pos + 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x - 50, (int)setPos.pos.y) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x + 50, (int)setPos.pos.y) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x + 50, (int)setPos.pos.y) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x, (int)setPos.pos.y + 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x, (int)setPos.pos.y - 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x + 50, (int)setPos.pos.y - 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else if (globalVar.GetAndConvertCellValue((int)setPos.pos.x - 50, (int)setPos.pos.y + 50) != ObjectType::FLOOR)
		{
			setPos = Position{ { mathEngine::RandNumberBetween(-RndBetweenX, RndBetweenX), mathEngine::RandNumberBetween(-RndBetweenY, RndBetweenY), 0.f } };
			continue;
		}
		else
			return setPos;
	}
}

PointLight LightEffects::RandomPointLight(std::pair<float, float> maxAmbientBetween)
{
	PointLight pointlight;
	pointlight.attenuationEq = { 0.f, mathEngine::RandNumberBetween(0.041f, 0.041f), 0.f };
	pointlight.ambient = mathEngine::RandNumberBetween(500.f, 500.f);
	pointlight.ambientDir = 1.f;
	pointlight.maxAmbient = pointlight.ambient + mathEngine::RandNumberBetween(maxAmbientBetween.first, maxAmbientBetween.second);
	return pointlight;
}


//void LightEffects::RenderLight(LightFlag& lightFlag, PointLight& pointLight, AABB& aabb, GLSLShader& shdrpgm)
//{
//	//auto& mLightECSmanager = gFakeEngine->GetECSManager<ECSManager>(EntitySystemOption::LIGHT_ENTITY);
//	//Graphics::Gfx::RenderFunctions& gRender = gFakeEngine->GetGraphicManager<Graphics::Gfx::RenderFunctions>();
//	//int i = 0;
//	//int flagI = 0;
//	//while (0 == (lightFlag.flag & LIGHT_NIL))
//	//{
//
//	//	auto& mLightSystem = gFakeEngine->GetECSLightManager<LightSystemsList>();
//
//	//	std::set<Entity>& entitySets = mLightSystem.lightSystem->mEntities;
//
//	//	if ((lightFlag.flag & FlagArray[flagI++]))
//	//	{
//	//		for (const auto& light_entity : mLightSystem.lightSystem->mEntities)
//	//		{
//	//			Position& position = mLightECSmanager.GetComponent<Position>(light_entity);
//	//			PointLight& pointLight = mLightECSmanager.GetComponent<PointLight>(light_entity);
//	//			if (Collision::OBB::CircleToRect(Circle(position.pos, pointLight.ambient), aabb))
//	//			{
//	//				Colour& colour = mLightECSmanager.GetComponent<Colour>(light_entity);
//	//				gRender.SetLight(pointLight, position.pos, colour.colour, i, shdrpgm);
//	//			}
//	//		}
//	//	}
//	//}
//
//}