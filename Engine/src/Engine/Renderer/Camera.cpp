#include "epch.h"
#include "Camera.h"

namespace Engine
{
	void Camera::SetPosition(glm::vec3 position)
	{
		m_Position = position;
		m_View = glm::translate(glm::mat4(1), -position);
	}

	OrthographicCamera::OrthographicCamera(glm::vec3 position, float width, float height, float zNear, float zFar)
	: m_Width(width), m_Height(height), m_ZNear(zNear), m_ZFar(zFar)
	{
		m_Proj = glm::ortho(0.0f, m_Width, 0.0f, m_Height, m_ZNear, m_ZFar);
		SetPosition(position);
	}
}