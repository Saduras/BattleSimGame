#include "epch.h"
#include "ImGuiLayer.h"
#include "Engine/Application.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "Engine/Events/Event.h"
#include "Engine/KeyCodes.h"

// TEMPORARY
#include <glad/glad.h>

namespace Engine
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; 

		io.KeyMap[ImGuiKey_Tab] = ENG_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = ENG_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = ENG_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = ENG_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = ENG_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = ENG_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = ENG_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = ENG_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = ENG_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = ENG_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = ENG_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = ENG_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = ENG_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = ENG_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = ENG_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = ENG_KEY_A;
		io.KeyMap[ImGuiKey_C] = ENG_KEY_C;
		io.KeyMap[ImGuiKey_V] = ENG_KEY_V;
		io.KeyMap[ImGuiKey_X] = ENG_KEY_X;
		io.KeyMap[ImGuiKey_Y] = ENG_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = ENG_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
	}

	void ImGuiLayer::OnUpdate(float deltaTime)
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		io.DeltaTime = deltaTime;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ShowPerformanceOverlay(deltaTime);
		//static bool show = true;
		//ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnMousePressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnMouseReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnMouseScrollEvent));
		dispatcher.Dispatch<KeyPressedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(ENG_BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
	}

	void ImGuiLayer::ShowPerformanceOverlay(float deltaTime)
	{
		// Record deltaTime history
		m_FrameTimeHistory[m_FrameTimeInsertIndex] = deltaTime;
		m_FrameTimeInsertIndex = (m_FrameTimeInsertIndex + 1) % c_FrameTimeHistoryLength;

		// Calculate average deltaTime over available history
		float avgDeltaTime = 0.0f;
		for (size_t i = 0; i < c_FrameTimeHistoryLength; i++)
			avgDeltaTime += m_FrameTimeHistory[i];
		avgDeltaTime /= (float)c_FrameTimeHistoryLength;

		// Display performance metrics in overlay
		const float DISTANCE = 10.0f;
		ImGuiIO& io = ImGui::GetIO();
		ImVec2 window_pos = ImVec2(DISTANCE, DISTANCE);
		ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
		bool open = true;
		if (ImGui::Begin("Performance", &open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Delta Time: %.2fms", avgDeltaTime * 1000.0f);
			ImGui::Text("FPS: %.1f", 1.0f / avgDeltaTime);
		}
		ImGui::End();
	}

	bool ImGuiLayer::OnMousePressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;
	}
	
	bool ImGuiLayer::OnMouseReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		
		return false;
	}
	
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}

	bool ImGuiLayer::OnMouseScrollEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}
	
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[ENG_KEY_LEFT_CONTROL] || io.KeysDown[ENG_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[ENG_KEY_LEFT_SHIFT] || io.KeysDown[ENG_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[ENG_KEY_LEFT_ALT] || io.KeysDown[ENG_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[ENG_KEY_LEFT_SUPER] || io.KeysDown[ENG_KEY_RIGHT_SUPER];

		return false;
	}
	
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x10000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}
	
	bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)e.GetWidth(), (float)e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}