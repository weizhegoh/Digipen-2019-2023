/*****************************************************************
*\file		 TextRenderSystem.cpp
*\brief		 TextRenderSystem functions definitions.

*\author(s)   Lee Liang Ping	52 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <TextRenderSystem.h>
#include <FakeEngine.h>


extern std::unique_ptr<FakeEngine> gFakeEngine;

void TextRenderSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	signature.set(mGameECSmanager.GetComponentType<EntityType>());
	mGameECSmanager.SetSystemBitSignature<TextRenderSystem>(signature);
}

void TextRenderSystem::Update()
{

	Timer& timer = gFakeEngine->GetGameManager<Timer>();
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	Window& window = gFakeEngine->GetGraphicManager<Window>();
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << "Time: "<< timer.gameTime;
	TextManager& textManager = gFakeEngine->GetGameManager<TextManager>();
	textManager.UpdateTextAndPosition(FreeTypeList::GAMETIMER, stream.str(), mathEngine::Vector3D(-100.f, (float)window.GetHalfHeight() * 0.9f, 0.f));
	
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	Inventory& inventory = mGameECSmanager.GetComponent<Inventory>(globalVar.mainCharID);
	
	std::stringstream stream_1;
	if(inventory._currObjective < globalVar.max_objective)
		stream_1 << "Find all Rock Samples";
	else
		stream_1 << "Go To the Golden Wrap";
	textManager.UpdateTextAndPosition(FreeTypeList::Objectives, stream_1.str(), mathEngine::Vector3D((float)window.GetHalfWidth() * 0.6f, (float)window.GetHalfHeight() * 0.9f, 0.f));

	std::stringstream stream_2;
	stream_2 << std::fixed << std::setprecision(1) << "Rock Samples: " << inventory._currObjective << " / " << globalVar.max_objective;
	textManager.UpdateTextAndPosition(FreeTypeList::InventoryCollect, stream_2.str(),mathEngine::Vector3D((float)window.GetHalfWidth() * 0.6f, (float)window.GetHalfHeight() * 0.85f, 0.f));

	float listmultipler = 0.9f;
	textManager.ActivateText(FreeTypeList::GOD_MODE_ACTIVATION, input.GetInputBooleen().god_mode);
	textManager.ActivateText(FreeTypeList::SPEED_MODE_ACTIVATION, input.GetInputBooleen().speed_hack);
	textManager.ActivateText(FreeTypeList::HACK_ACTIVATION, input.GetInputBooleen().activate_hack);
	if (input.GetInputBooleen().god_mode)
	{
		textManager.UpdatePosition(FreeTypeList::GOD_MODE_ACTIVATION, mathEngine::Vector3D((float)window.GetHalfWidth() * 0.75f, -(float)window.GetHalfHeight() * listmultipler, 0.f));
		listmultipler -= 0.05f;
	}
	if (input.GetInputBooleen().speed_hack)
	{
		textManager.UpdatePosition(FreeTypeList::SPEED_MODE_ACTIVATION, mathEngine::Vector3D((float)window.GetHalfWidth() * 0.75f, -(float)window.GetHalfHeight() * listmultipler, 0.f));
		listmultipler -= 0.05f;
	}
	if (input.GetInputBooleen().activate_hack)
	{
		textManager.UpdatePosition(FreeTypeList::HACK_ACTIVATION, mathEngine::Vector3D((float)window.GetHalfWidth() * 0.75f, -(float)window.GetHalfHeight() * listmultipler, 0.f));
		listmultipler -= 0.05f;
	}
	//textManager.UpdatePosition(FreeTypeList::SPEED_MODE_ACTIVATION, mathEngine::Vector3D((float)window.GetHalfWidth() * 0.6, -(float)window.GetHalfHeight() * 0.85, 0.f));
	//textManager.ActivateText(FreeTypeList::WIN_STATEMENT, input.GetInputBooleen().win_state);

}