#include "epch.h"
#include "Camera.h"

#include "Engine/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Components
{
	glm::vec4 Camera::ScreenToView(glm::vec2 screenPoint, float screenWidth, float screenHeight)
	{
		// Note: Screen space has an inverted y-axis (top left is 0,0)
		glm::mat4 screenToView(1);
		screenToView = glm::translate(screenToView, glm::vec3(-1.0f, 1.0f, 0.0f));
		screenToView = glm::scale(screenToView, glm::vec3(2 / screenWidth, -2 / screenHeight, 0.0f));
		return screenToView * glm::vec4(screenPoint, 0.0f, 1.f);
	}

	glm::vec4 Camera::ViewToWorld(glm::vec4 viewPoint)
	{
		return m_ProjInverse * viewPoint;
	}

	glm::vec4 Camera::ScreenToWorld(glm::vec2 screenPoint, float screenWidth, float screenHeight)
	{
		return ViewToWorld(ScreenToView(screenPoint, screenWidth, screenHeight));
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_ZNear(zNear), m_ZFar(zFar)
	{
		m_Proj = glm::ortho(left, right, bottom, top, m_ZNear, m_ZFar);
		m_ProjInverse = glm::inverse(m_Proj);
	}
}