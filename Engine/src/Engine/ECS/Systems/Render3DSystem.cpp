#include "epch.h"
#include "Render3DSystem.h"

#include "Engine/ECS/Components/Camera.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine::Systems {
	void Render3DSystem::Execute(float deltaTime)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		auto cameras = m_Scene->GetView<Components::Transform, Components::OrthographicCamera>();
		cameras.each([this](Components::Transform& transform, Components::OrthographicCamera& camera) {
			auto viewMatrix = transform.GetTransformationMatrix();
			auto projMatrix = camera.GetProjectionMatrix();

			Engine::Renderer::BeginScene(projMatrix * viewMatrix);

			// Render all entities with transform, mesh and material
			auto renderableView = m_Scene->GetView<Components::Transform, Components::Renderable3D>();
			renderableView.each(Render3DSystem::RenderRenderable);

			Engine::Renderer::EndScene();
			});
	}

	void Render3DSystem::RenderRenderable(const Components::Transform& transform, const Components::Renderable3D& renderable)
	{
		Material& material = AssetRegistry::Get<Material>(renderable.MaterialID);
		Mesh& mesh = Engine::AssetRegistry::Get<Mesh>(renderable.MeshID);
		Shader& shader = AssetRegistry::Get<Shader>(material.GetShaderID());

		Renderer::SetShader(&shader);
		auto modelMatrix = transform.GetTransformationMatrix();
		shader.SetProperty("u_Model", modelMatrix);
		shader.SetProperty("u_Color", material.GetColor());
		shader.ApplyProperties();
		Renderer::Submit(mesh.GetVertexArray());
	}
}

