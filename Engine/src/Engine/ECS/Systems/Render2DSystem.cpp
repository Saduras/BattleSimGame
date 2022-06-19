#include "epch.h"
#include "Render2DSystem.h"

#include "Engine/Application.h"
#include "Engine/ECS/Components/Camera.h"
#include "Engine/ECS/Components/Renderable.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Sprite.h"
#include "Engine/Renderer/TextureAtlas.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine::Systems {
	void Render2DSystem::Execute(float deltaTime)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		auto cameras = m_Scene->GetView<Components::Transform, Components::OrthographicCamera>();
		cameras.each([this](Components::Transform& transform, Components::OrthographicCamera& camera) {
			Mat4 viewMatrix = transform.GetTransformationMatrix();
			Mat4 projMatrix = camera.GetProjectionMatrix();

			Engine::Renderer::BeginScene(projMatrix * viewMatrix);

			// Render all entities with transform, mesh and material
			auto renderableView = m_Scene->GetView<Components::Transform, Components::Renderable2D>();
			renderableView.each(Render2DSystem::RenderRenderable);

			Engine::Renderer::EndScene();
			});
	}

	void Render2DSystem::RenderRenderable(const Components::Transform& transform, const Components::Renderable2D& renderable)
	{
		for (const Components::Renderable2D::RenderData2D& data : renderable.Data)
		{
			Sprite& sprite = AssetRegistry::Get<Sprite>(data.SpriteID);
			Shader& shader = AssetRegistry::Get<Shader>(sprite.GetShaderID());
			TextureAtlas& atlas = AssetRegistry::Get<TextureAtlas>(sprite.GetAtlasID());

			Mesh mesh(GetMeshDataForSubTexture(atlas, sprite.GetIndex()));

			atlas.Bind();
			Renderer::SetShader(&shader);
			Mat4 modelMatrix = transform.GetTransformationMatrix();
			shader.SetProperty("u_Model", modelMatrix);
			shader.SetProperty("u_Color", sprite.GetColor());
			shader.SetProperty("u_TextureSize", atlas.GetSize());
			shader.ApplyProperties();
			Renderer::Submit(mesh.GetVertexArray());
		}
	}
}

