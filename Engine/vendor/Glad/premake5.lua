project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs 
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter "system:macosx"
		systemversion "latest"
		staticruntime "On"
		xcodebuildsettings { ["ALWAYS_SEARCH_USER_PATHS"] = "YES" }

		links
		{
			"Cocoa.framework",
			"IOKit.framework",
			"QuartzCore.framework",
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		runtime "Release"
