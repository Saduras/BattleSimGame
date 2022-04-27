#include "epch.h"
#include "Render2DSystem.h"

#include "Engine/ECS/Components/Camera.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Sprite.h"
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
		Sprite& sprite = AssetRegistry::Get<Sprite>(renderable.SpriteID);
		Mesh& mesh = AssetRegistry::Get<Mesh>(renderable.MeshID);
		Shader& shader = AssetRegistry::Get<Shader>(sprite.GetShaderID());

		sprite.Bind();
		Renderer::SetShader(&shader);
		Mat4 modelMatrix = transform.GetTransformationMatrix();
		shader.SetUniformMat4f("u_Model", modelMatrix);
		shader.SetUniform4f("u_Color", sprite.GetColor());
		Renderer::Submit(mesh.GetVertexArray());
	}
}

