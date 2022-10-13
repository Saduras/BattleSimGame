#include "TowerBattleLayer.h"

#include <algorithm> 
#include <limits>

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

unsigned int UnitKey(Faction faction, SoldierType type)
{
	return (unsigned int)faction + (unsigned int)Faction::COUNT * (unsigned int)type;
}

Engine::UUID TowerBattleLayer::GetFactionSpriteID(Faction faction)
{
	if(m_TowerSpriteIdMap.find(faction) != m_TowerSpriteIdMap.end())
		return m_TowerSpriteIdMap[faction];

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return Engine::UUID();
}

Engine::UUID TowerBattleLayer::GetUnitSpriteID(Faction faction, SoldierType soldierType)
{
	if (m_UnitSpriteIdMap.find(UnitKey(faction, soldierType)) != m_UnitSpriteIdMap.end())
		return m_UnitSpriteIdMap[UnitKey(faction, soldierType)];

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return Engine::UUID();
}

Engine::UUID TowerBattleLayer::GetUnitBarSprite(Faction faction)
{
	switch (faction)
	{
	case Faction::Red:  return m_TowerBarFillSpriteRedUUID;
	case Faction::Blue: return m_TowerBarFillSpriteBlueUUID;
	case Faction::None: return m_TowerBarFillSpriteNoneUUID;
	}

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return Engine::UUID();
}

static void CollisionDetectionSystem(float deltaTime, Engine::Scene* scene, Engine::Entity entity, QuadCollider& collider)
{
	collider.Collisions.clear();

	auto view = scene->GetView<QuadCollider>();
	view.each([scene, entity, &collider](entt::entity otherId, QuadCollider& otherCollider) {
		Engine::Entity other(otherId, scene);

		if (entt::entity(entity) == otherId || !other.IsValid())
			return;

		Engine::Vec2 bottom_left = collider.GetBottomLeft();
		Engine::Vec2 top_right = collider.GetTopRight();
		Engine::Vec2 other_bottom_left = otherCollider.GetBottomLeft();
		Engine::Vec2 other_top_right = otherCollider.GetTopRight();

		if (bottom_left.x < other_top_right.x
			&& top_right.x > other_bottom_left.x
			&& bottom_left.y < other_top_right.y
			&& top_right.y > other_bottom_left.y)
			collider.Collisions.push_back(Engine::Entity(otherId, scene));
	});
}

static void EnemyContactSystem(float deltaTime, Engine::Entity entity, EnemyContact& contact, Unit& unit, QuadCollider& collider)
{
	contact.HasContact = false;
	contact.Enemies.clear();
	contact.ClosestEnemy = Engine::Entity();

	float min_distance = std::numeric_limits<float>::max();

	for (Engine::Entity other : collider.Collisions)
		if (other.HasComponent<Unit>() && other.GetComponent<Unit>().Faction != unit.Faction)
		{
			contact.HasContact = true;
			contact.Enemies.push_back(other);

			QuadCollider& otherCollider = other.GetComponent<QuadCollider>();
			float distance = Engine::Magnitude(otherCollider.Center - collider.Center);
			if (distance < min_distance)
			{
				min_distance = distance;
				contact.ClosestEnemy = other;
			}
		}
}

static bool HasCollisionWithEnemyUnit(Engine::Entity entity)
{
	return entity.HasComponent<EnemyContact>() && entity.GetComponent<EnemyContact>().HasContact;
}

