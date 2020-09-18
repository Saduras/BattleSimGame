#include <Engine.h>
#include <Engine/EntryPoint.h>

#include <entt/entt.hpp>

#include "TowerBattleLayer.h"

//class ExampleLayer : public Engine::Layer
//{
//private:
//	entt::registry m_Registry;
//	entt::entity m_CamEntity;
//public:
//	ExampleLayer()
//		: Layer("Example")
//	{
//		auto e1 = m_Registry.create();
//		m_Registry.emplace<Engine::Components::Transform>(e1,
//			glm::vec3( 0.0f, 0.0f, 0.0f ), // position
//			glm::vec3( 0.0f, 0.0f, 0.0f ), // rotation
//			glm::vec3( 100.0f, 100.0f, 1.0f )  // scale
//		);
//		m_Registry.emplace<Engine::Components::Mesh>(e1, Engine::PrimitiveMesh::Quad);
//		m_Registry.emplace<Engine::Components::Material>(e1, "res/shader/default.shader");
//
//		m_CamEntity = m_Registry.create();
//		m_Registry.emplace<Engine::Components::Transform>(m_CamEntity,
//			glm::vec3(0.0f, 0.0f, 0.0f), // position
//			glm::vec3(0.0f, 0.0f, 0.0f), // rotation
//			glm::vec3(1.0f, 1.0f, 1.0f)  // scale
//		);
//		m_Registry.emplace<Engine::Components::OrthographicCamera>(m_CamEntity,
//			(float)-1280 / 2, // left
//			(float)1280 / 2, // right
//			(float)-720 / 2, // bottom
//			(float)720 / 2  // top
//		);
//	}
//
//	void OnUpdate(float deltaTime) override
//	{
//		using namespace Engine::Components;
//
//		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
//		Engine::RenderCommand::Clear();
//
//		// For simplicity assume the first camera is the main camera
//		auto proj = m_Registry.get<OrthographicCamera>(m_CamEntity).GetProjectionMatrix();
//		auto view = glm::inverse(m_Registry.get<Transform>(m_CamEntity).GetTransformationMatrix());
//		Engine::Renderer::BeginScene(proj * view);
//
//		// Render all entities with transform, mesh and material
//		auto renderView = m_Registry.view<Transform, Mesh, Material>();
//		renderView.each([](auto& transform, auto& mesh, auto& material) {
//			auto shader = material.GetShader();
//			Engine::Renderer::SetShader(shader);
//			auto modelMatrix = transform.GetTransformationMatrix();
//			shader->SetUniformMat4f("u_Model", modelMatrix);
//			Engine::Renderer::Submit(mesh.GetVertexArray());
//		});
//
//		Engine::Renderer::EndScene();
//	}
//};

class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new TowerBattleLayer());
		//PushOverlay(new Engine::ImGuiLayer());
	}

	~BattleSimGame()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new BattleSimGame();
}