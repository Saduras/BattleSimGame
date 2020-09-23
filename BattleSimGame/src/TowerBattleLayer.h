#pragma once

#include <Engine.h>

#include <entt/entt.hpp>

class TowerBattleLayer : public Engine::Layer
{
public:
	TowerBattleLayer();
	~TowerBattleLayer();

	void OnUpdate(float deltaTime) override;
	void OnEvent(Engine::Event& event) override;

	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event);
	void OnTowerClick(entt::entity tower);
private:
	entt::registry m_Registry;
	entt::entity m_CameraEntity;
	entt::entity m_SourceTower;

	entt::entity CreateTower(glm::vec3 position);
	entt::entity CreateCamera();
};

