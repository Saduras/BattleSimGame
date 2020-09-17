#pragma once

#include <glm/glm.hpp>

namespace Engine::Components
{
	struct Transform
	{
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;

		Transform();
		Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
		~Transform();

		glm::mat4 GetTransformationMatrix() const;
	};
}