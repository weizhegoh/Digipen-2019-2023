/*****************************************************************
*\file		 ImGuiVar.h
*\brief		 Variable of Imgui

*\author(s)   Lee Liang Ping  67 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include <PointLight.h>
#include <Position.h>

extern float g_dt;
struct ImGuiVar
{
	glm::vec3 lightpos;
	glm::vec3 lightColour;
	float lightambient;
	float lightconstant;
	float lightlinear;
	float lightquadratic;


	std::vector<PointLight> lightList;
	std::vector<Position> position;
	std::vector<std::pair<float, float>> timer;

	void init()
	{
		lightList.reserve(128);
		position.reserve(128);
		timer.reserve(128);
		for (int i = 0; i < 128; ++i)
		{
			PointLight tmp;
			Position postmp;
			tmp.ambient = mathEngine::RandNumberBetween(2.1f, 10.f);
			tmp.attenuationEq =
			{
				0.f,
				mathEngine::RandNumberBetween(0.006f, 0.041f),
				0.f
			};
			postmp.pos =
			{
				mathEngine::RandNumberBetween(-2000.f, 2000.f),
				mathEngine::RandNumberBetween(-2000.f, 2000.f),
				0.f
			};
			position.push_back(postmp);
			lightList.push_back(tmp);
			timer.push_back(std::make_pair(mathEngine::RandNumberBetween(1.f, 20.f), 0.f));
		}
	}
	void updte()
	{
		for (int i = 0; i < 128; ++i)
		{
			timer[i].second += g_dt;
			if (timer[i].second >= timer[i].first)
			{
				timer[i].second = 0.f;
				lightList[i].ambient = mathEngine::RandNumberBetween(2.1f, 10.f);
				lightList[i].attenuationEq =
				{
					0.f,
					mathEngine::RandNumberBetween(0.006f, 0.041f),
					0.f
				};
				position[i].pos =
				{
					mathEngine::RandNumberBetween(-2000.f, 2000.f),
					mathEngine::RandNumberBetween(-2000.f, 2000.f),
					0.f
				};
			}
		
		}
	}
};

