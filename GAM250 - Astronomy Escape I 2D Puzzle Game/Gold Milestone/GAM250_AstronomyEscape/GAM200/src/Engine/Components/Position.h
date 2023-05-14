/*****************************************************************
*\file		 Position.h
*\brief		 Position Component 

*\author(s)   Lee Liang Ping	21 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <vector3D.h>

struct Position
{
	mathEngine::Vector3D pos {0.0f,0.0f,0.0f};
	mathEngine::Vector3D starting_pos{ 0.0f,0.0f,0.0f };
	mathEngine::Vector3D prev_pos {0.0f,0.0f,0.0f};
	mathEngine::Vector3D prev_binarypos{ 0.0f,0.0f,0.0f };
	mathEngine::Vector3D next_binarypos{ 0.0f,0.0f,0.0f };


	void Print()
	{ 
		std::cout << " Position : "; pos.print(); std::cout << std::endl;
		std::cout << " Previous Position : "; prev_pos.print(); std::cout << std::endl;
		std::cout << " Starting Position : "; starting_pos.print();  std::cout << std::endl;
	}

	void Update(mathEngine::Vector3D& velocity, float& dt)
	{
		pos.x += velocity.x * dt;
		pos.y += velocity.y * dt;
	}

};