#include <Engine.h>

class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{

	}

	~BattleSimGame()
	{

	}
};

Engine::Application* Engine::CreateApplication()
{
	return new BattleSimGame();
}