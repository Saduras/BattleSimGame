#pragma once

#include <string>

#include "Engine/Math.h"

namespace Engine::Components
{
	struct Renderable3D
	{
		std::string MaterialID;
		std::string MeshID;
	};

	struct Renderable2D
	{
		struct RenderData2D
		{
			std::string SpriteID;
			Vec2 offset;
		};

		Renderable2D(const std::vector<RenderData2D>& data) : Data(data) { }
		Renderable2D(std::string spriteID, Vec2 offset = Vec2(0.0f, 0.0f)) : Data{ { spriteID, offset } } {}

		std::vector<RenderData2D> Data;
	};
}

namespace Engine
{
	typedef std::vector<Engine::Components::Renderable2D::RenderData2D> ArrayRenderData2D;
}