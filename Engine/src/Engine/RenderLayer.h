#pragma once
#include "Engine/Layer.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Renderable.h"

namespace Engine {
	class RenderLayer : public Layer
	{
		public:
			RenderLayer();
			~RenderLayer();

			void OnUpdate(float deltaTime) override;
		private:
			static void RenderRenderable(const Components::Transform& transform, const Components::Renderable& renderable);
	};
}

