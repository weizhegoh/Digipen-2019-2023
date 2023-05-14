-- Dependencies

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/../XiviEngine/Dep/GLFW/Include"
IncludeDir["glm"] = "%{wks.location}/../XiviEngine/Dep/glm/Include"
IncludeDir["fmod"] = "%{wks.location}/../XiviEngine/Dep/fmod/core/Include"
IncludeDir["fmod_studio"] = "%{wks.location}/../XiviEngine/Dep/fmod/studio/Include"
IncludeDir["glew"] = "%{wks.location}/../XiviEngine/Dep/glew/Include"
IncludeDir["tracy"] = "%{wks.location}/../XiviEngine/Dep/tracy"
IncludeDir["rttr"] = "%{wks.location}/../XiviEngine/Dep/rttr"
IncludeDir["rapidjson"] = "%{wks.location}/../XiviEngine/Dep/rapidjson/Include"
IncludeDir["assimp"] = "%{wks.location}/../XiviEngine/Dep/assimp/include"
IncludeDir["zlib"] = "%{wks.location}/../XiviEngine/Dep/zlib"
IncludeDir["spdlog"] = "%{wks.location}/../XiviEngine/Dep/XVUtils/Dep/spdlog/include"
IncludeDir["tuplet"] = "%{wks.location}/../XiviEngine/Dep/XVUtils/Dep/tuplet/include"
IncludeDir["XVUtils"] = "%{wks.location}/../XiviEngine/Dep/XVUtils/Include"
IncludeDir["XVResource"] = "%{wks.location}/../XiviEngine/Dep/XVResource/Include"
IncludeDir["XVEvents"] = "%{wks.location}/../XiviEngine/Dep/XVEvents/Include"
IncludeDir["XVServiceLocator"] = "%{wks.location}/../XiviEngine/Dep/XVServiceLocator/Include"
IncludeDir["XVInput"] = "%{wks.location}/../XiviEngine/Dep/XVInput/Include"

-- Editor Dependencies
IncludeDir["imgui"] = "%{wks.location}/../XiviEngine/Dep/imgui"
IncludeDir["imgui_gizmo"] = "%{wks.location}/../XiviEngine/Dep/ImGuizmo"
IncludeDir["imterm"] = "%{wks.location}/../XiviEngine/Dep/ImTerm/include"
IncludeDir["icon"] = "%{wks.location}/../XiviEngine/Dep/IconFontCppHeaders"

LibraryDir = {}
LibraryDir["GLFW"] = "%{wks.location}/../XiviEngine/Dep/GLFW/Lib"
LibraryDir["fmod"] = "%{wks.location}/../XiviEngine/Dep/fmod/core/Lib"
LibraryDir["fmod_studio"] = "%{wks.location}/../XiviEngine/Dep/fmod/studio/Lib"
LibraryDir["glew"] = "%{wks.location}/../XiviEngine/Dep/glew/Lib"
LibraryDir["rttr"] = "%{wks.location}/../XiviEngine/Dep/rttr/Lib"
LibraryDir["assimp"] = "%{wks.location}/../XiviEngine/Dep/assimp/lib"
LibraryDir["zlib"] = "%{wks.location}/../XiviEngine/Dep/zlib/Lib"

Library = {}
Library["GLFW_Release"] = "%{LibraryDir.GLFW}/glfw3.lib"
Library["fmod_Release_DLL"] = "%{LibraryDir.fmod}/fmod.dll"
Library["fmod_Release"] = "%{LibraryDir.fmod}/fmod_vc.lib"
Library["fmod_Studio_Release_DLL"] = "%{LibraryDir.fmod_studio}/fmodstudio.dll"
Library["fmod_Studio_Release"] = "%{LibraryDir.fmod_studio}/fmodstudio_vc.lib"
Library["glew_Release"] = "%{LibraryDir.glew}/libglew32.lib"
Library["rttr_Release"] = "%{LibraryDir.rttr}/Release/librttr_core.lib"
Library["zlib_Release"] = "%{LibraryDir.zlib}/Release/zlibstatic.lib"
Library["assimp_Release"] = "%{LibraryDir.assimp}/Release/assimp-vc142-mt.lib"
Library["assimp_Release_DLL"] = "%{LibraryDir.assimp}/Release/assimp-vc142-mt.dll"


Library["GLFW_Debug"] = "%{LibraryDir.GLFW}/glfw3d.lib"
Library["fmod_Debug_DLL"] = "%{LibraryDir.fmod}/fmodL.dll"
Library["fmod_Debug"] = "%{LibraryDir.fmod}/fmodL_vc.lib"
Library["fmod_Studio_Debug_DLL"] = "%{LibraryDir.fmod_studio}/fmodstudioL.dll"
Library["fmod_Studio_Debug"] = "%{LibraryDir.fmod_studio}/fmodstudioL_vc.lib"
Library["glew_Debug"] = "%{LibraryDir.glew}/libglew32d.lib"
Library["rttr_Debug"] = "%{LibraryDir.rttr}/Debug/librttr_core_d.lib"
Library["zlib_Debug"] = "%{LibraryDir.zlib}/Debug/zlibstaticd.lib"
Library["assimp_Debug"] = "%{LibraryDir.assimp}/Debug/assimp-vc142-mtd.lib"
Library["assimp_Debug_DLL"] = "%{LibraryDir.assimp}/Debug/assimp-vc142-mtd.dll"