static void UnitAnimationSystem(float deltaTime, Engine::Entity entity, Unit& unit, Engine::Components::Animator& animator)
{
	Engine::UUID prevAnimation = animator.CurrentAnimationUUID;

	if (HasCollisionWithEnemyUnit(entity))
	{
		// Disable automatic animation playbackk
		animator.CurrentAnimationUUID = Engine::UUID::Invalid();
		if (prevAnimation != animator.CurrentAnimationUUID)
			animator.AnimationTime = 0.0f;

		
		// Play a procedural animation based on the direction to the enemy
		using namespace Engine;
		// Compute direction towards enemy
		EnemyContact& contact = entity.GetComponent<EnemyContact>();
		Transform ownTransform = entity.GetComponent<Transform>();
		Transform enemyTransform = contact.ClosestEnemy.GetComponent<Transform>();
		Vec3 direction = Normalize(enemyTransform.Position - ownTransform.Position);

		Vec3 startTranslation = direction * -0.1f;;
		Vec3 endTranslation = direction * 0.3f;
		float hitTime = 0.05f;
		float recoverTime = 0.3f;
		float totalDuration = hitTime + recoverTime;

		// Update animation time
		animator.AnimationTime = Mod(animator.AnimationTime + deltaTime, totalDuration);

		// Compute translation
		Vec3 translation;
		if (animator.AnimationTime <= hitTime)
		{
			float t = animator.AnimationTime / hitTime;
			translation = Lerp(startTranslation, endTranslation, t);
		}
		else
		{
			float t = (animator.AnimationTime - hitTime) / recoverTime;
			translation = Lerp(endTranslation, startTranslation, t);
		}

		// Mirror animation if sprite is flipped
		float spriteDirection = Sign(ownTransform.Scale.x);
		translation.x = spriteDirection * translation.x;

		animator.Offset = Transform(translation, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
	}
	else
	{
		animator.CurrentAnimationUUID = TowerBattleLayer::GetWalkAnimUUID();

		if (prevAnimation != animator.CurrentAnimationUUID)
		{
			animator.Offset = Engine::Transform();
			animator.AnimationTime = 0.0f;
		}
	}
}

static void UnitMoveSystem(float deltaTime, Engine::Entity entity, Unit& unit, QuadCollider& collider, Engine::Transform& transform)
{
	Engine::Vec3 targetPosition = unit.Target.GetComponent<Engine::Transform>().Position;
	Engine::Vec3 currentPosition = transform.Position;
	Engine::Vec3 distance = targetPosition - currentPosition;

	if (Engine::Magnitude(distance) > 0.1f && !HasCollisionWithEnemyUnit(entity))
	{
		transform.Position += Engine::Normalize(distance) * unit.Speed * deltaTime;
		collider.Center = transform.Position;
	}
}

static void UnitAttackUnitSystem(float deltaTime, Engine::Entity, Unit& unit, EnemyContact& contact)
{
	unit.NextAttackTime = std::max(0.0f, unit.NextAttackTime - deltaTime);

	if (!contact.HasContact)
		return;

	if (unit.NextAttackTime == 0.0f)
	{
		Unit& enemy = contact.ClosestEnemy.GetComponent<Unit>();
		enemy.Health = std::max(0.0f, enemy.Health - unit.Attack);

		// One attack per second
		unit.NextAttackTime = 1.0f;
	}
}

static void UnitDeathSystem(float deltaTime, Engine::Entity entity, Unit& unit)
{
	if (unit.Health <= 0.0f)
		entity.Destroy();
}

static void UnitAttackTowerSystem(float deltaTime, Engine::Entity entity, Unit& unit, Engine::Transform& transform)
{
	Engine::Vec3 targetPosition = unit.Target.GetComponent<Engine::Transform>().Position;
	Engine::Vec3 distance = targetPosition - transform.Position;

	// Check if we are close enough to attack
	if (Engine::Magnitude(distance) > 1.0f)
		return;


	Tower& targetTower = unit.Target.GetComponent<Tower>();
	if (targetTower.Faction == unit.Faction)
	{
		// Reinforcing own tower
		TowerBattleLayer::ChangeTowerUnits(unit.Target, +1);
	}
	else if (targetTower.Units == 0)
	{
		// Capturing tower
		targetTower.Faction = unit.Faction;
		TowerBattleLayer::ChangeTowerUnits(unit.Target, +1);
		TowerBattleLayer::CheckVictoryCondition();
	}
	else
	{
		// Damage tower
		TowerBattleLayer::ChangeTowerUnits(unit.Target, -1);
	}

	entity.Destroy();

	ENG_TRACE("Unit reached target (Unit Faction: {1}, Target Faction: {2})", unit.Faction, targetTower.Faction);
}

static void TowerViewSystem(float deltaTime, Engine::Entity entity, Tower& tower, Engine::Components::Renderable2D& renderable)
{
	if (!tower.ViewUpdateRequested)
		return;

	renderable.Data[0].SpriteUUID = TowerBattleLayer::GetFactionSpriteID(tower.Faction);
	tower.ViewUpdateRequested = false;
}

static void TowerProductionSystem(float deltaTime, Engine::Entity entity, Tower& tower)
{
	if (tower.Units >= tower.MaxUnits || tower.Faction == Faction::None)
		return;

	tower.NextProductionTime -= deltaTime;
	if (tower.NextProductionTime <= 0) {
		tower.NextProductionTime += tower.ProductionIntervall;
		TowerBattleLayer::ChangeTowerUnits(entity, +1);

		ENG_TRACE("Faction {0} got one unit. Currently {1}/{2}", tower.Faction, tower.Units, tower.MaxUnits);
	}
}

static void UnitBarUISystem(float delta_time, Engine::Entity entity, UnitBar& unitBar, Engine::Transform& transform, Engine::Components::Renderable2D& renderable)
{
	Tower& tower = unitBar.TowerEntity.GetComponent<Tower>();
	float fraction = tower.Units / (float)tower.MaxUnits;

	transform.Scale.x = unitBar.MaxWidth * fraction;
	transform.Position.x = unitBar.X + unitBar.MaxWidth / 2.0f * fraction;

	renderable.Data[0].SpriteUUID = TowerBattleLayer::GetUnitBarSprite(tower.Faction);
}

static void AIStrategistSystem(float deltaTime, Engine::Entity entity, AIStrategist& ai)
{
	ai.TimeUntilAction -= deltaTime;

	if (ai.TimeUntilAction > 0.0f)
		return;

	ai.TimeUntilAction = 7.0f + 2.0f * Engine::GetRandomFloat();

	std::vector<Engine::Entity> own_tower_entities;
	std::vector<Engine::Entity> other_tower_entities;

	// Gather towers for each faction
	Engine::Scene* scene = TowerBattleLayer::GetScene();
	scene->GetView<Tower>().each([&own_tower_entities, &other_tower_entities, &ai, scene](entt::entity entity, Tower& tower) {
		if (tower.Faction == ai.Faction)
			own_tower_entities.push_back(Engine::Entity(entity, scene));
		else
			other_tower_entities.push_back(Engine::Entity(entity, scene));
	});

	// Reduce action time when controling more towers
	ai.TimeUntilAction /= own_tower_entities.size();

	int src_index = Engine::GetRandomIndex(own_tower_entities.size());
	int target_index = Engine::GetRandomIndex(other_tower_entities.size());

	TowerBattleLayer::Attack(own_tower_entities[src_index], other_tower_entities[target_index]);
}

static void DrawCollidersSystem(float deltaTime, Engine::Entity entity, QuadCollider& collider)
{
	Engine::Vec2 offset(collider.Width / 2.0f, collider.Height / 2.0f);		
	Engine::Vec3 color(1.0f, 0.8f, 0.3f);

	if (entity.HasComponent<Unit>() && HasCollisionWithEnemyUnit(entity))
		color = Engine::Vec3(1.0f, 0.0f, 0.0f);

	Engine::Debug::DrawRect(collider.Center - offset, collider.Center + offset, color, 1.0f);
}

bool TowerBattleLayer::m_GameRunning = false;
Engine::Scene* TowerBattleLayer::m_Scene = nullptr;

std::unordered_map<Faction, Engine::UUID> TowerBattleLayer::m_TowerSpriteIdMap;
std::unordered_map<unsigned int, Engine::UUID> TowerBattleLayer::m_UnitSpriteIdMap;
std::unordered_map<SoldierType, Unit> m_SoldierConfiguration =
{
	{ SoldierType::Light, {
			Faction::None,
			Engine::Entity(),
			40.0f, // Speed
			5.0f, // Health
			1.0f, // Attack
			1.0f, // Attack Speed
		}
	},
	{ SoldierType::Heavy, {
			Faction::None,
			Engine::Entity(),
			20.0f, // Speed
			20.0f, // Health
			3.0f, // Attack
			0.5f, // Attack Speed
		}
	},
};

Engine::UUID TowerBattleLayer::m_TowerBarFillSpriteRedUUID;
Engine::UUID TowerBattleLayer::m_TowerBarFillSpriteBlueUUID;
Engine::UUID TowerBattleLayer::m_TowerBarFillSpriteNoneUUID;
Engine::UUID TowerBattleLayer::m_TowerBarFrameSpriteUUID;
Engine::UUID TowerBattleLayer::m_TowerBarBackgroundSpriteUUID;
Engine::UUID TowerBattleLayer::m_SelectionSpriteUUID;

Engine::UUID TowerBattleLayer::m_GrassSprite01UUID;
Engine::UUID TowerBattleLayer::m_GrassSprite02UUID;
Engine::UUID TowerBattleLayer::m_LeafForestLightSprite01UUID;
Engine::UUID TowerBattleLayer::m_LeafForestLightSprite02UUID;
Engine::UUID TowerBattleLayer::m_LeafForestDenseSprite01UUID;
Engine::UUID TowerBattleLayer::m_LeafForestDenseSprite02UUID;
Engine::UUID TowerBattleLayer::m_NeedleForestLightSprite01UUID;
Engine::UUID TowerBattleLayer::m_NeedleForestLightSprite02UUID;
Engine::UUID TowerBattleLayer::m_NeedleForestDenseSprite01UUID;
Engine::UUID TowerBattleLayer::m_NeedleForestDenseSprite02UUID;

Engine::UUID TowerBattleLayer::m_AnimWalkUUID;
Engine::UUID TowerBattleLayer::m_AnimSpawnUUID;

TowerBattleLayer::TowerBattleLayer(Engine::Scene* scene)
	: Layer("TowerBattle")
{
	Engine::SetSeed(42);
	m_Scene = scene;

	using namespace Engine;

	// Prepare assets
	TextureAtlas* atlas = new TextureAtlas("res/sprite/medieval_sprite_pack.png");
	Engine::UUID atlasUUID = AssetRegistry::Add(atlas);
	std::vector<Engine::UUID> subtexMeshUUIDs = CreateMeshesForAtlas(*atlas);


	Shader* shader = new Shader("res/shader/pixel_sprite.shader");
	shader->SetProperty("u_TexelPerPixel", 50.0f);
	Engine::UUID shaderUUID = AssetRegistry::Add(shader);

	m_TowerSpriteIdMap[Faction::Blue] = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_blue")]));
	m_TowerSpriteIdMap[Faction::Red]  = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_red")]));
	m_TowerSpriteIdMap[Faction::None] = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_neutral")]));

	m_UnitSpriteIdMap[UnitKey(Faction::Blue, SoldierType::Light)] = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("spearman_blue")]));
	m_UnitSpriteIdMap[UnitKey(Faction::Blue, SoldierType::Heavy)] = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("spearman_armored_blue")]));
	m_UnitSpriteIdMap[UnitKey(Faction::Red, SoldierType::Light)]  = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("spearman_red")]));
	m_UnitSpriteIdMap[UnitKey(Faction::Red, SoldierType::Heavy)] = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("spearman_armored_red")]));

	m_SelectionSpriteUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_selection")]));
	m_TowerBarFrameSpriteUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_bar_frame")]));
	m_TowerBarFillSpriteNoneUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_bar_fill")], Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	m_TowerBarFillSpriteRedUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_bar_fill")], Vec4(233.0f / 255.0f, 125.0f / 255.0f, 85.0f / 255.0f, 1.0f)));
	m_TowerBarFillSpriteBlueUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_bar_fill")], Vec4(85.0f / 255.0f, 173.0f / 255.0f, 233.0f / 255.0f, 1.0f)));
	m_TowerBarBackgroundSpriteUUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("tower_bar_background")]));

	m_GrassSprite01UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("grass_01")]));
	m_GrassSprite02UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("grass_02")]));
	m_LeafForestLightSprite01UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("leaf_forest_light_01")]));
	m_LeafForestLightSprite02UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("leaf_forest_light_02")]));
	m_LeafForestDenseSprite01UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("leaf_forest_dense_01")]));
	m_LeafForestDenseSprite02UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("leaf_forest_dense_02")]));
	m_NeedleForestLightSprite01UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("needle_forest_light_01")]));
	m_NeedleForestLightSprite02UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("needle_forest_light_02")]));
	m_NeedleForestDenseSprite01UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("needle_forest_dense_01")]));
	m_NeedleForestDenseSprite02UUID = AssetRegistry::Add(new Sprite(shaderUUID, atlasUUID, subtexMeshUUIDs[atlas->FindSubTexIndex("needle_forest_dense_02")]));

	m_AnimWalkUUID = AssetRegistry::Add(new Animation(Animation::PlayMode::Loop,
		{ 
			{
				0.2f, // duration
				{ { 0.1f, 0.0f, 0.0f}, { 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f } }, // start transform
				{ { -0.1f, 0.0f, 0.0f}, { 0.0f, 0.0f,  10.0f }, { 1.0f, 1.0f, 1.0f } }, // end transform
			},
			{
				0.2f, // duration
				{ { -0.1f, 0.0f, 0.0f}, { 0.0f, 0.0f,  10.0f }, { 1.0f, 1.0f, 1.0f } }, // start transform
				{ { 0.1f, 0.0f, 0.0f}, { 0.0f, 0.0f, -10.0f }, { 1.0f, 1.0f, 1.0f } }, // end transform
			},
		}
	));
	m_AnimSpawnUUID = AssetRegistry::Add(new Animation(Animation::PlayMode::Once,
		{
			{
				0.15f, // duration
				{ { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } }, // start transform
				{ { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 1.1f, 1.2f, 1.0f } }, // end transform
			},
			{
				0.15f, // duration
				{ { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 1.1f, 1.2f, 1.0f } }, // start transform
				{ { 0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } }, // end transform
			},
		}
	));

	Debug::SetShader("res/shader/debug.shader");

	CreateCamera();
	CreateSelection();

	CreateLevel(1280, 720, 64);

	CreateAI(Faction::Blue);
	CreateAI(Faction::Red);

	m_GameRunning = true;
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	if (m_GameRunning)
	{
		// Update collision detection
		m_Scene->ExecuteSystem<QuadCollider>(deltaTime, CollisionDetectionSystem);
		// Update units
		m_Scene->ExecuteSystem<EnemyContact, Unit, QuadCollider>(deltaTime, EnemyContactSystem);
		m_Scene->ExecuteSystem<Unit, EnemyContact>(deltaTime, UnitAttackUnitSystem);
		m_Scene->ExecuteSystem<Unit, QuadCollider, Engine::Transform>(deltaTime, UnitMoveSystem);
		m_Scene->ExecuteSystem<Unit, Engine::Transform>(deltaTime, UnitAttackTowerSystem);
		// Update towers
		m_Scene->ExecuteSystem<Tower>(deltaTime, TowerProductionSystem);
		m_Scene->ExecuteSystem<Tower, Engine::Components::Renderable2D>(deltaTime, TowerViewSystem);
		// Update AI
		m_Scene->ExecuteSystem<AIStrategist>(deltaTime, AIStrategistSystem);
		// Update UI
		m_Scene->ExecuteSystem<UnitBar, Engine::Transform>(deltaTime, UnitBarUISystem);
		// Update Animations
		m_Scene->ExecuteSystem<Unit, Engine::Components::Animator>(deltaTime, UnitAnimationSystem);
		m_Scene->ExecuteSystem<Engine::Components::Animator>(deltaTime, Engine::Systems::TransformAnimationSystem);
		// Clean up
		m_Scene->ExecuteSystem<Unit>(deltaTime, UnitDeathSystem);
	}

	// Debug
	//m_Scene->ExecuteSystem<QuadCollider>(deltaTime, DrawCollidersSystem);

	m_Scene->Update(deltaTime);
}

