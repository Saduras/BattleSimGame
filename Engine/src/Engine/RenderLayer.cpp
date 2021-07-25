#include "epch.h"
#include "RenderLayer.h"

#include "Engine/Components/Camera.h"
#include "Engine/Assets/AssetRegistry.h"
#include "Engine/EntityRegistry.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Renderer.h"

namespace Engine {
	RenderLayer::RenderLayer()
	{

	}

	RenderLayer::~RenderLayer()
	{

	}

	void RenderLayer::OnUpdate(float deltaTime)
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();

		auto cameras = EntityRegistry::View<Components::Transform, Components::OrthographicCamera>();
		cameras.each([](Components::Transform& transform, Components::OrthographicCamera& camera) {
			auto viewMatrix = transform.GetTransformationMatrix();
			auto projMatrix = camera.GetProjectionMatrix();

			Engine::Renderer::BeginScene(projMatrix * viewMatrix);

			// Render all entities with transform, mesh and material
			auto renderableView = EntityRegistry::View<Components::Transform, Components::Renderable>();
			renderableView.each(RenderLayer::RenderRenderable);

			Engine::Renderer::EndScene();
		});
	}

	void RenderLayer::RenderRenderable(const Components::Transform& transform, const Components::Renderable& renderable)
	{
		auto& material = AssetRegistry::Get<Material>(renderable.MaterialID);
		auto& mesh = Engine::AssetRegistry::Get<Mesh>(renderable.MeshID);

		auto shader = material.GetShader();
		Renderer::SetShader(shader);
		auto modelMatrix = transform.GetTransformationMatrix();
		shader->SetUniformMat4f("u_Model", modelMatrix);
		shader->SetUniform4f("u_Color", material.GetColor());
		Renderer::Submit(mesh.GetVertexArray());
	}
}