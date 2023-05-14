/*****************************************************************
*\file		 Serialization.h
*\brief		 Header file declarations for Serialization.

*\author(s)   Kenric Tan Wei Liang	13 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#include <EnginePch.h>
#include <Object.h>
namespace Serialization
{


	void load_sound_FromFile();
	void load_object_FromFile();
	[[nodiscard]] Object load_objects(std::string filename);

	//test for debugging
	void testObjData(std::vector<Object> _objects);

	void LoadBoard(std::string filename);

	void creation_of_gameplay_stuff(std::string filename, const std::vector<Object> allobjects);

	void AssignBoard(const Data _data, const std::vector<Object> allobjects);
};