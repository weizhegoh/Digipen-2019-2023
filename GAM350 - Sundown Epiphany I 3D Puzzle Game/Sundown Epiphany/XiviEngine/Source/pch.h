/*****************************************************************
*\file         pch.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#pragma warning (disable: 4201)
#pragma warning (disable: 4005)
#pragma warning (disable: 4348)
#pragma warning (disable: 4456)
#pragma warning (disable: 4189)
#pragma warning (disable: 4996)
#pragma warning (disable: 4267)
#pragma warning (disable: 4390)

#define RAPIDJSON_HAS_STDSTRING 1

// SIMD
#include <immintrin.h>

// STD Core
#include <type_traits>
#include <cstdint>
#include <climits>
#include <cmath>
#include <future>
#include <bit>
#include <chrono>
#include <thread>
#include <random>
#include <concepts>
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <filesystem>
#include <atomic>
#include <fstream>
#include <numeric>
#include <random>

#include <iostream>
#include <chrono>
#include <sstream>

// STD Debug
#include <stdexcept>
#include <cassert>

// STD Containers
#include <STL/array.h>
#include <STL/vector.h>
#include <STL/span.h>
#include <STL/list.h>
#include <STL/forward_list.h>
#include <STL/deque.h>
#include <STL/queue.h>
#include <STL/stack.h>
#include <STL/map.h>
#include <STL/unordered_map.h>
#include <STL/set.h>
#include <STL/unordered_set.h>
#include <tuplet/tuple.hpp>
#include <string>
#include <string_view>
#include <iterator>

// External Libaries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/norm.hpp >
#include <gtx/matrix_transform_2d.hpp>
#include <gtx/component_wise.hpp>
#include <gtx/euler_angles.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/matrix_inverse.hpp>
#include <gtc/matrix_access.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/component_wise.hpp>
#include <gtx/string_cast.hpp>
#include <rttr/registration.h>
#include <fmod_studio.hpp>

// Base
#include <Core.h>
#include <Log.h>

// Types
#include <Types/Base.h>
#include <Types/GUID.h>
#include <Types/Traits/Specialised.h>
#include <Types/Traits/IsArrayType.h>
#include <Types/Traits/Function.h>
#include <Types/Traits/Tuple.h>
#include <Types/Typelist/Typelist.h>

// Utils
#include <Utils/Assert.h>
#include <Utils/Enumerate.h>
#include <Utils/NonCopyable.h>
#include <Utils/Singleton.h>
#include <Utils/Typename.h>
#include <Utils/XXH3.h>

#include <Service/Locator.h>

// System
#include "Xivi/Core/Utils/Utils.h"
#include "Xivi/Core/Utils/Profiler/Profiler.h"
#include "Xivi/Core/Timer/TimerEngine.h"

// Putting here to utilise engine math
#define XVMATH
#include <Input/Manager.h>