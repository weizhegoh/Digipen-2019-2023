/*****************************************************************
*\file		 Animations.cpp
*\brief		 Animations functions definitions.

*\author(s)   Lee Jun Jie	63 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#include <EnginePch.h>
#include "Animations.h"
#include "rapidjson/document.h"


ObjectState StringToObjectStateEnum(std::string str)
{
	if (str == "running")
		return ObjectState::MOVING;
	if (str == "pushing")
		return ObjectState::PUSHING_BARREL;
	if (str == "death")
		return ObjectState::DEATH;
	if (str == "monster_running")
		return ObjectState::MONSTER_MOVING;
	if (str == "monster_idle")
		return ObjectState::MONSTER_IDLE;
	if (str == "monster2_running")
		return ObjectState::MONSTER_GUIDE_MOVING;
	if (str == "monster2_idle")
		return ObjectState::MONSTER_GUIDE_IDLE;
	if (str == "monster3_running")
		return ObjectState::MONSTER_PATROL_MOVING;
	if (str == "monster3_idle")
		return ObjectState::MONSTER_PATROL_IDLE;
	if (str == "DOOR_OPEN")
		return ObjectState::DOOR_CLOSE;
	if (str == "DOOR_CLOSE")
		return ObjectState::DOOR_OPEN;
	return ObjectState::IDLE;
}


Animations::Animations(const char* pFileName)
{
	std::ifstream inputFile(pFileName);
	assert(inputFile);
	std::stringstream jsonDocumentBuffer;
	std::string inputLine;
	while (std::getline(inputFile, inputLine))
		jsonDocumentBuffer << inputLine << "\n";
	rapidjson::Document dom;
	dom.Parse(jsonDocumentBuffer.str().c_str());
	const rapidjson::Value& attributes = dom["sprite"];
	assert(attributes.IsArray()); // attributes is an array

	static float counter = 0.f;

	const rapidjson::Value& prevattribute = *attributes.Begin();
	ObjectState prevObjState = StringToObjectStateEnum(prevattribute["name"].GetString());
	frameOffset.insert({ prevObjState,counter });
	for (rapidjson::Value::ConstValueIterator itr = attributes.Begin(); itr != attributes.End(); ++itr)
	{
		const rapidjson::Value& attribute = *itr;
		assert(attribute.IsObject()); // each attribute is an object
		std::cout << attribute["name"].GetString() << std::endl;
		ObjectState objState = StringToObjectStateEnum(attribute["name"].GetString());

		//Numbers numbers = StringToNumStateEnum(attribute["name"].GetString());
		glm::vec2 offset{ attribute["x"].GetFloat() / dom["sheet_width"].GetFloat(),
		  attribute["y"].GetFloat() / dom["sheet_height"].GetFloat() };
		FrameData data{ offset, attribute["duration"].GetFloat() };
		animationList[objState].push_back(data);
		counter++;


		if (objState == prevObjState)
			continue;
		frameOffset.insert({ objState,counter });
		prevObjState = objState;

	}
	counter = 0;
}