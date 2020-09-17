#include "epch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Components
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
	: m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_ZNear(zNear), m_ZFar(zFar)
	{
		m_Proj = glm::ortho(left, right, bottom, top, m_ZNear, m_ZFar);
	}
}