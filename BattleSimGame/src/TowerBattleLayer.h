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
	bool ViewUpdateRequested = false;
};

struct Unit
{
	Faction Faction = Faction::None;
	Engine::Entity Target;
	float Speed = 20.0f;
	float Health = 5.0f;
	float Attack = 1.0f;
	float AttackSpeed = 1.0f;
	float NextAttackTime = 0.0f;
};

struct AIStrategist
{
	Faction Faction = Faction::None;
	float TimeUntilAction = 0.0f;
};

struct UnitBar
{
	float MaxWidth;
	float X;
	Engine::Entity TowerEntity;
};

struct QuadCollider
{
	Engine::Vec2 Center;
	float Width;
	float Height;
	std::vector<Engine::Entity> Collisions;

	Engine::Vec2 GetTopRight() { return Engine::Vec2(Center.x + Width / 2, Center.y + Height / 2); }
	Engine::Vec2 GetBottomLeft() { return Engine::Vec2(Center.x - Width / 2, Center.y - Height / 2); }

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

struct EnemyContact
{
	bool HasContact;
	std::vector<Engine::Entity> Enemies;
	Engine::Entity ClosestEnemy;
};

class TowerBattleLayer : public Engine::Layer
{
public:
	TowerBattleLayer(Engine::Scene* scene);

	void OnUpdate(float deltaTime) override;
	void OnEvent(Engine::Event& event) override;

	bool OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event);
	void OnTowerClick(Engine::Entity tower);
	static void Attack(Engine::Entity source, Engine::Entity target);

	static void CheckVictoryCondition();
	static void ChangeTowerUnits(Engine::Entity towerEntity, int unitDelta);

	static Engine::Scene* GetScene() { return m_Scene; };
private:
	static bool m_GameRunning;
	static Engine::Scene* m_Scene;

	Engine::Entity m_SourceTower;
	Engine::Entity m_Selection;

	Engine::Entity CreateTower(Engine::Vec3 position, Faction faction);
	Engine::Entity CreateAI(Faction faction);
	Engine::Entity CreateCamera();
	Engine::Entity CreateSelection();
	void CreateLevel(int width, int height, int tileSize);
};

