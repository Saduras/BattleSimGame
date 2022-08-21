#include "epch.h"
#include "Render2DSystem.h"

#include "Engine/Application.h"
#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/Renderable.h"
#include "Engine/ECS/Entity.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Sprite.h"
#include "Engine/Renderer/TextureAtlas.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/DebugTool.h"
#include "Engine/Math.h"
#include "Engine/ECS/Systems/TransformAnimationSystem.h"
#include "Engine/String.h"

namespace Engine::Systems {
	void Render2DSystem::Execute(float deltaTime)
	{
		using namespace Engine::Components;

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		auto cameras = m_Scene->GetView<Transform, OrthographicCamera>();
		cameras.each([this](Transform& transform, OrthographicCamera& camera) {
			Mat4 viewMatrix = TransformToMatrix(transform);
			Mat4 projMatrix = camera.GetProjectionMatrix();

			Engine::Renderer::BeginScene(projMatrix * viewMatrix);

			// Sort entities according to z
			m_Scene->Sort<Transform>([](const Transform& lhs, const Transform& rhs) {
				return lhs.Position.z > rhs.Position.z;
			});

			// Render all entities with transform, mesh and material
			auto renderableView = m_Scene->GetView<Transform>();
			renderableView.each([this](entt::entity entityID, const Transform& transform)
				{
					Entity entity(entityID, m_Scene);
					if (!entity.HasComponent<Components::Renderable2D>())
						return;

					Render2DSystem::RenderRenderable(entity, transform, entity.GetComponent<Components::Renderable2D>());
				}
			);

			Debug::Render();

			Engine::Renderer::EndScene();
			});
	}

	void Render2DSystem::RenderRenderable(Entity entity, const Transform& transform, const Components::Renderable2D& renderable)
	{
		for (const Components::Renderable2D::RenderData2D& data : renderable.Data)
		{
			Sprite& sprite = AssetRegistry::Get<Sprite>(data.SpriteUUID);
			Shader& shader = AssetRegistry::Get<Shader>(sprite.GetShaderUUID());
			TextureAtlas& atlas = AssetRegistry::Get<TextureAtlas>(sprite.GetAtlasUUID());
			Mesh& mesh = AssetRegistry::Get<Mesh>(sprite.GetMeshUUID());

			atlas.Bind();
			Renderer::SetShader(&shader);
			Mat4 modelMatrix = TransformToMatrix(transform);

			if (entity.HasComponent<Components::Animator>())
				modelMatrix = modelMatrix * TransformToMatrix(entity.GetComponent<Components::Animator>().Offset);

			shader.SetProperty("u_Model", modelMatrix);
			shader.SetProperty("u_Color", sprite.GetColor());
			shader.SetProperty("u_TextureSize", atlas.GetSize());
			shader.ApplyProperties();
			Renderer::Submit(mesh.GetVertexArray());
		}
	}
}

