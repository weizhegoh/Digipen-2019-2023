/*****************************************************************
*\file		ScoreSystem.h
*\brief		Declaration for ScoreSystem.

*\author(s)   Goh Wei Zhe	12 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef SCORESYSTEM_H
#define SCORESYSTEM_H

#include <EnginePch.h>
class ScoreSystem : public System
{
public:

	inline static std::string scoreInString{};

	void Init();
	void Update();
	void Draw();
};

#endif