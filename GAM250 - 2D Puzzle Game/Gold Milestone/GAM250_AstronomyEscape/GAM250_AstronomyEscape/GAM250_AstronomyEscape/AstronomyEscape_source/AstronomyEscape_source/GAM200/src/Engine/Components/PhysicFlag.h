/*****************************************************************
*\file		 PhysicFlag.h
*\brief		 PhysicFlag

*\author(s)   Lee Liang Ping	25 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once


struct PhysicFlag
{
	unsigned int flag{ P_NIL };

	void Read(int normal, int friction, int gravity)
	{
		if (normal == 0 && friction == 0 && gravity == 0)
		{
			flag = P_NIL;
			return;
		}
		if (normal)
		{
			(flag == P_NIL) ? flag = P_NORMAL : flag |= P_NORMAL;
		}
		if (friction)
		{
			(flag == P_NIL) ? flag = P_FRICTION : flag |= P_FRICTION;
		}
		if (gravity)
		{
			(flag == P_NIL) ? flag = P_GRAVITY : flag |= P_GRAVITY;
		}

	}

};