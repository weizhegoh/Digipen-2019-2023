/*****************************************************************
*\file         EntryPoint.h
*\author(s)    Tan Tong Wee

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#pragma once
#include "pch.h"
#include "Core/Application.h"

#ifdef XV_PLATFORM_WINDOWS
extern Xivi::Ptr<Xivi::Application> Xivi::CreateApplication();

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, int )
{
  auto app = Xivi::CreateApplication();
  app->Run();
  delete app;
}
#else
extern Xivi::Ptr<Xivi::Application> Xivi::CreateApplication();

int main( int argc, char **argv )
{
  auto app = Xivi::CreateApplication();
  app->Run();
  delete app;
}
#endif