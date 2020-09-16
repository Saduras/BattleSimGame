#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
private:
	Engine::Components::Camera* m_Camera;
public:
	ExampleLayer()
		: Layer("Example")
	{
		Engine::Entity* e1 = CreateEntity();
		e1->AddComponent(new Engine::Components::Transform(
			{ 0.0f, 0.0f, 0.0f }, // position
			{ 0.0f, 0.0f, 0.0f }, // rotation
			{ 100.0f, 100.0f, 1.0f }  // scale
		));
		e1->AddComponent(new Engine::Components::Mesh(Engine::PrimitiveMesh::Quad));
		e1->AddComponent(new Engine::Components::Material("res/shader/default.shader"));

		Engine::Entity* cameraEntity = CreateEntity();
		cameraEntity->AddComponent(new Engine::Components::Transform(
			{ 0.0f, 0.0f, 0.0f }, // position
			{ 0.0f, 0.0f, 0.0f }, // rotation
			{ 1.0f, 1.0f, 1.0f }  // scale
		));
		m_Camera = new Engine::Components::OrthographicCamera(
			(float)-1280/2, // left
			(float) 1280/2, // right
			(float) -720/2, // bottom
			(float)  720/2  // top
		);
		cameraEntity->AddComponent(m_Camera);
	}

	void OnUpdate(float deltaTime) override
	{
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(*m_Camera);

		for (const auto& entity : m_Entities) {
			if (entity->HasComponent<Engine::Components::Mesh*>(typeid(Engine::Components::Mesh*))
				&& entity->HasComponent<Engine::Components::Material*>(typeid(Engine::Components::Material*))
				&& entity->HasComponent<Engine::Components::Transform*>(typeid(Engine::Components::Transform*)))
			{
				auto material = entity->GetComponent<Engine::Components::Material*>(typeid(Engine::Components::Material*));
				auto shader = material->GetShader();
				Engine::Renderer::SetShader(shader);
				auto transform = entity->GetComponent<Engine::Components::Transform*>(typeid(Engine::Components::Transform*));
				auto modelMatrix = transform->GetTransformationMatrix();
				shader->SetUniformMat4f("u_Model", modelMatrix);

				auto mesh = entity->GetComponent<Engine::Components::Mesh*>(typeid(Engine::Components::Mesh*));
				Engine::Renderer::Submit(mesh->GetVertexArray());
			}
		}

		Engine::Renderer::EndScene();
	}
};

class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Engine::ImGuiLayer());
	}

	~BattleSimGame()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new BattleSimGame();
}