void TowerBattleLayer::OnEvent(Engine::Event& event)
{
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(ENG_BIND_EVENT_FN(TowerBattleLayer::OnMouseButtonPressed));
}

bool TowerBattleLayer::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event)
{
	using namespace Engine::Components;

	if (event.GetMouseButton() == ENG_MOUSE_BUTTON_LEFT) {
		float x = Engine::Input::GetMouseX();
		float y = Engine::Input::GetMouseY();
		
		auto cameras = m_Scene->GetView<OrthographicCamera>();
		cameras.each([x, y, this](OrthographicCamera& camera) {
			// Translate screen position to world position
			float screenWidth = (float)Engine::Application::Get().GetWindow().GetWidth();
			float screenHeight = (float)Engine::Application::Get().GetWindow().GetHeight();
			Engine::Vec4 worldPos = Camera::ScreenToWorld(camera, { x, y }, screenWidth, screenHeight);

			// Check collision
			m_Scene->GetView<QuadCollider, Tower, Renderable2D>().each([&worldPos, this](const entt::entity entityID, auto& collider, auto& tower, auto& renderable) {
				ENG_TRACE("Hit {0}", collider.IsInside({ worldPos.x, worldPos.y }));
				if (collider.IsInside({ worldPos.x, worldPos.y })) {
					this->OnTowerClick({ entityID, m_Scene });
				}
			});
		});
	}

	return true;
}

