/*****************************************************************
*\file		 Tutorial.cpp
*\brief		 Tutorial functions definition

*\author(s)   Kenric Tan Wei Liang		288 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <Tutorial.h>
#include <FakeEngine.h>
#include <EntityCreation.h>

extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

int offset = 100;

void Tutorial::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	gFakeEngine->SubscribeEvent(this, &Tutorial::ActivateTutorial, EventID::ActivateTutorial);
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<Tutorial>(signature);
}

void Tutorial::InitEntity()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();

	Object tmp;
	Object h_tmp;
	Object t_tmp;

	for (auto& x : stageChanger.allobjects)
	{
		if (x._name.second == "TUTORIAL")
			tmp = x;
		else if (x._name.second == "TUTORIAL_BOX")
			h_tmp = x;
		else if (x._name.second == "TUTORIAL_INSTRUCT")
		{
			t_tmp = x;
			serializationVar.instructObject = x;
		}
	}
	Position h_pos , t_pos , tI_pos; 
	h_pos.pos = mathEngine::Vector3D(-1900.f, 400.0f, -0.5f);
	t_pos.pos = mathEngine::Vector3D(-600.0f, -800.0f, -0.5f);
	tI_pos.pos = mathEngine::Vector3D(-600.0f, -890.0f, -0.5f);

	Position mc_position = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);

	serializationVar.tutorialEntity = EntityCreation::CreateEntity(EntitySystemOption::GAME_ENTITY,
		EntityType{ ObjectType::TUTORIAL },
		Flags{ FLAG_INACTIVE },
		Position{ mc_position },
		Rigidbody{ tmp._rigidbody.second },
		Transform{ tmp._transform.second },
		Render{ tmp._render.second },
		RenderTex{ tmp._texture.second[0] },
		Colour{ tmp._colour.second },
		Light({ tmp._light.second.highlight ,0.8f, tmp._light.second.interp }),
		AABB(tmp._aabb.second),
		LightFlag(tmp._lightFlag.second)
	);

	serializationVar.boxEntity = EntityCreation::CreateEntity(EntitySystemOption::GAME_ENTITY,
		EntityType{ ObjectType::TUTORIAL_BOX },
		Flags{ FLAG_INACTIVE },
		Position{ tI_pos },
		Rigidbody{ h_tmp._rigidbody.second },
		Transform{ h_tmp._transform.second },
		Render{ h_tmp._render.second },
		RenderTex{ h_tmp._texture.second[0] },
		Colour{ h_tmp._colour.second },
		Light({ h_tmp._light.second.highlight ,0.8f, h_tmp._light.second.interp }),
		AABB(h_tmp._aabb.second),
		LightFlag(h_tmp._lightFlag.second)
	);

	serializationVar.instructEntity = EntityCreation::CreateEntity(EntitySystemOption::GAME_ENTITY,
		EntityType{ ObjectType::TUTORIAL_INSTRUCT },
		Flags{ FLAG_INACTIVE },
		Position{ t_pos },
		Rigidbody{ t_tmp._rigidbody.second },
		Transform{ t_tmp._transform.second },
		Render{ t_tmp._render.second },
		RenderTex{ t_tmp._texture.second[0] },
		Colour{ t_tmp._colour.second },
		Light({ t_tmp._light.second.highlight ,0.8f, h_tmp._light.second.interp }),
		AABB(t_tmp._aabb.second),
		LightFlag(t_tmp._lightFlag.second)
	);

	serializationVar.speed = g_dt;

	for (const auto& x : serializationVar.tutorialMap)
	{
		Light& tut_light = mGameECSmanager.GetComponent<Light>(x.first);
		tut_light.transparncy = 0.f;
	}
}

void Tutorial::Update()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

	Flags& flag = mGameECSmanager.GetComponent<Flags>(serializationVar.tutorialEntity);

	Position mc_position = mGameECSmanager.GetComponent<Position>(globalVar.mainCharID);
	Position& tut_pos = mGameECSmanager.GetComponent<Position>(serializationVar.tutorialEntity);
	Light& tut_light = mGameECSmanager.GetComponent<Light>(serializationVar.tutorialEntity);
	State mc_state = mGameECSmanager.GetComponent<State>(globalVar.mainCharID);

	tut_pos.pos.x = mc_position.pos.x - 200.f;
	tut_pos.pos.y = mc_position.pos.y + 150.f;
	tut_pos.pos.z = -0.5f;

	//increment time

	if (serializationVar.tutorialTime > 0)
	{
		serializationVar.tutorialTime -= g_dt;
		tut_light.transparncy -= g_dt;
	}
	else
	{
		flag.activeFlag = FLAG_INACTIVE;
		serializationVar.activateFlag = false;
		serializationVar.audiocheck = false;
	}

	if (serializationVar.activateFlag)
	{
		serializationVar.audiocheck = true;
		flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
	}
}

void Tutorial::BoxUpdate()
{
	StageChanger& stageChanger = gFakeEngine->GetGraphicManager<StageChanger>();
	if (stageChanger.GetCurrentStage() == StagesEnum::ACTUAL_STAGE)
	{
		auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();

		SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
		Input& input = gFakeEngine->GetGraphicManager<Input>();
		Flags& h_flag = mGameECSmanager.GetComponent<Flags>(serializationVar.boxEntity);
		Flags& i_flag = mGameECSmanager.GetComponent<Flags>(serializationVar.instructEntity);
		Position& box_pos = mGameECSmanager.GetComponent<Position>(serializationVar.boxEntity);
		Transform& box_scale = mGameECSmanager.GetComponent<Transform>(serializationVar.boxEntity);
		RenderTex& instruct_tex = mGameECSmanager.GetComponent<RenderTex>(serializationVar.instructEntity);

		Position h_pos, ht_pos, hc_pos;
		h_pos.pos = mathEngine::Vector3D(-1900.f, 400.0f, -0.5f);
		ht_pos.pos = mathEngine::Vector3D(-570.f, 400.0f, -0.5f);
		hc_pos.pos = mathEngine::Vector3D(480.f, 370.0f, -0.5f);


		if (serializationVar.afterPopup == true)
		{
			serializationVar.duringTutorial = true;
			i_flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;
			h_flag.activeFlag = FLAG_ACTIVE | FLAG_VISIBLE;

			if (input.CheckPress(GLFW_KEY_SPACE) || serializationVar.press)
			{
				serializationVar.press = true;

				switch (serializationVar.caseId)
				{
				case 0:
				{

					instruct_tex = serializationVar.instructObject._texture.second[1];
					IncrementBox(mathEngine::Vector3D{ 200.0f , 100.0f , -0.5f }, h_pos.pos);
					//std::cout << "x " << box_scale.scale.x << std::endl;
					//std::cout << "y " << box_scale.scale.y << std::endl;
					//std::cout << "xpos box " << .pos.x
					if (box_scale.scale.x == 200.0f
						&& box_scale.scale.y == 100.f
						&& h_pos.pos.x == box_pos.pos.x
						&& h_pos.pos.y == box_pos.pos.y)
					{
						serializationVar.caseId = 1;
						serializationVar.press = false;
						serializationVar.speed = g_dt;

					}
					break;
				}
				case 1:
				{
					instruct_tex = serializationVar.instructObject._texture.second[2];

					IncrementBox(mathEngine::Vector3D{ 400.0f , 100.0f , -0.5f }, ht_pos.pos);
					if (box_scale.scale.x == 400.0f
						&& box_scale.scale.y == 100.f
						&& ht_pos.pos.x == box_pos.pos.x
						&& ht_pos.pos.y == box_pos.pos.y)
					{
						serializationVar.caseId = 2;
						serializationVar.press = false;
						serializationVar.speed = g_dt;
					}
					break;
				}
				case 2:
				{
					instruct_tex = serializationVar.instructObject._texture.second[3];

					IncrementBox(mathEngine::Vector3D{ 710.0f , 100.0f , -0.5f }, hc_pos.pos);
					if (box_scale.scale.x == 710.0f
						&& box_scale.scale.y == 100.f
						&& hc_pos.pos.x == box_pos.pos.x
						&& hc_pos.pos.y == box_pos.pos.y)
					{
						if (input.CheckPress(GLFW_KEY_SPACE))
						{
							serializationVar.press = false;
							serializationVar.duringTutorial = false;
							serializationVar.afterPopup = false;

							h_flag.activeFlag = FLAG_INACTIVE;
							i_flag.activeFlag = FLAG_INACTIVE;
						}
					}
					break;
				}
				}
			}
		}
	}
}

void Tutorial::IncrementBox(mathEngine::Vector3D outputScale , mathEngine::Vector3D outputPos)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	Transform& box_scale = mGameECSmanager.GetComponent<Transform>(serializationVar.boxEntity);
	Position& box_pos = mGameECSmanager.GetComponent<Position>(serializationVar.boxEntity);

	if (box_scale.scale.x < outputScale.x)
	{
		if (box_scale.scale.x >= outputScale.x - 0.5f)
		{
			box_scale.scale.x = outputScale.x;
		}
		box_scale.scale.x = mathEngine::LinearInterpolate(box_scale.scale.x, outputScale.x, serializationVar.speed / 2);

	}
	else if(box_scale.scale.x > outputScale.x)
	{
		if (box_scale.scale.x <= outputScale.x + 0.5f)
		{
			box_scale.scale.x = outputScale.x;
		}
		else if (box_scale.scale.x > outputScale.x)
		{
			box_scale.scale.x = outputScale.x;
		}
		box_scale.scale.x = mathEngine::LinearInterpolate(box_scale.scale.x, outputScale.x, serializationVar.speed / 2);

	}


	if (box_scale.scale.y < outputScale.y)
	{
		if (box_scale.scale.y >= outputScale.y - 0.5f)
		{					
			box_scale.scale.y = outputScale.y;
		}
		box_scale.scale.y = mathEngine::LinearInterpolate(box_scale.scale.y, outputScale.y, serializationVar.speed / 2);

	}
	else if (box_scale.scale.y > outputScale.y)
	{
		if (box_scale.scale.y <= outputScale.y + 0.5f)
		{
			box_scale.scale.y = outputScale.y;
		}
		box_scale.scale.y = mathEngine::LinearInterpolate(box_scale.scale.y, outputScale.y, serializationVar.speed / 2);

	}

	if (box_pos.pos.x < outputPos.x)
	{
		if (box_pos.pos.x >= outputPos.x - 0.5f)
		{
			box_pos.pos.x = outputPos.x;
		}
		box_pos.pos.x = mathEngine::LinearInterpolate(box_pos.pos.x, outputPos.x, serializationVar.speed / 2);


	}
	else if (box_pos.pos.x > outputPos.x)
	{
		if (box_pos.pos.x <= outputPos.x + 0.5f)
		{
			box_pos.pos.x = outputPos.x;
		}
		box_pos.pos.x = mathEngine::LinearInterpolate(box_pos.pos.x, outputPos.x, serializationVar.speed / 2);

	}

	if (box_pos.pos.y < outputPos.y)
	{
		if (box_pos.pos.y >= outputPos.y - 0.5f)
		{
			box_pos.pos.y = outputPos.y;
		}
		box_pos.pos.y = mathEngine::LinearInterpolate(box_pos.pos.y, outputPos.y, serializationVar.speed / 2);

	}
	else if (box_pos.pos.y > outputPos.y)
	{
		if (box_pos.pos.y <= outputPos.y + 0.5f)
		{
			box_pos.pos.y = outputPos.y;
		}
		box_pos.pos.y = mathEngine::LinearInterpolate(box_pos.pos.y, outputPos.y, serializationVar.speed / 2);

	}

	serializationVar.speed *= 1.1f;

}

void Tutorial::ActivateTutorial(TutorialActivate* activateTutorial)
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	SerializationVar& serializationVar = gFakeEngine->GetGameManager<SerializationVar>();
	Light& tut_light = mGameECSmanager.GetComponent<Light>(serializationVar.tutorialEntity);
	RenderTex& tut_tex = mGameECSmanager.GetComponent<RenderTex>(serializationVar.tutorialEntity);
	tut_tex.texture = serializationVar.tutorialMap.find(activateTutorial->tutorialFloorID)->second.texture;

	tut_light.transparncy = 0.8f;
	serializationVar.activateFlag = true;
	serializationVar.tutorialTime = 3.0f;

	if (!serializationVar.audiocheck)
		gAudioEngine->PlaySound(gAudioEngine->TypeToString(SoundType::BUTTON_CLICK), FMOD_LOOP_OFF, 0.9f);

}
