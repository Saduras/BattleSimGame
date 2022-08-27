#pragma once

#include "Engine/Layer.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/ApplicationEvent.h"

namespace Engine
{
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate(float deltaTime);
		void OnEvent(Event& event);
	private:
		void ShowPerformanceOverlay(float deltaTime);
		bool OnMousePressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrollEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizedEvent(WindowResizeEvent& e);

		const static int c_FrameTimeHistoryLength = 30;
		float m_FrameTimeHistory[c_FrameTimeHistoryLength] = { 0 };
		int m_FrameTimeInsertIndex = 0;
	};
}