/*****************************************************************
*\file		 Tutorial.h
*\brief		 Tutorial function declaration

*\author(s)   Kenric Tan Wei Liang		12 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>

class Tutorial : public System
{
public:
	void Init();
	void Update();
	void BoxUpdate();
	void InitEntity();
	void ActivateTutorial(TutorialActivate* activateTutorial);
	//void TutorialCase();
	void IncrementBox(mathEngine::Vector3D outputScale , mathEngine::Vector3D outputPos);
};