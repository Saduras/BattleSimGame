#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engine
{
	class Camera
	{
	public:
		virtual ~Camera() {};

		void SetPosition(glm::vec3 position);
		inline glm::vec3 GetPosition() const { return m_Position; }

		inline glm::mat4 GetPV() { return m_Proj * m_View; }
	protected:
		glm::vec3 m_Position;
		glm::mat4 m_View;
		glm::mat4 m_Proj;
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(glm::vec3 position, float width, float height, float zNear = -5.0f, float zFar = 5.0f); 
	private:
		float m_Width;
		float m_Height;
		float m_ZNear;
		float m_ZFar;
	};
}