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

	struct Transform
	{
		Vec3 Position;
		Vec3 Rotation;
		Vec3 Scale;

		Transform() : 
			Position({ 0.0f, 0.0f, 0.0f }), 
			Rotation({ 0.0f, 0.0f, 0.0f }), 
			Scale({ 1.0f, 1.0f, 1.0f }) 
		{ }

		Transform(Vec3 position, Vec3 rotation, Vec3 scale)
			: Position(position), Rotation(rotation), Scale(scale) { }
	};
	
	Mat4 TransformToMatrix(Transform transform);
}