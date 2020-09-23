#pragma once

#include <Engine.h>

#include <entt/entt.hpp>

enum class Faction
{
	None, Red, Blue
};

class TowerBattleLayer : public Engine::Layer
{
public:
	TowerBattleLayer();
	~TowerBattleLayer();

	void OnUpdate(float deltaTime) override;
	void OnEvent(Engine::Event& event) override;

	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event);
	void OnTowerClick(entt::entity tower);
	void Attack(entt::entity source, entt::entity target);
private:
	entt::registry m_Registry;
	entt::entity m_CameraEntity;
	entt::entity m_SourceTower;

	entt::entity CreateTower(glm::vec3 position, Faction faction);
	entt::entity CreateCamera();
};

