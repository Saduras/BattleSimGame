#include "epch.h"
#include "String.h"

namespace Engine::String
{
	std::string ToString(const Transform& transform)
	{
		std::stringstream ss;
		ss << "[";
		ss << "(" << transform.Position.x << ", " << transform.Position.y << ", " << transform.Position.z << "), ";
		ss << "(" << transform.Rotation.x << ", " << transform.Rotation.y << ", " << transform.Rotation.z << "), ";
		ss << "(" << transform.Scale.x << ", " << transform.Scale.y << ", " << transform.Scale.z << ")";
		ss << "]";
		return ss.str();
	}
}
