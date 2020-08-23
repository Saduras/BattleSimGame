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
		ENG_INFO("ExampleLayer::Update");
	}

	void OnEvent(Engine::Event& event) override
	{
		ENG_TRACE("{0}", event);
	}
};

class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{
		PushLayer(new ExampleLayer());
	}

	~BattleSimGame()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new BattleSimGame();
}