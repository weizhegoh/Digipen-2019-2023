project "Sundown"
  kind "StaticLib"
  language "C++"
  cppdialect "C++latest"
  editandcontinue "Off"
  characterset ("MBCS")
  system "windows"

  buildoptions
  {
    "/bigobj"
  }

  defines
  {
    "EXPORTDLL",
  }

  files
  {
    "Source/**.h",
    "Source/**.cpp",
    "Source/**.inl",
  }

  includedirs
  {
    "%{IncludeDir.glew}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.tuplet}",
    "%{IncludeDir.Vulkan}",
    "%{IncludeDir.fmod}",
    "%{IncludeDir.fmod_studio}",
    "%{IncludeDir.tracy}",
    "%{IncludeDir.imgui}",
    "%{IncludeDir.imgui_gizmo}",
    "%{IncludeDir.imterm}",
    "%{IncludeDir.rttr}",
    "%{IncludeDir.icon}",
    "%{IncludeDir.rapidjson}",
    "%{IncludeDir.XVUtils}",
    "%{IncludeDir.XVResource}",
    "%{IncludeDir.XVEvents}",
    "%{IncludeDir.XVServiceLocator}",
    "%{IncludeDir.XVInput}",
  }

  includedirs
  {
    "../XiviEngine/Source",
    "Source",
  }

  links
  {
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
      "XV_PROFILE",
      "TRACY_ENABLE",
      "TRACY_ON_DEMAND"
    }

  filter "configurations:Development"
    runtime "Release"
    optimize "On"
    defines
    {
      "XV_DEV",
      "XV_PROFILE",
      "TRACY_ENABLE",
      "TRACY_ON_DEMAND"
    }

  filter "configurations:Release"
    runtime "Release"
    optimize "Full"
    defines
    {
      "XV_RELEASE"
    }