#include "epch.h"
#include "Application.h"

#include "Engine/Events/Event.h"
#include "Engine/Log.h"
#include "Engine/Input.h"

#include "Engine/Renderer/Renderer.h"

namespace Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		ENG_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ENG_BIND_EVENT_FN(Application::OnEvent));

		m_VertexArray.reset(VertexArray::Create());

		float verticies[3 * 7] = {
			100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			200.0f, 100.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			150.0f, 200.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(verticies, sizeof(verticies)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color" }
			};

			vertexBuffer->SetLayout(layout);
		}
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indicies[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_Shader.reset(new Shader("res/shader/basic.shader"));

		float squareVerticies[3 * 4] = {
			400.0f, 400.0f, 0.0f,
			600.0f, 400.0f, 0.0f,
			600.0f, 600.0f, 0.0f,
			400.0f, 600.0f, 0.0f,
		};

		m_SquareVA.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVerticies, sizeof(squareVerticies)));
		squareVB->SetLayout( {
			{ ShaderDataType::Float3, "a_Position" },
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		unsigned int squareIndicies[6] = { 
			0, 1, 2,
			2, 3, 0,
		};
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndicies, sizeof(squareIndicies) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_BlueShader.reset(new Shader("res/shader/blue.shader"));

		m_Camera = OrthographicCamera(
			{0.0f, 0.0f, 0.0f}, 
			(float)m_Window->GetWidth(), 
			(float)m_Window->GetHeight()
		);
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(ENG_BIND_EVENT_FN(Application::OnWindowClosed));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}

	void Application::Run()
	{
		while (m_Running) {
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene(m_Camera);

			Renderer::SetShader(m_BlueShader);
			Renderer::Submit(m_SquareVA);

			Renderer::SetShader(m_Shader);
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();


			for (Layer* layer: m_LayerStack)
				layer->OnUpdate();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}