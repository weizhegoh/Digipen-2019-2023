project "SundownLauncher"
  kind "WindowedApp"
  language "C++"
  cppdialect "C++latest"
  editandcontinue "Off"
  characterset ("MBCS")


  files
  {
    "Source/**.h",
    "Source/**.cpp",
    "Source/**.inl"
  }

  includedirs
  {
    "../Sundown/Source"
  }

  links
  {
    "Sundown",
    "XiviEngine"
  }

filter "system:windows"
  cppdialect "C++latest"
  staticruntime "off"
  systemversion "latest"
  defines "XV_PLATFORM_WINDOWS"


  filter "platforms:Vulkan"
    defines
    {
      "XV_RENDER_VULKAN"
    }

  filter "platforms:OpenGL"
    defines
    {
      "XV_RENDER_OPENGL"
    }

  filter "platforms:VulkanEditor"
    defines
    {
      "XV_RENDER_VULKAN",
      "XV_EDITOR"
    }

  filter "platforms:OpenGLEditor"
    defines
    {
      "XV_RENDER_OPENGL",
      "XV_EDITOR"
    }

  filter "configurations:Debug"
    targetsuffix "-d"
    runtime "Debug"
    symbols "On"
    defines
    {
      "XV_DEBUG",
    }

  filter "configurations:Development"
    runtime "Release"
    optimize "On"
    defines
    {
      "XV_DEV",
      "XV_PROFILE"
    }

  filter "configurations:Release"
    runtime "Release"
    optimize "Full"
    defines
    {
      "XV_RELEASE"
    }