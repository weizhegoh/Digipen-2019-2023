/*****************************************************************
*\file		 TemplateFunctions.h
*\brief		 Helper Function that performs clear

*\author(s)   Lee Liang Ping	13 lines x % Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/


#pragma once
#include <EnginePch.h>


template<class T>
T& clear(T& classClear) {
	T emptyClass;
	std::swap(emptyClass, classClear);
	return classClear;
}

void clear(std::queue<int>& q)
{
	std::queue<int> empty;
	std::swap(q, empty);
}
