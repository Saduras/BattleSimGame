#include <Engine.h>

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Engine::Input::IsKeyPressed(ENG_KEY_TAB))
			ENG_TRACE("Tab key is pressed!");
	}

	void OnEvent(Engine::Event& event) override
	{
		//ENG_TRACE("{0}", event);
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