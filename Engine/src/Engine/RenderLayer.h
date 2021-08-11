#pragma once
#include "Engine/Layer.h"
#include "Engine/Scene.h"
#include "Engine/Components/Transform.h"
#include "Engine/Components/Renderable.h"

namespace Engine {
	class RenderLayer : public Layer
	{
		public:
			RenderLayer(Scene& scene);
			~RenderLayer();

			void OnUpdate(float deltaTime) override;
		private:
			Scene& m_Scene;

			static void RenderRenderable(const Components::Transform& transform, const Components::Renderable& renderable);
	};
}

