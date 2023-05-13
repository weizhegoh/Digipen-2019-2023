/*****************************************************************
*\file		ScoreSystem.cpp
*\brief		Definition for ScoreSystem.

*\author(s)   Goh Wei Zhe	42 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <ScoreSystem.h>
#include <FakeEngine.h>

extern float g_dt;
extern std::unique_ptr<FakeEngine> gFakeEngine;
extern std::unique_ptr<AudioEngine> gAudioEngine;

void ScoreSystem::Init()
{
	auto& mGameECSmanager = gFakeEngine->GetECSGameManager<ECSManager>();
	BitSignature signature;
	mGameECSmanager.SetSystemBitSignature<ScoreSystem>(signature);
}

void ScoreSystem::Update()
{
	Input& input = gFakeEngine->GetGraphicManager<Input>();
	GlobalVar& globalVar = gFakeEngine->GetGameManager<GlobalVar>();

	int finalScore = 0;
	int baseScore = 0;
	int startingScore = 1000000;
	//int minScore = 5000;

	if (input.GetInputBooleen().win_state)
	{
		globalVar.scoreTime = globalVar.scoreTime;

		baseScore = static_cast<int>(startingScore / globalVar.scoreTime);

		if (baseScore < 5000)
			baseScore = 5000;

		if (baseScore > startingScore)
			baseScore = startingScore;

		finalScore = baseScore + globalVar.score;
	}
	else
	{
		globalVar.scoreTime += g_dt;
	}

	std::stringstream ss;

	ss << std::setw(7) << std::setfill('0') << finalScore;
	scoreInString = ss.str();
}

void ScoreSystem::Draw()
{
	Graphics::FreeType& freetype = gFakeEngine->GetFreeTypeFont(FreeType_Font::AgencyFBBold);
	freetype.RenderText(scoreInString, -225.f, -45.f, 3.0f, glm::vec3(1.f, 1.f, 1.f), FreeRenderType::SCREEN);
}