#pragma once

#include <Engine.h>

#include <entt/entt.hpp>

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
	void OnTowerClick(entt::entity tower);
	void UpdateTower(Tower& tower, float deltaTime);
	void Attack(entt::entity source, entt::entity target);
private:
	Engine::Scene& m_Scene;
	entt::entity m_SourceTower;

	entt::entity CreateTower(glm::vec3 position, Faction faction);
	entt::entity CreateCamera();
};

