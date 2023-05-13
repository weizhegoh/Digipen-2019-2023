/*****************************************************************
*\file		 State.h
*\brief		 State Component 

*\author(s)   Lee Liang Ping	27 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct State
{
	ObjectState objectState{ ObjectState::IDLE };
	ObjectDirection objectDir{ ObjectDirection::NO_DIR };

	ObjectDirection InverseDir()
	{
		if (objectDir == ObjectDirection::DOWN)
			return ObjectDirection::UP;
		else if (objectDir == ObjectDirection::UP)
			return ObjectDirection::DOWN;
		else if (objectDir == ObjectDirection::LEFT)
			return ObjectDirection::RIGHT;
		else if (objectDir == ObjectDirection::RIGHT)
			return ObjectDirection::LEFT;
		else if (objectDir == ObjectDirection::UPLEFT)
			return ObjectDirection::DOWNRIGHT;
		else if (objectDir == ObjectDirection::UPRIGHT)
			return ObjectDirection::DOWNLEFT;
		else if (objectDir == ObjectDirection::DOWNLEFT)
			return ObjectDirection::UPRIGHT;
		else if (objectDir == ObjectDirection::DOWNRIGHT)
			return ObjectDirection::UPLEFT;
		else
			return objectDir;
	}
};