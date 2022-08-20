#include "epch.h"
#include "Engine/Time.h"

#include <GLFW/glfw3.h>

namespace Engine
{
	/// <summary>
	/// Get current time in seconds.
	/// </summary>
	/// <returns></returns>
	float Time::GetTime()
	{
		return (float)glfwGetTime();
	}
}