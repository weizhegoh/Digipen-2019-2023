project "XiviEngine"
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
    "GLEW_STATIC",
  }

  files
  {
    "Source/**.h",
    "Source/**.cpp",
    "Source/**.inl",
    "%{IncludeDir.imgui}/*.h",
    "%{IncludeDir.imgui}/*.cpp",
    "%{IncludeDir.imgui}/*.inl",
    "%{IncludeDir.imgui_gizmo}/*.cpp",
    "%{IncludeDir.imgui_gizmo}/*.h",
    "%{IncludeDir.imgui_gizmo}/*.inl",
    "%{IncludeDir.imgui}/backends/imgui_impl_opengl3.h",
    "%{IncludeDir.imgui}/backends/imgui_impl_opengl3.cpp",
    "%{IncludeDir.imgui}/backends/imgui_impl_glfw.h",
    "%{IncludeDir.imgui}/backends/imgui_impl_glfw.cpp"
  }

  removefiles 
  {
    "Source/Xivi/Platform/**"
  }

  files
  {
    "Source/Xivi/Platform/Platform.h",
  }

  includedirs
  {
    "Source",
    "%{IncludeDir.glew}",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.spdlog}",
    "%{IncludeDir.tuplet}",
    "%{IncludeDir.Vulkan}",
    "%{IncludeDir.fmod}",
    "%{IncludeDir.fmod_studio}",
    "%{IncludeDir.tracy}",
    "%{IncludeDir.rttr}",
    "%{IncludeDir.rapidjson}",
    "%{IncludeDir.XVUtils}",
    "%{IncludeDir.XVResource}",
    "%{IncludeDir.XVEvents}",
    "%{IncludeDir.XVServiceLocator}",
    "%{IncludeDir.XVInput}",
    "%{IncludeDir.imgui}",
    "%{IncludeDir.imgui_gizmo}",    
    "%{IncludeDir.imterm}",
    "%{IncludeDir.icon}",    
  }

  filter 'files:Dep/**.cpp'
    flags  { 'NoPCH' }

    filter 'files:Source/Xivi/Graphics/**.cpp'
    flags  { 'NoPCH' }

    filter 'files:Source/Xivi/Utils/Math/**.cpp'
    flags  { 'NoPCH' }

  filter "system:windows"
    defines "XV_PLATFORM_WINDOWS"
    staticruntime "off"
    systemversion "latest"

    pchheader ("pch.h")
    pchsource ("Source/pch.cpp")
    
    files
    {
      "Source/Xivi/Platform/Windows/GLFW/**.h",
      "Source/Xivi/Platform/Windows/GLFW/**.cpp",
      "Source/Xivi/Platform/Graphics/OpenGL/**.h",
      "Source/Xivi/Platform/Graphics/OpenGL/**.cpp",
      "Source/Xivi/Platform/Graphics/Vulkan/**.h",
      "Source/Xivi/Platform/Graphics/Vulkan/**.cpp"
    }

  filter "platforms:Vulkan"
    defines
    {
      "XV_RENDER_VULKAN"
    }
    includedirs 
    { 
      "$(VULKAN_SDK)/include",
    }
    links
    {
      "$(VULKAN_SDK)/lib/vulkan-1.lib",
    }

  filter "platforms:OpenGL"
    defines
    {
      "XV_RENDER_OPENGL"
    }
    links
    {
      "opengl32.lib"
    }

  filter "platforms:VulkanEditor"
    defines
    {
      "XV_RENDER_VULKAN",
      "XV_EDITOR"
    }
    includedirs 
    { 
      "$(VULKAN_SDK)/include",
    }
    links
    {
      "$(VULKAN_SDK)/lib/vulkan-1.lib",
    }

  filter "platforms:OpenGLEditor"
    defines
    {
      "XV_RENDER_OPENGL",
      "XV_EDITOR"
    }
    links
    {
      "opengl32.lib"
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
    links
    {
      "%{Library.GLFW_Debug}",
      "%{Library.fmod_Debug}",
      "%{Library.fmod_Debug_DLL}",
      "%{Library.fmod_Studio_Debug}",
      "%{Library.fmod_Studio_Debug_DLL}",
      "%{Library.glew_Debug}",
      "%{Library.rttr_Debug}",
    }
    files
    {
      "%{IncludeDir.tracy}/TracyClient.cpp",
    }
    postbuildcommands
    {
      "{COPY} %{Library.fmod_Studio_Debug_DLL} %{settings.targetdir}",
      "{COPY} %{Library.fmod_Debug_DLL} %{settings.targetdir}",
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
    links
    {
      "%{Library.GLFW_Release}",
      "%{Library.fmod_Release}",
      "%{Library.fmod_Release_DLL}",
      "%{Library.fmod_Studio_Release}",
      "%{Library.fmod_Studio_Release_DLL}",    
      "%{Library.glew_Release}",
      "%{Library.rttr_Release}",
    }
    files
    {
      "%{IncludeDir.tracy}/TracyClient.cpp",
    }
    postbuildcommands
    {
      "{COPY} %{Library.fmod_Studio_Release_DLL} %{settings.targetdir}",
      "{COPY} %{Library.fmod_Release_DLL} %{settings.targetdir}",
    }

  filter "configurations:Release"
    runtime "Release"
    optimize "Full"
    defines
    {
      "XV_RELEASE"
    }
    links
    {
      "%{Library.GLFW_Release}",
      "%{Library.fmod_Release}",
      "%{Library.fmod_Release_DLL}",
      "%{Library.fmod_Studio_Release}",
      "%{Library.fmod_Studio_Release_DLL}",
      "%{Library.glew_Release}",
      "%{Library.rttr_Release}",
    }
    postbuildcommands
    {
      "{COPY} %{Library.fmod_Studio_Release_DLL} %{settings.targetdir}",
      "{COPY} %{Library.fmod_Release_DLL} %{settings.targetdir}",
    }