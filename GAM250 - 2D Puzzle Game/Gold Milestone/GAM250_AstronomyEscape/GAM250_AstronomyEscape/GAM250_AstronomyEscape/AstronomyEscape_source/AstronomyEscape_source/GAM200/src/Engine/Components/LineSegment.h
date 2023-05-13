/*****************************************************************
*\file		 LineSegment.h
*\brief		 LineSegment Component 

*\author(s)   Lee Liang Ping	7 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once

struct LineSegment
{
	glm::vec2 m_pt0;
	glm::vec2 m_pt1;
	glm::vec2 m_normal;
};