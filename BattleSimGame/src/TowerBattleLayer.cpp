#include "TowerBattleLayer.h"

#include <algorithm> 
#include <limits>

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

static std::string GetFactionSpriteID(Faction faction)
{
	switch (faction) {
	case Faction::Red: return "sprite/tower/red";
	case Faction::Blue: return "sprite/tower/blue";
	case Faction::None: return "sprite/tower/none";
	}

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return "";
}

static std::string GetUnitSpriteID(Faction faction)
{
	switch (faction) {
	case Faction::Red: return "sprite/unit/red";
	case Faction::Blue: return "sprite/unit/blue";
	}

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return "";
}

static std::string GetFactionSpritePath(Faction faction)
{
	switch (faction) {
	case Faction::Red:  return "res/sprite/Tower_Red.png";
	case Faction::Blue: return "res/sprite/Tower_Blue.png";
	case Faction::None: return "res/sprite/Tower_None.png";
	}

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return "";
}

static std::string GetUnitBarSprite(Faction faction)
{
	switch (faction)
	{
	case Faction::Red:  return "sprite/bar/fill/red";
	case Faction::Blue: return "sprite/bar/fill/blue";
	case Faction::None: return "sprite/bar/fill/neutral";
	}

	ENG_ASSERT_FMT(false, "Unsupported Faction {0}", faction);
	return "";
}

