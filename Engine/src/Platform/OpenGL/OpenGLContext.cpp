#include "epch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Engine
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		ENG_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ENG_CORE_ASSERT(status, "Failed to initialize Glad!");

		ENG_CORE_INFO("OpenGL Info:");
		ENG_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		ENG_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		ENG_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}