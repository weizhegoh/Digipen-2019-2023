/*****************************************************************
*\file         SundownLauncher.cpp
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#include "Sundown.h"

#ifdef XV_PLATFORM_WINDOWS
#include <Windows.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
  _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
  (void)hInstance;
  SundownEXE::Launch();
  return 0;
}
#else

int main(int argc, char** argv)
{
  SundownEXE::Launch();
}
#endif
