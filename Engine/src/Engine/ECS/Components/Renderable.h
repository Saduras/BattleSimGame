#pragma once

#include <string>

namespace Engine::Components
{
	struct Renderable3D
	{
		std::string MaterialID;
		std::string MeshID;
	};

	struct Renderable2D
	{
		std::string SpriteID;
		std::string MeshID;
	};
}