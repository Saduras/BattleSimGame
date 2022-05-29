#include <Engine.h>
#include <Engine/EntryPoint.h>

#include <entt/entt.hpp>

#include "TowerBattleLayer.h"
#include "ExamplesLayer.h"


class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{
		//PushLayer(new ExampleLayer(ExampleScene::SpriteRender));
		Engine::Scene* scene = new Engine::Scene();
		scene->AddSystem<Engine::Systems::Render2DSystem>();
		PushLayer(new TowerBattleLayer(scene));
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