void TowerBattleLayer::OnTowerClick(Engine::Entity towerEntity)
{
	using namespace Engine::Components;

	Renderable2D& renderable = towerEntity.GetComponent<Renderable2D>();
	Tower& tower = towerEntity.GetComponent<Tower>();

	if (m_SourceTower.IsNull()) {
		ENG_TRACE("Selected {0}", towerEntity);
		m_SourceTower = towerEntity;

		if (tower.Faction == Faction::Blue) 
		{
			tower.Selected = true;
			m_Selection.GetComponent<Engine::Transform>().Position = towerEntity.GetComponent<Engine::Transform>().Position;
		}
	} else {
		if (m_SourceTower == towerEntity) {
			ENG_TRACE("Deselected {0}", towerEntity);
			m_SourceTower = Engine::Entity();

			if (tower.Faction == Faction::Blue)
				m_Selection.GetComponent<Engine::Transform>().Position = Engine::Vec3(-1000.0f, 0.0f, 0.0f);
		}
		else {
			Attack(m_SourceTower, towerEntity);

			Tower& src_tower_comp = m_SourceTower.GetComponent<Tower>();
			src_tower_comp.Selected = false;
			m_Selection.GetComponent<Engine::Transform>().Position = Engine::Vec3(-1000.0f, 0.0f, 0.0f);
			
			m_SourceTower = Engine::Entity();
		}
	}
}

