#include "epch.h"
#include "Render3DSystem.h"

#include "Engine/ECS/Components/Camera.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/DebugTool.h"
#include "Engine/Math.h"

namespace Engine::Systems {
	void Render3DSystem::Execute(float deltaTime)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		auto cameras = m_Scene->GetView<Transform, Components::OrthographicCamera>();
		cameras.each([this](Transform& transform, Components::OrthographicCamera& camera) {
			auto viewMatrix = TransformToMatrix(transform);
			auto projMatrix = camera.GetProjectionMatrix();

			Engine::Renderer::BeginScene(projMatrix * viewMatrix);

			// Render all entities with transform, mesh and material
			auto renderableView = m_Scene->GetView<Transform, Components::Renderable3D>();
			renderableView.each(Render3DSystem::RenderRenderable);

			Debug::Render();

			Engine::Renderer::EndScene();
			});
	}

	void Render3DSystem::RenderRenderable(const Transform& transform, const Components::Renderable3D& renderable)
	{
		Material& material = AssetRegistry::Get<Material>(renderable.MateriaUUID);
		Mesh& mesh = Engine::AssetRegistry::Get<Mesh>(renderable.MeshUUID);
		Shader& shader = AssetRegistry::Get<Shader>(material.GetShaderUUID());

		Renderer::SetShader(&shader);
		auto modelMatrix = TransformToMatrix(transform);
		shader.SetProperty("u_Model", modelMatrix);
		shader.SetProperty("u_Color", material.GetColor());
		shader.ApplyProperties();
		Renderer::Submit(mesh.GetVertexArray());
	}
}

