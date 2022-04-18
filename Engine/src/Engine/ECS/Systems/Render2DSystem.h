#pragma once

#include "Engine/ECS/System.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Renderable.h"

namespace Engine::Systems {
	class Render2DSystem : public System
	{
	public:
		Render2DSystem(Scene* scene) : System(scene) {}

		void Execute(float deltaTime) override;
		std::string GetName() const override { return "Render2DSystem"; }

	private:
		static void RenderRenderable(const Components::Transform& transform, const Components::Renderable2D& renderable);
	};
}