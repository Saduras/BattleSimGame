#pragma once

#include "Engine/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/common.hpp>

namespace Engine
{
	using Vec2 = glm::vec2;
	using Vec3 = glm::vec3;
	using Vec4 = glm::vec4;
	using Mat4 = glm::mat4;
	
	ALIAS_TEMPLATE_FUNCTION(Sign, glm::sign);
	ALIAS_TEMPLATE_FUNCTION(Magnitude, glm::length);
	ALIAS_TEMPLATE_FUNCTION(Normalize, glm::normalize);
	ALIAS_TEMPLATE_FUNCTION(Translate, glm::translate);
	ALIAS_TEMPLATE_FUNCTION(Rotate, glm::rotate);
	ALIAS_TEMPLATE_FUNCTION(Scale, glm::scale);
	ALIAS_TEMPLATE_FUNCTION(DegToRad, glm::radians);
	ALIAS_TEMPLATE_FUNCTION(Ortho, glm::ortho);
	ALIAS_TEMPLATE_FUNCTION(Inverse, glm::inverse);
	ALIAS_TEMPLATE_FUNCTION(RotateX, glm::rotateX)
	ALIAS_TEMPLATE_FUNCTION(RotateY, glm::rotateY)
	ALIAS_TEMPLATE_FUNCTION(RotateZ, glm::rotateZ)
}