#pragma once

#include <Engine/Math.h>

namespace Engine::Components
{
	struct Transform
	{
		Vec3 Position;
		Vec3 Rotation;
		Vec3 Scale;

		Transform();
		Transform(Vec3 position, Vec3 rotation, Vec3 scale);
		~Transform();

		Mat4 GetTransformationMatrix() const;
	};
}