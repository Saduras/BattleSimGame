#include "epch.h"
#include "Application.h"

#include "Engine/Events/Event.h"
#include "Engine/Log.h"
#include "Engine/Input.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Time.h"

namespace Engine
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_LastFrameTime(0.0f), m_Running(true)
	{
		ENG_CORE_ASSERT(!s_Instance, "Application already exists.");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(ENG_BIND_EVENT_FN(Application::OnEvent));
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
		m_LastFrameTime = Time::GetTime();
		while (m_Running) {
			float newTime = Time::GetTime();
			float deltaTime = newTime - m_LastFrameTime;
			m_LastFrameTime = newTime;

			for (Layer* layer: m_LayerStack)
				layer->OnUpdate(deltaTime);

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}