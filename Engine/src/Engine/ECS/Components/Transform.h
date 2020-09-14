#pragma once

#include <glm/glm.hpp>

#include "Engine/ECS/Component.h"

namespace Engine::Components
{
	class Transform : Component
	{
	public:
		Transform();
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
		~Transform();

		glm::mat4 GetTransformationMatrix() const;

		inline glm::vec3 GetPosition() const { return m_Position; }
		inline glm::vec3 GetRotation() const { return m_Rotation; }
		inline glm::vec3 GetScale() const { return m_Scale; }

		inline void SetPosition(glm::vec3 position) { m_Position = position; }
		inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; }
		inline void SetScale(glm::vec3 scale) { m_Scale = scale; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};
}