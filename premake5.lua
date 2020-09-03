workspace "BattleSimGame"
	architecture "x64"
	startproject "BattleSimGame"

	configurations 
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Inlcude directories relative to the root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
IncludeDir["Glad"] = "Engine/vendor/Glad/include"
IncludeDir["ImGui"] = "Engine/vendor/imgui/"
IncludeDir["glm"] = "Engine/vendor/glm"

group "Dependencies"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/Glad"
	include "Engine/vendor/imgui"

group ""

project "BattleSimGame"
	location "BattleSimGame"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src",
		"%{IncludeDir.glm}",
	}

	links 
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}
	
	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ENG_DIST"
		runtime "Release"
		optimize "On"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
	}

	links 
	{
		"GLFW",
		"Glad",
		"ImGui",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		pchheader "epch.h"
		pchsource "Engine/src/epch.cpp"

		links
		{
			"opengl32.lib",
		}

		defines
		{
			"ENG_PLATFORM_WINDOWS",
			"ENG_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_GLAD",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/BattleSimGame/\"")
		}
	
	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"
		xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }
		pchheader "src/epch.h"
		pchsource "Engine/src/epch.cpp"

		links
		{
			"Cocoa.framework",
			"IOKit.framework",
			"QuartzCore.framework",
		}

		defines
		{
			"ENG_PLATFORM_MAC",
			"ENG_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_GLAD",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/BattleSimGame/\"")
		}
	
	filter "configurations:Debug"
		defines "ENG_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ENG_DIST"
		runtime "Release"
		optimize "On"