static void SpawnUnit(Engine::Scene& scene, Engine::Entity sourceTower, Engine::Entity targetTower, size_t index)
{
	using namespace Engine::Components;

	Tower& sourceTowerComponent = sourceTower.GetComponent<Tower>();
	Faction faction = sourceTowerComponent.Faction;
	SoldierType type = sourceTowerComponent.SoldierType;
	
	// Choose position at random point on circle around source tower
	Engine::Vec3 towerPosition = sourceTower.GetComponent<Engine::Transform>().Position;
	Engine::Vec3 targetPosition = targetTower.GetComponent<Engine::Transform>().Position;
	Engine::Vec3 direction = Engine::Normalize(targetPosition - towerPosition);

	int sign = index % 2 == 0 ? 1 : -1;
	float angle = Engine::DegToRad(30.0f * index / 2 * sign);
	float radius = 30.0f;
	Engine::Vec3 position = towerPosition + Engine::RotateZ(direction, angle) * radius;

	float faceDirection = direction.x != 0.0f ? Engine::Sign(direction.x) : 1.0f;

	Engine::Entity unit = scene.CreateEntity();
	unit.AddComponent<Engine::Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(faceDirection * 18.0f, 25.0f, 1.0f)  // scale
		);
	unit.AddComponent<Unit>(
		faction, 
		targetTower,
		m_SoldierConfiguration[type].Speed,
		m_SoldierConfiguration[type].Health,
		m_SoldierConfiguration[type].Attack,
		m_SoldierConfiguration[type].AttackSpeed
	);
	unit.AddComponent<EnemyContact>();
	unit.AddComponent<Renderable2D>(TowerBattleLayer::GetUnitSpriteID(faction, type));
	unit.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		18.0f, // width
		25.0f // height
	);
	unit.AddComponent<Animator>(TowerBattleLayer::GetWalkAnimUUID());
}

