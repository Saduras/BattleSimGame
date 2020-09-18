#pragma once

#include <Engine.h>

#include <entt/entt.hpp>

class TowerBattleLayer : public Engine::Layer
{
public:
	TowerBattleLayer();
	~TowerBattleLayer();

	void OnUpdate(float deltaTime) override;
private:
	entt::registry m_Registry;
	entt::entity m_CameraEntity;

	entt::entity CreateTower(glm::vec3 position);
	entt::entity CreateCamera();
};

