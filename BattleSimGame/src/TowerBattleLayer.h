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
	bool Selected = false;
};

struct Unit
{
	Faction Faction = Faction::None;
	Engine::Entity Target;
	float Speed = 20.0f;
};

struct QuadCollider
{
	Engine::Vec2 Center;
	float Width;
	float Height;

	bool IsInside(Engine::Vec2 position)
	{
		float left = Center.x - Width / 2;
		float right = Center.x + Width / 2;
		float bottom = Center.y - Height / 2;
		float top = Center.y + Height / 2;

		return left <= position.x && position.x <= right
			&& bottom <= position.y && position.y <= top;
	}
};

struct UpdateView
{
	// TODO how to do marker components without members?
	bool IsSet = true;
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
	void Attack(Engine::Entity source, Engine::Entity target);
	static void ChangeTowerUnits(Engine::Entity towerEntity, int unitDelta);
private:
	Engine::Scene& m_Scene;
	Engine::Entity m_SourceTower;

	Engine::Entity CreateTower(Engine::Vec3 position, Faction faction);
	Engine::Entity CreateCamera();
};

