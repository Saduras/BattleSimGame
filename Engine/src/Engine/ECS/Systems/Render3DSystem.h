#pragma once

#include "Engine/ECS/System.h"
#include "Engine/ECS/Components/Renderable.h"

namespace Engine::Systems {
	class Render3DSystem : public System
	{
	public:
		Render3DSystem(Scene* scene) : System(scene) {}

		void Execute(float deltaTime) override;
		std::string GetName() const override { return "Render3DSystem"; }

	private:
		static void RenderRenderable(const Transform& transform, const Components::Renderable3D& renderable);
	};
}