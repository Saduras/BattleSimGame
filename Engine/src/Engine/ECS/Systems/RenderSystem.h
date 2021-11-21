#pragma once

#include "Engine/ECS/System.h"
#include "Engine/ECS/Components/Transform.h"
#include "Engine/ECS/Components/Renderable.h"

namespace Engine::Systems {
	class RenderSystem : public System
	{
	public:
		RenderSystem(Scene* scene) : System(scene) {}

		void Execute(float deltaTime) override;
		std::string GetName() const override { return "RenderSystem"; }

	private:
		static void RenderRenderable(const Components::Transform& transform, const Components::Renderable3D& renderable);
	};
}