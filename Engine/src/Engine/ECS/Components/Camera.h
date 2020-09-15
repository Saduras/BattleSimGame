#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine/ECS/Component.h"

namespace Engine::Components
{
	class Camera : public Component
	{
	public:
		virtual ~Camera() {};

		inline glm::mat4 GetProjectionMatrix() { return m_Proj; }
	protected:
		glm::mat4 m_Proj;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -5.0f, float zFar = 5.0f);
	private:
		float m_Left;
		float m_Right;
		float m_Bottom;
		float m_Top;
		float m_ZNear;
		float m_ZFar;
	};
}