void TowerBattleLayer::Attack(Engine::Entity source, Engine::Entity target)
{
	ENG_TRACE("Attack from {0} to {1}", source, target);
	ENG_TRACE("Deselected {0}", source);
	auto& srcTower = source.GetComponent<Tower>();

	// Take units from source tower
	int units = std::min((int)srcTower.Units, 5);
	ChangeTowerUnits(source, -units);
	ENG_TRACE("{0} units move from source (Faction: {1}, Units: {2})", units, srcTower.Faction, srcTower.Units);

	for (size_t i=0; i<units; i++)
		SpawnUnit(*m_Scene, source, target, i);

	// Play animation by resetting animation time
	Engine::Components::Animator& animator = source.GetComponent<Engine::Components::Animator>();
	animator.AnimationTime = 0.0f;
}

void TowerBattleLayer::CheckVictoryCondition()
{
	int redTowerCount = 0;
	int blueTowerCount = 0;
	auto towerView = m_Scene->GetView<Tower>();
	towerView.each([&redTowerCount, &blueTowerCount](const entt::entity& entity, Tower& tower)
		{
			if (tower.Faction == Faction::Red)
				redTowerCount++;
			else if (tower.Faction == Faction::Blue)
				blueTowerCount++;
		}
	);

	if (blueTowerCount == 0) {
		ENG_TRACE("Defeated!");
		m_GameRunning = false;
	}
	else if (redTowerCount == 0)
	{
		ENG_TRACE("Victory!");
		m_GameRunning = false;
	}
}

