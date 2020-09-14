#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
private:
	Engine::Components::Camera* m_Camera;
	std::shared_ptr<Engine::Shader> m_Shader;
	std::shared_ptr<Engine::VertexArray> m_VertexArray;
	std::shared_ptr<Engine::Shader> m_BlueShader;
	std::shared_ptr<Engine::VertexArray> m_SquareVA;
public:
	ExampleLayer()
		: Layer("Example")
	{
		m_VertexArray.reset(Engine::VertexArray::Create());

		float verticies[3 * 7] = {
			100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			200.0f, 100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			150.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create(verticies, sizeof(verticies)));

		{
			Engine::BufferLayout layout = {
				{ Engine::ShaderDataType::Float3, "a_Position" },
				{ Engine::ShaderDataType::Float4, "a_Color" }
			};

			vertexBuffer->SetLayout(layout);
		}
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indicies[3] = { 0, 1, 2 };
		std::shared_ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(new Engine::Shader("res/shader/basic.shader"));

		float squareVerticies[3 * 4] = {
			400.0f, 400.0f, 0.0f,
			600.0f, 400.0f, 0.0f,
			600.0f, 600.0f, 0.0f,
			400.0f, 600.0f, 0.0f,
		};

		m_SquareVA.reset(Engine::VertexArray::Create());
		std::shared_ptr<Engine::VertexBuffer> squareVB;
		squareVB.reset(Engine::VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));
		squareVB->SetLayout({
			{ Engine::ShaderDataType::Float3, "a_Position" },
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndicies[6] = {
			0, 1, 2,
			2, 3, 0,
		};
		std::shared_ptr<Engine::IndexBuffer> squareIB;
		squareIB.reset(Engine::IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_BlueShader.reset(new Engine::Shader("res/shader/blue.shader"));

		Engine::Entity* cameraEntity = CreateEntity();
		cameraEntity->AddComponent(new Engine::Components::Transform(
			{ 0.0f, 0.0f, 0.0f }, // position
			{ 0.0f, 0.0f, 0.0f }, // rotation
			{ 1.0f, 1.0f, 1.0f }  // scale
		));
		m_Camera = new Engine::Components::OrthographicCamera(
			(float)1280, // width
			(float)720   // height
		);
		cameraEntity->AddComponent(m_Camera);
	}

	void OnUpdate() override
	{
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RenderCommand::Clear();

		Engine::Renderer::BeginScene(*m_Camera);

		Engine::Renderer::SetShader(m_BlueShader);
		Engine::Renderer::Submit(m_SquareVA);

		Engine::Renderer::SetShader(m_Shader);
		Engine::Renderer::Submit(m_VertexArray);

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