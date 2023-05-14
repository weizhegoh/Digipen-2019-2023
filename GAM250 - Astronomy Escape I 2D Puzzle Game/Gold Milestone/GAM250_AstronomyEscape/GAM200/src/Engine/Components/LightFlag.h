/*****************************************************************
*\file		 LightFlag.h
*\brief		 Light flag component

*\author(s)   Lee Liang Ping	41 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct LightFlag
{
	unsigned int flag{ LIGHT_NIL };

	void Read(int normal, int mc, int ko, int ko_part, int levellight)
	{
		if (normal == 0 && mc == 0 && ko == 0 && ko_part == 0 && levellight == 0)
		{
			flag = LIGHT_NIL;
			return;
		}
		if (normal)
		{
			if (flag == LIGHT_NIL)
				flag = LIGHT_NORMAL;
			else
				flag |= LIGHT_NORMAL;
		}
		if (mc)
		{
			if (flag == LIGHT_NIL)
				flag = LIGHT_MC;
			else
				flag |= LIGHT_MC;
		}
		if (ko)
		{
			if (flag == LIGHT_NIL)
				flag = LIGHT_KO;
			else
				flag |= LIGHT_KO;
		}
		
		if (ko_part)
		{
			if (flag == LIGHT_NIL)
				flag = LIGHT_KO;
			else
				flag |= LIGHT_KO;
		}

	}

};