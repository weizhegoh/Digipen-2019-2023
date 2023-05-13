/*****************************************************************
*\file		 EnginePch.h
*\brief		 PCH

*\author(s)   Lee Liang Ping	58 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable : 4005)
//----------------------------------
// Standard Libaries
//----------------------------------
#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <string>
#include <iomanip> 
#include <memory>
#include <cassert>
#include <ctime>
#include <filesystem>
#include <cstdint>
#include <random>
#include <math.h>
#include<stdarg.h>
//----------------------------------
// Contianers Libaries
//----------------------------------
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>
#include <iterator>
#include <utility>
#include <functional>
#include <bitset>

//----------------------------------
// External Libaries
//----------------------------------
#include "fmod.hpp"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp >
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/ext/matrix_projection.hpp >
//----------------------------------
// Own Libaries
//----------------------------------
#include <Debug.h>
#include <matrix3x3.h>
#include <vector3D.h>
#include <MathsFunctions.h>
#include <EnumToString.h>
#include <TemplateFunctions.h>
//----------------------------------
// ECS Manager
//----------------------------------
#include <ComponentList.h>
#include <EntityManager.h>
#include <ComponentManager.h>
#include <SystemManager.h>
#include <ECSManager.h>
#include <EventBus.h>
#include <MemoryManager.h>
#include <System.h>


