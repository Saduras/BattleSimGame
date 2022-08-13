#include "epch.h"
#include "Math.h"

namespace Engine
{
    Mat4 TransformToMatrix(Transform transform)
    {
		Mat4 transformMat(1.0f);
		transformMat = Translate(transformMat, transform.Position);
		transformMat = Rotate(transformMat, DegToRad(transform.Rotation.x), Vec3({ 1.0f, 0.0f, 0.0f }));
		transformMat = Rotate(transformMat, DegToRad(transform.Rotation.y), Vec3({ 0.0f, 1.0f, 0.0f }));
		transformMat = Rotate(transformMat, DegToRad(transform.Rotation.z), Vec3({ 0.0f, 0.0f, 1.0f }));
		transformMat = Scale(transformMat, transform.Scale);
		return transformMat;
    }
}