void TowerBattleLayer::ChangeTowerUnits(Engine::Entity towerEntity, int unitDelta)
{
	Tower& tower = towerEntity.GetComponent<Tower>();
	tower.Units += unitDelta;
	tower.ViewUpdateRequested = true;
}

Engine::Entity TowerBattleLayer::CreateTower(Engine::Vec3 position, Tower towerConfig)
{
	using namespace Engine::Components;
	
	auto tower = m_Scene->CreateEntity();
	tower.AddComponent<Engine::Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(44.0f, 84.0f, 1.0f)  // scale
	);
	tower.AddComponent<Renderable2D>(GetFactionSpriteID(towerConfig.Faction));
	tower.AddComponent<Tower>(towerConfig.Faction, towerConfig.SoldierType, (unsigned int)0, (unsigned int)36, 1.0f, 0.0f);
	tower.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);
	Engine::Animation& animation = Engine::AssetRegistry::Get<Engine::Animation>(m_AnimSpawnUUID);
	tower.AddComponent<Animator>(animation.GetUUID());

	auto bar_frame = m_Scene->CreateEntity();
	bar_frame.AddComponent<Engine::Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -3.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(52.0f, 6.0f, 1.0f)  // scale
	);
	bar_frame.AddComponent<Renderable2D>(m_TowerBarFrameSpriteUUID);

	auto bar_fill = m_Scene->CreateEntity();
	bar_fill.AddComponent<Engine::Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -2.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(49.0f, 8.0f, 1.0f)  // scale
		);
	bar_fill.AddComponent<Renderable2D>(GetUnitBarSprite(towerConfig.Faction));
	bar_fill.AddComponent<UnitBar>(49.0f, position.x - 49.0f/2.0f, tower);

	auto bar_background = m_Scene->CreateEntity();
	bar_background.AddComponent<Engine::Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -1.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(49.0f, 6.0f, 1.0f)  // scale
		);
	bar_background.AddComponent<Renderable2D>(m_TowerBarBackgroundSpriteUUID);
	
	return tower;
}

Engine::Entity TowerBattleLayer::CreateAI(Faction faction)
{
	Engine::Entity ai = m_Scene->CreateEntity();
	ai.AddComponent<AIStrategist>(faction, 3.0f);

	return ai;
}

Engine::Entity TowerBattleLayer::CreateCamera()
{
	auto camera = m_Scene->CreateEntity();
	camera.AddComponent<Engine::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(1.0f, 1.0f, 1.0f)  // scale
	);
	camera.AddComponent<Engine::Components::OrthographicCamera>(
		(float)-1280 / 2, // left
		(float)1280 / 2, // right
		(float)-720 / 2, // bottom
		(float)720 / 2  // top
	);
	return camera;
}

