workspace "BattleSimGame"
	architecture "x64"

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

include "Engine/vendor/GLFW"

project "BattleSimGame"
	location "BattleSimGame"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Engine/vendor/spdlog/include",
		"Engine/src",
	}

	links 
	{
		"Engine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines "ENG_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ENG_DIST"
		buildoptions "/MD"
		optimize "On"

project "Engine"
	location "Engine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "epch.h"
	pchsource "Engine/src/epch.cpp"

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
	}

	links 
	{
		"GLFW",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENG_PLATFORM_WINDOWS",
			"ENG_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/BattleSimGame")
		}
	
	filter "configurations:Debug"
		defines "ENG_DEBUG;ENG_ENABLE_ASSERTS"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "ENG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "ENG_DIST"
		buildoptions "/MD"
		optimize "On"