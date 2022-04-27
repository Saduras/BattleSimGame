#pragma once

#include <Engine.h>

enum class Faction
{
	None, Red, Blue
};

struct Tower
{
	Faction Faction = Faction::None;
	unsigned int Units = 0;
	unsigned int MaxUnits = 10;
	float ProductionIntervall = 1.0f;
	float NextProductionTime = 0.0f;
};

class TowerBattleLayer : public Engine::Layer
{
public:
	TowerBattleLayer(Engine::Scene& scene);
	~TowerBattleLayer();

	void OnUpdate(float deltaTime) override;
	void OnEvent(Engine::Event& event) override;

	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event);
	void OnTowerClick(Engine::Entity tower);
	void UpdateTower(Tower& tower, Engine::Components::Renderable2D& renderable, float deltaTime);
	void UpdateTowerSprite(Engine::Components::Renderable2D& renderable, int units);
	void Attack(Engine::Entity source, Engine::Entity target);
private:
	Engine::Scene& m_Scene;
	Engine::Entity m_SourceTower;

	Engine::Entity CreateTower(Engine::Vec3 position, Faction faction);
	Engine::Entity CreateCamera();
};

