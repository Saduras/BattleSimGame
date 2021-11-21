#include "epch.h"
#include "Transform.h"

namespace Engine::Components
{
	Transform::Transform()
		: Position({ 0.0f, 0.0f, 0.0f }), Rotation({ 0.0f, 0.0f, 0.0f }), Scale({ 1.0f, 1.0f, 1.0f })
	{
	}

	Transform::Transform(Vec3 pos, Vec3 rot, Vec3 scale)
		: Position(pos), Rotation(rot), Scale(scale)
	{
	}

	Transform::~Transform()
	{
	}
	
	Mat4 Transform::GetTransformationMatrix() const
	{
		Mat4 transformMat(1.0f);
		transformMat = Engine::Translate(transformMat, Position);
		transformMat = Engine::Rotate(transformMat, Engine::DegToRad(Rotation.x), Vec3({ 1.0f, 0.0f, 0.0f }));
		transformMat = Engine::Rotate(transformMat, Engine::DegToRad(Rotation.y), Vec3({ 0.0f, 1.0f, 0.0f }));
		transformMat = Engine::Rotate(transformMat, Engine::DegToRad(Rotation.z), Vec3({ 0.0f, 0.0f, 1.0f }));
		transformMat = Engine::Scale(transformMat, Scale);
		return transformMat;
	}
}

