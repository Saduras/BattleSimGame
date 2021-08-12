#include "epch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Components
{
	Transform::Transform()
		: Position({ 0.0f, 0.0f, 0.0f }), Rotation({ 0.0f, 0.0f, 0.0f }), Scale({ 1.0f, 1.0f, 1.0f })
	{
	}

	Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
		: Position(pos), Rotation(rot), Scale(scale)
	{
	}

	Transform::~Transform()
	{
	}
	
	glm::mat4 Transform::GetTransformationMatrix() const
	{
		glm::mat4 transformMat(1.0f);
		transformMat = glm::translate(transformMat, Position);
		transformMat = glm::rotate(transformMat, glm::radians(Rotation.x), glm::vec3({ 1.0f, 0.0f, 0.0f }));
		transformMat = glm::rotate(transformMat, glm::radians(Rotation.y), glm::vec3({ 0.0f, 1.0f, 0.0f }));
		transformMat = glm::rotate(transformMat, glm::radians(Rotation.z), glm::vec3({ 0.0f, 0.0f, 1.0f }));
		transformMat = glm::scale(transformMat, Scale);
		return transformMat;
	}
}

