#pragma once

#include <string>

#include "Engine/Math.h"
#include "Engine/UUID.h"

namespace Engine::Components
{
	struct Renderable3D
	{
		UUID MateriaUUID;
		UUID MeshUUID;
	};

	struct Renderable2D
	{
		struct RenderData2D
		{
			UUID SpriteUUID;
			Vec2 offset;
		};

		Renderable2D(const std::vector<RenderData2D>& data) : Data(data) { }
		Renderable2D(UUID spriteUUID, Vec2 offset = Vec2(0.0f, 0.0f)) : Data{ { spriteUUID, offset } } {}

		std::vector<RenderData2D> Data;
	};
}

namespace Engine
{
	typedef std::vector<Engine::Components::Renderable2D::RenderData2D> ArrayRenderData2D;
}