static void CollisionDetectionSystem(float deltaTime, Engine::Scene* scene, Engine::Entity entity, QuadCollider& collider)
{
	collider.Collisions.clear();

	auto view = scene->GetView<QuadCollider>();
	view.each([scene, entity, &collider](entt::entity otherId, QuadCollider& otherCollider) {
		// Test if the top-right corner is inside the other collider (both ways)
		if (otherCollider.IsInside(collider.GetTopRight()) || collider.IsInside(otherCollider.GetTopRight()))
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

static void UnitMoveSystem(float deltaTime, Engine::Entity entity, Unit& unit, QuadCollider& collider, Engine::Components::Transform& transform)
{
	Engine::Vec3 targetPosition = unit.Target.GetComponent<Engine::Components::Transform>().Position;
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

static void UnitAttackTowerSystem(float deltaTime, Engine::Entity entity, Unit& unit, Engine::Components::Transform& transform)
{
	Engine::Vec3 targetPosition = unit.Target.GetComponent<Engine::Components::Transform>().Position;
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

	renderable.Data[0].SpriteID = GetFactionSpriteID(tower.Faction);
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

static void UnitBarUISystem(float delta_time, Engine::Entity entity, UnitBar& unitBar, Engine::Components::Transform& transform, Engine::Components::Renderable2D& renderable)
{
	Tower& tower = unitBar.TowerEntity.GetComponent<Tower>();
	float fraction = tower.Units / (float)tower.MaxUnits;

	transform.Scale.x = unitBar.MaxWidth * fraction;
	transform.Position.x = unitBar.X + unitBar.MaxWidth / 2.0f * fraction;

	renderable.Data[0].SpriteID = GetUnitBarSprite(tower.Faction);
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

TowerBattleLayer::TowerBattleLayer(Engine::Scene* scene)
	: Layer("TowerBattle")
{
	m_Scene = scene;

	using namespace Engine;

	// Prepare assets
	TextureAtlas* atlas = new Engine::TextureAtlas("res/sprite/medieval_sprite_pack.png");
	AssetRegistry::Add("atlas", atlas);
	Shader* shader = new Shader("res/shader/pixel_sprite.shader");
	shader->SetProperty("u_TexelPerPixel", 50.0f);
	AssetRegistry::Add("shader/sprite", shader);

	AssetRegistry::Add(GetFactionSpriteID(Faction::Blue), new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_blue")));
	AssetRegistry::Add(GetFactionSpriteID(Faction::Red), new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_red")));
	AssetRegistry::Add(GetFactionSpriteID(Faction::None), new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_neutral")));

	AssetRegistry::Add("sprite/unit/blue", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("spearman_blue")));
	AssetRegistry::Add("sprite/unit/red", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("spearman_red")));

	AssetRegistry::Add("sprite/selection", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_selection")));
	AssetRegistry::Add("sprite/bar/frame", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_bar_frame")));
	AssetRegistry::Add("sprite/bar/fill/neutral", new Sprite("shader/sprite", "atlas", Vec4(1.0f, 1.0f, 1.0f, 1.0f), atlas->FindSubTexIndex("tower_bar_fill")));
	AssetRegistry::Add("sprite/bar/fill/red", new Sprite("shader/sprite", "atlas", Vec4(233.0f/255.0f, 125.0f/255.0f, 85.0f/255.0f, 1.0f), atlas->FindSubTexIndex("tower_bar_fill")));
	AssetRegistry::Add("sprite/bar/fill/blue", new Sprite("shader/sprite", "atlas", Vec4(85.0f/255.0f, 173.0f/255.0f, 233.0f/255.0f, 1.0f), atlas->FindSubTexIndex("tower_bar_fill")));
	AssetRegistry::Add("sprite/bar/background", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("tower_bar_background")));

	Debug::SetShader("res/shader/debug.shader");

	CreateCamera();
	CreateSelection();

	CreateTower({ 100.0f, 0.0f, 0.0f }, Faction::None);
	CreateTower({ 200.0f, 50.0f, 0.0f }, Faction::Red);
	CreateTower({ -50.0f, 75.0f, 0.0f }, Faction::Blue);

	CreateAI(Faction::Blue);
	CreateAI(Faction::Red);

	AssetRegistry::Add("sprite/grass01", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("grass_01")));
	AssetRegistry::Add("sprite/grass02", new Sprite("shader/sprite", "atlas", atlas->FindSubTexIndex("grass_02")));

	CreateBackground(1280, 720, 64);

	m_GameRunning = true;
}

TowerBattleLayer::~TowerBattleLayer()
{
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	if (!m_GameRunning)
		return;

	// Update collision detection
	m_Scene->ExecuteSystem<QuadCollider>(deltaTime, CollisionDetectionSystem);
	// Update units
	m_Scene->ExecuteSystem<EnemyContact, Unit, QuadCollider>(deltaTime, EnemyContactSystem);
	m_Scene->ExecuteSystem<Unit, EnemyContact>(deltaTime, UnitAttackUnitSystem);
	m_Scene->ExecuteSystem<Unit>(deltaTime, UnitDeathSystem);
	m_Scene->ExecuteSystem<Unit, QuadCollider, Engine::Components::Transform>(deltaTime, UnitMoveSystem);
	m_Scene->ExecuteSystem<Unit, Engine::Components::Transform>(deltaTime, UnitAttackTowerSystem);
	// Update towers
	m_Scene->ExecuteSystem<Tower>(deltaTime, TowerProductionSystem);
	m_Scene->ExecuteSystem<Tower, Engine::Components::Renderable2D>(deltaTime, TowerViewSystem);
	// Update AI
	m_Scene->ExecuteSystem<AIStrategist>(deltaTime, AIStrategistSystem);
	// Update UI
	m_Scene->ExecuteSystem<UnitBar, Engine::Components::Transform>(deltaTime, UnitBarUISystem);

	// Debug
	m_Scene->ExecuteSystem<QuadCollider>(deltaTime, DrawCollidersSystem);

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
			m_Selection.GetComponent<Transform>().Position = towerEntity.GetComponent<Transform>().Position;
		}
	} else {
		if (m_SourceTower == towerEntity) {
			ENG_TRACE("Deselected {0}", towerEntity);
			m_SourceTower = Engine::Entity();

			if (tower.Faction == Faction::Blue)
				m_Selection.GetComponent<Transform>().Position = Engine::Vec3(-1000.0f, 0.0f, 0.0f);
		}
		else {
			Attack(m_SourceTower, towerEntity);

			Tower& src_tower_comp = m_SourceTower.GetComponent<Tower>();
			src_tower_comp.Selected = false;
			m_Selection.GetComponent<Transform>().Position = Engine::Vec3(-1000.0f, 0.0f, 0.0f);
			
			m_SourceTower = Engine::Entity();
		}
	}
}

static void SpawnUnit(Engine::Scene& scene, Engine::Entity sourceTower, Engine::Entity targetTower, size_t index)
{
	using namespace Engine::Components;

	Faction faction = sourceTower.GetComponent<Tower>().Faction;
	
	// Choose position at random point on circle around source tower
	Engine::Vec3 towerPosition = sourceTower.GetComponent<Transform>().Position;
	Engine::Vec3 targetPosition = targetTower.GetComponent<Transform>().Position;
	Engine::Vec3 direction = Engine::Normalize(targetPosition - towerPosition);

	int sign = index % 2 == 0 ? 1 : -1;
	float angle = Engine::DegToRad(30.0f * index / 2 * sign);
	float radius = 30.0f;
	Engine::Vec3 position = towerPosition + Engine::RotateZ(direction, angle) * radius;

	float faceDirection = direction.x != 0.0f ? Engine::Sign(direction.x) : 1.0f;

	Engine::Entity unit = scene.CreateEntity();
	unit.AddComponent<Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(faceDirection * 18.0f, 25.0f, 1.0f)  // scale
		);
	unit.AddComponent<Unit>(faction, targetTower);
	unit.AddComponent<EnemyContact>();
	unit.AddComponent<Renderable2D>(GetUnitSpriteID(faction));
	unit.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		18.0f, // width
		25.0f // height
	);
}

void TowerBattleLayer::Attack(Engine::Entity source, Engine::Entity target)
{
	ENG_TRACE("Attack from {0} to {1}", source, target);
	ENG_TRACE("Deselected {0}", source);
	auto& srcTower = source.GetComponent<Tower>();

	// take units from source tower
	int units = std::min((int)srcTower.Units, 5);
	ChangeTowerUnits(source, -units);
	ENG_TRACE("{0} units move from source (Faction: {1}, Units: {2})", units, srcTower.Faction, srcTower.Units);

	for (size_t i=0; i<units; i++)
		SpawnUnit(*m_Scene, source, target, i);
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

Engine::Entity TowerBattleLayer::CreateTower(Engine::Vec3 position, Faction faction)
{
	using namespace Engine::Components;
	
	auto tower = m_Scene->CreateEntity();
	tower.AddComponent<Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(44.0f, 84.0f, 1.0f)  // scale
	);
	tower.AddComponent<Renderable2D>(GetFactionSpriteID(faction));
	tower.AddComponent<Tower>(faction, (unsigned int)0, (unsigned int)36, 1.0f, 0.0f);
	tower.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);

	auto bar_frame = m_Scene->CreateEntity();
	bar_frame.AddComponent<Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -3.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(52.0f, 6.0f, 1.0f)  // scale
	);
	bar_frame.AddComponent<Renderable2D>("sprite/bar/frame");

	auto bar_fill = m_Scene->CreateEntity();
	bar_fill.AddComponent<Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -2.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(49.0f, 8.0f, 1.0f)  // scale
		);
	bar_fill.AddComponent<Renderable2D>(GetUnitBarSprite(faction));
	bar_fill.AddComponent<UnitBar>(49.0f, position.x - 49.0f/2.0f, tower);

	auto bar_background = m_Scene->CreateEntity();
	bar_background.AddComponent<Transform>(
		position + Engine::Vec3(0.0f, 52.0f, -1.0f),
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(49.0f, 6.0f, 1.0f)  // scale
		);
	bar_background.AddComponent<Renderable2D>("sprite/bar/background");
	
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
	camera.AddComponent<Engine::Components::Transform>(
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
	m_Selection.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(-1000.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(60.0f, 98.0f, 1.0f)  // scale
		);
	m_Selection.AddComponent<Engine::Components::Renderable2D>("sprite/selection");

	return m_Selection;
}

void TowerBattleLayer::CreateBackground(int width, int height, int tileSize)
{
	int grid_width = width / tileSize + 1;
	int grid_height = height / tileSize + 1;

	Engine::Grid<int> grid(grid_width, grid_height);
	for (int x = 0; x < grid_width; x += 1)
	{
		for (int y = 0; y < grid_height; y += 1)
		{
			grid(x, y) = 0;
		}
	}

	std::vector<std::string> map[1] = {
		{ "sprite/grass01", "sprite/grass02" }
	};

	for (int x = 0; x < grid_width; x += 1)
	{
		for (int y = 0; y < grid_height; y += 1)
		{
			int pixel_x = x * tileSize - width / 2.0f;
			int pixel_y = y * tileSize - height / 2.0f;


			Engine::Entity tile = m_Scene->CreateEntity();
			tile.AddComponent<Engine::Components::Transform>(
				Engine::Vec3(pixel_x, pixel_y, 3.0f),				// position
				Engine::Vec3(0.0f, 0.0f, 0.0f),			// rotation
				Engine::Vec3(tileSize+1, tileSize+1, 1.0f)	// scale
			);
			tile.AddComponent<Engine::Components::Renderable2D>(Engine::GetRandomEntry(map[grid(x, y)]));
		}
	}
}
