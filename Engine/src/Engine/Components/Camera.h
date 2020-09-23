#pragma once

#include <glm/glm.hpp>

namespace Engine::Components
{
	class Camera
	{
	public:
		virtual ~Camera() {};

		inline glm::mat4 GetProjectionMatrix() { return m_Proj; }
		inline glm::mat4 GetProjectionMatrixInverse() { return m_ProjInverse; }
		glm::vec4 ScreenToView(glm::vec2 screenPoint, float screenWidth, float screenHeight);
		glm::vec4 ViewToWorld(glm::vec4 viewPoint);
		glm::vec4 ScreenToWorld(glm::vec2 screenPoint, float screenWidth, float screenHeight);
	protected:
		glm::mat4 m_Proj;
		glm::mat4 m_ProjInverse;
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