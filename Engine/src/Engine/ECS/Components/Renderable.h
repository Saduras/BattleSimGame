#pragma once

#include <string>

namespace Engine::Components
{
	struct Renderable
	{
		std::string MaterialID;
		std::string MeshID;
	};
}