Engine::Entity TowerBattleLayer::CreateSelection()
{
	m_Selection = m_Scene->CreateEntity();
	m_Selection.AddComponent<Engine::Transform>(
		Engine::Vec3(-1000.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(60.0f, 98.0f, 1.0f)  // scale
		);
	m_Selection.AddComponent<Engine::Components::Renderable2D>(m_SelectionSpriteUUID);

	return m_Selection;
}

enum class CellType : int
{
	Invalid = -1,
	Grass = 0,
	LeafForestLight,
	LeafForestDense,
	NeedleForestLight,
	NeedleForestDense,

	Length
};

static void PlaceTower(int x, int y, Faction faction, SoldierType soldierType, std::unordered_map<Engine::GridPoint, Tower> & tower_configs, Engine::Grid<std::vector<int>>& grid, const Engine::WFC::RuleSet& rules)
{
	grid.Get(x, y) = { (int)CellType::Grass };
	Engine::WFC::UpdateAdjecentCells(grid, x, y, rules);
	tower_configs[{x, y}] = { faction, soldierType };
}

void TowerBattleLayer::CreateLevel(int width, int height, int tileSize)
{
	int grid_width = width / tileSize + 1;
	int grid_height = height / tileSize + 1;

	Engine::WFC::RuleSet rules = {
		{(int)CellType::Grass,
			{(int)CellType::Grass, (int)CellType::LeafForestLight, (int)CellType::NeedleForestLight}},
		{(int)CellType::LeafForestLight,
			{(int)CellType::Grass, (int)CellType::LeafForestLight, (int)CellType::LeafForestDense}},
		{(int)CellType::LeafForestDense,
			{(int)CellType::LeafForestLight, (int)CellType::LeafForestDense}},
		{(int)CellType::NeedleForestLight,
			{(int)CellType::Grass, (int)CellType::NeedleForestLight, (int)CellType::NeedleForestDense}},
		{(int)CellType::NeedleForestDense,
			{(int)CellType::NeedleForestLight, (int)CellType::NeedleForestDense}}
	};

	// Allow any cell type except towers
	const int length = static_cast<size_t>(CellType::Length);
	std::vector<int> values(length);
	for (int i = (int)CellType::Grass; i < length; i++)
		values[i] = i;

	Engine::Grid<std::vector<int>> grid(grid_width, grid_height, values);

	// Place towers
	std::unordered_map<Engine::GridPoint, Tower> tower_configs;
	// Blue faction
	PlaceTower(5, 5, Faction::Blue, SoldierType::Light, tower_configs, grid, rules);
	// Red faction
	PlaceTower(15, 7, Faction::Red, SoldierType::Light, tower_configs, grid, rules);
	// Neutral
	PlaceTower(10, 5, Faction::None, SoldierType::Heavy, tower_configs, grid, rules);
	PlaceTower(10, 8, Faction::None, SoldierType::Heavy, tower_configs, grid, rules);

	Engine::WFC::CollapseGrid(grid, rules);

	std::vector<Engine::UUID> map[static_cast<size_t>(CellType::Length)] = {
		{ m_GrassSprite01UUID, m_GrassSprite02UUID },
		{ m_LeafForestLightSprite01UUID, m_LeafForestLightSprite02UUID },
		{ m_LeafForestDenseSprite01UUID, m_LeafForestDenseSprite02UUID },
		{ m_NeedleForestLightSprite01UUID, m_NeedleForestLightSprite02UUID },
		{ m_NeedleForestDenseSprite01UUID, m_NeedleForestDenseSprite02UUID }
	};

	// Spawn tiles for grid cells
	for (int x = 0; x < grid_width; x += 1)
	{
		for (int y = 0; y < grid_height; y += 1)
		{
			int pixel_x = int(x * tileSize - width / 2.0f);
			int pixel_y = int(y * tileSize - height / 2.0f);

			// Add tile
			Engine::Entity tile = m_Scene->CreateEntity();
			tile.AddComponent<Engine::Transform>(
				Engine::Vec3(pixel_x, pixel_y, 3.0f),				// position
				Engine::Vec3(0.0f, 0.0f, 0.0f),			// rotation
				Engine::Vec3(tileSize+1, tileSize+1, 1.0f)	// scale
			);
			int cell_type = grid.Get(x, y)[0];
			tile.AddComponent<Engine::Components::Renderable2D>(Engine::GetRandomEntry(map[cell_type]));

			if(tower_configs.find({x, y}) != tower_configs.end())
				CreateTower({ pixel_x, pixel_y, 0.0f }, tower_configs[{x, y}]);
		}
	}
}
