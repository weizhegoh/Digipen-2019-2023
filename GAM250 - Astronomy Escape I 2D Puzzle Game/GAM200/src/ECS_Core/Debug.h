/*****************************************************************
*\file		 Debug.h
*\brief		 Debug Assert Code

*\author(s)   Kenric Tan Wei Liang	28 lines x 100% Code contribution

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#ifndef DEBUG
#include <EnginePch.h>
#define ARTLESS_ASSERT(condition, output){ \
	if (!(condition)) \
	  { \
		  struct tm newtime;\
		  time_t now = time(0);\
		  localtime_s(&newtime, &now);\
	      std::ofstream myfile;\
		  myfile.open("../Logs/log.txt", std::fstream::app);\
		  myfile <<"\n\n";\
		  myfile << newtime.tm_mday << "/"<<newtime.tm_mon+1 <<"/" << newtime.tm_year+1900 << std::endl;\
		  myfile << newtime.tm_hour << ":"<<newtime.tm_min <<":" << newtime.tm_sec << std::endl;\
		  myfile << "Error Name: " << output << std::endl;\
		  myfile << "Assertion failed at " << __FILE__ << ":" << __LINE__; \
		  myfile << " inside " << __FUNCTION__ << std::endl; \
		  myfile << "Condition: " << #condition; \
		  myfile.close();\
		  std::cerr << "Error Name: " << output << std::endl;\
		  std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__; \
		  std::cerr << " inside " << __FUNCTION__ << std::endl; \
		  std::cerr << "Condition: " << #condition; \
		  abort();\
	  } \
  }
#else
#define ARTLESS_ASSERT
#endif
