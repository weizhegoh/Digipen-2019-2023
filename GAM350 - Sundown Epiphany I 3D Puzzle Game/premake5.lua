require "Scripts/premake-settings"
require "Scripts/premake-dependencies"
require "Scripts/premake-triggers"

-- Specify your workspace name
workspace(settings.workspace)
  startproject "SundownLauncher"
    location "Build"
    flags {
      "MultiProcessorCompile",
      "LinkTimeOptimization"
    }
    preferredtoolarchitecture "x86_64"
    floatingpoint "Fast"
    targetdir("%{settings.targetdir}/")
    objdir("%{settings.objdir}/")
    warnings "Extra"

    if _OPTIONS["Architecture"] == "arm" then
      architecture "ARM"
    elseif _OPTIONS["Architecture"] == "x64" then
      architecture "x86_64"
    end

    -- Configurations List
    platforms
    {
      "OpenGLEditor",
      "OpenGL",
      "VulkanEditor",
      "Vulkan"
    }
    
    configurations
    {
      "Debug",
      "Development",
      "Release"
    }

    include "SundownLauncher"
    include "Sundown"
    include "XiviEngine"