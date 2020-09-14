#include "epch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine::Components
{
	Transform::Transform()
		: m_Position({ 0.0f, 0.0f, 0.0f }), m_Rotation({ 0.0f, 0.0f, 0.0f }), m_Scale({ 1.0f, 1.0f, 1.0f })
	{
	}

	Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
		: m_Position(pos), m_Rotation(rot), m_Scale(scale)
	{
	}

	Transform::~Transform()
	{
	}
	
	glm::mat4 Transform::GetTransformationMatrix() const
	{
		glm::mat4 transformMat(1.0f);
		glm::translate(transformMat, m_Position);
		glm::rotate(transformMat, glm::radians(m_Rotation.x), glm::vec3({ 1.0f, 0.0f, 0.0f }));
		glm::rotate(transformMat, glm::radians(m_Rotation.y), glm::vec3({ 0.0f, 1.0f, 0.0f }));
		glm::rotate(transformMat, glm::radians(m_Rotation.z), glm::vec3({ 0.0f, 0.0f, 1.0f }));
		glm::scale(transformMat, m_Scale);
		return transformMat;
	}
}

