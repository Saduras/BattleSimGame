#include "TowerBattleLayer.h"

#include <algorithm> 

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

static std::string GetFactionSpriteID(Faction faction, bool selected)
{
	switch (faction) {
	case Faction::Red: 
		return selected ? "sprite/tower/red/selected" : "sprite/tower/red/unselected";
	case Faction::Blue:
		return selected ? "sprite/tower/blue/selected" : "sprite/tower/blue/unselected";
	case Faction::None:
		return selected ? "sprite/tower/none/selected" : "sprite/tower/none/unselected";
	}

	ENG_ASSERT(false, "Unsupported Faction {0}", faction);
	return "";
}

static std::string GetUnitSpriteID(Faction faction)
{
	switch (faction) {
	case Faction::Red: return "sprite/unit/red";
	case Faction::Blue: return "sprite/unit/blue";
	}

	ENG_ASSERT(false, "Unsupported Faction {0}", faction);
	return "";
}

static std::string GetFactionSpritePath(Faction faction)
{
	switch (faction) {
	case Faction::Red:  return "res/sprite/Tower_Red.png";
	case Faction::Blue: return "res/sprite/Tower_Blue.png";
	case Faction::None: return "res/sprite/Tower_None.png";
	}

	ENG_ASSERT(false, "Unsupported Faction {0}", faction);
	return "";
}

class UnitMoveSystem : public Engine::System
{
public:
	UnitMoveSystem(Engine::Scene* scene) : Engine::System(scene) {}

	void Execute(float deltaTime) override
	{
		using namespace Engine::Components;

		auto view = m_Scene->GetView<Unit, Transform>();
		view.each([deltaTime](Unit& unit, Transform& transform) {

			Engine::Vec3 targetPosition = unit.Target.GetComponent<Transform>().Position;
			Engine::Vec3 currentPosition = transform.Position;
			Engine::Vec3 distance = targetPosition - currentPosition;
			
			if(Engine::Magnitude(distance) > 0.1f)
				transform.Position += Engine::Normalize(distance) * unit.Speed * deltaTime;
		});
	}
	std::string GetName() const override { return "UnitMoveSystem"; }
};

class UnitAttackSystem : public Engine::System
{
public:
	UnitAttackSystem(Engine::Scene* scene) : Engine::System(scene) {}

	void Execute(float deltaTime) override
	{
		using namespace Engine::Components;

		auto view = m_Scene->GetView<Unit, Transform>();
		view.each([this, deltaTime](const entt::entity& entity, Unit& unit, Transform& transform) {

			Engine::Vec3 targetPosition = unit.Target.GetComponent<Transform>().Position;
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
				TowerBattleLayer::CheckVictoryCondition(m_Scene);
			} 
			else 
			{
				// Damage tower
				TowerBattleLayer::ChangeTowerUnits(unit.Target, -1);
			}

			m_Scene->DestroyEntity(Engine::Entity(entity, m_Scene));
			
			ENG_TRACE("Unit reached target (Unit Faction: {1}, Target Faction: {2})", unit.Faction, targetTower.Faction);
		});
	}
	std::string GetName() const override { return "UnitAttackSystem"; }
};

class TowerViewSystem : public Engine::System
{
public:
	TowerViewSystem(Engine::Scene* scene) : Engine::System(scene) {}

	void Execute(float deltaTime) override
	{
		using namespace Engine::Components;

		auto view = m_Scene->GetView<Tower, Renderable2D, UpdateView>();
		view.each([this, deltaTime](const entt::entity& entity, Tower& tower, Renderable2D& renderable, UpdateView& updateView) {
			size_t startIndex = renderable.MeshID.find_last_of('/') + 1;
			renderable.MeshID.replace(startIndex, renderable.MeshID.length() - startIndex, std::to_string(tower.Units));
			renderable.SpriteID = GetFactionSpriteID(tower.Faction, tower.Selected);
			Engine::Entity(entity, m_Scene).RemoveComponent<UpdateView>();
		});
	}

	std::string GetName() const override { return "TowerViewSystem"; }
};

class TowerProductionSystem : public Engine::System
{
public:
	TowerProductionSystem(Engine::Scene* scene) : Engine::System(scene) {}

	void Execute(float deltaTime) override
	{
		using namespace Engine::Components;

		auto view = m_Scene->GetView<Tower>();
		view.each([this, deltaTime](const entt::entity& entity, Tower& tower) {
			if (tower.Units >= tower.MaxUnits || tower.Faction == Faction::None)
				return;

			tower.NextProductionTime -= deltaTime;
			if (tower.NextProductionTime <= 0) {
				tower.NextProductionTime += tower.ProductionIntervall;
				TowerBattleLayer::ChangeTowerUnits(Engine::Entity(entity, m_Scene), +1);

				ENG_TRACE("Faction {0} got one unit. Currently {1}/{2}", tower.Faction, tower.Units, tower.MaxUnits);
			}
		});
	}

	std::string GetName() const override { return "TowerProductionSystem"; }
};

bool TowerBattleLayer::m_GameRunning = false;

TowerBattleLayer::TowerBattleLayer(Engine::Scene& scene)
	: Layer("TowerBattle"), m_Scene(scene)
{
	// Prepare assets
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::TextureQuad));

	Engine::Shader* shader = new Engine::Shader("res/shader/pixel_sprite.shader");
	shader->SetProperty("u_TexelPerPixel", 5.0f);
	Engine::AssetRegistry::Add("shader/sprite", shader);

	std::vector<Engine::TextureCoordinates> texCoords;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			texCoords.push_back({ 
				x * 16.0f / 128.0f,
				(128 - (y + 1) * 16.0f) / 128.0f, 
				(x + 1) * 16.0f / 128.0f,
				(128 - y * 16.0f) / 128.0f
			});
		}
	}

	// Player faction sprites
	Engine::Sprite* towerSpriteSelected = new Engine::Sprite("shader/sprite", "res/sprite/Tower_Blue_Selected.png");
	towerSpriteSelected->SetTextureCoordinates(texCoords);
	Engine::Sprite* towerSpriteUnselected = new Engine::Sprite("shader/sprite", "res/sprite/Tower_Blue.png");
	towerSpriteUnselected->SetTextureCoordinates(texCoords);
	Engine::AssetRegistry::Add(GetFactionSpriteID(Faction::Blue, true), towerSpriteSelected);
	Engine::AssetRegistry::Add(GetFactionSpriteID(Faction::Blue, false), towerSpriteUnselected);

	// Other sprites
	for (Faction faction : { Faction::Red, Faction::None })
	{
		Engine::Sprite* towerSprite = new Engine::Sprite("shader/sprite", GetFactionSpritePath(faction));
		towerSprite->SetTextureCoordinates(texCoords);
		Engine::AssetRegistry::Add(GetFactionSpriteID(faction, true), towerSprite);
		Engine::AssetRegistry::Add(GetFactionSpriteID(faction, false), towerSprite);

	}

	Engine::Sprite& sprite = Engine::AssetRegistry::Get<Engine::Sprite>(GetFactionSpriteID(Faction::None, false));
	for (size_t i = 0; i < sprite.GetTileCount(); i++)
	{
		Engine::TextureCoordinates texCoords = sprite.GetTextureCoordinates((int)i);
		Engine::MeshData meshData = Engine::Mesh::PrimitiveToMeshData(Engine::PrimitiveMesh::TextureQuad);
		Engine::SetTextureCoordinatesOnMeshData(texCoords, meshData, 3, 5);
		Engine::AssetRegistry::Add(Engine::FormatString("mesh/quad/{}", i), new Engine::Mesh(meshData));
	}

	Engine::AssetRegistry::Add("sprite/unit/blue", new Engine::Sprite("shader/sprite", "res/sprite/Unit_Blue.png"));
	Engine::AssetRegistry::Add("sprite/unit/red", new Engine::Sprite("shader/sprite", "res/sprite/Unit_Red.png"));
	Engine::AssetRegistry::Add("mesh/quad/unit", new Engine::Mesh(Engine::PrimitiveMesh::TextureQuad));

	CreateCamera();

	CreateTower({ 100.0f, 0.0f, 0.0f }, Faction::None);
	CreateTower({ 200.0f, 50.0f, 0.0f }, Faction::Red);
	CreateTower({ -50.0f, 75.0f, 0.0f }, Faction::Blue);

	m_Scene.AddSystem<UnitMoveSystem>();
	m_Scene.AddSystem<UnitAttackSystem>();
	m_Scene.AddSystem<TowerViewSystem>();
	m_Scene.AddSystem<TowerProductionSystem>();

	m_GameRunning = true;
}

TowerBattleLayer::~TowerBattleLayer()
{
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	if (!m_GameRunning)
		return;

	m_Scene.Update(deltaTime);
}

void TowerBattleLayer::OnEvent(Engine::Event& event)
{
	Engine::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Engine::MouseButtonPressedEvent>(ENG_BIND_EVENT_FN(TowerBattleLayer::OnMouseButtonPressed));
}

bool TowerBattleLayer::OnMouseButtonPressed(Engine::MouseButtonPressedEvent& event)
{
	using namespace Engine::Components;
	// TODO get const for left mouse button
	if (event.GetMouseButton() == 0) {
		float x = Engine::Input::GetMouseX();
		float y = Engine::Input::GetMouseY();
		
		auto cameras = m_Scene.GetView<OrthographicCamera>();
		cameras.each([x, y, this](OrthographicCamera& camera) {
			// Translate screen position to world position
			float screenWidth = (float)Engine::Application::Get().GetWindow().GetWidth();
			float screenHeight = (float)Engine::Application::Get().GetWindow().GetHeight();
			Engine::Vec4 worldPos = Camera::ScreenToWorld(camera, { x, y }, screenWidth, screenHeight);

			// Check collision
			m_Scene.GetView<QuadCollider, Tower, Renderable2D>().each([&worldPos, this](const entt::entity entityID, auto& collider, auto& tower, auto& renderable) {
				ENG_TRACE("Hit {0}", collider.IsInside({ worldPos.x, worldPos.y }));
				if (collider.IsInside({ worldPos.x, worldPos.y })) {
					this->OnTowerClick({ entityID, &m_Scene });
				}
			});
		});
	}

	return true;
}

void TowerBattleLayer::OnTowerClick(Engine::Entity towerEntity)
{
	using namespace Engine::Components;

	auto& renderable = towerEntity.GetComponent<Renderable2D>();
	auto& tower = towerEntity.GetComponent<Tower>();

	if (m_SourceTower.IsNull()) {
		ENG_TRACE("Selected {0}", towerEntity);
		m_SourceTower = towerEntity;

		if (tower.Faction == Faction::Blue) 
		{
			tower.Selected = true;
			if(!towerEntity.HasComponent<UpdateView>())
				towerEntity.AddComponent<UpdateView>();
		}
	} else {
		if (m_SourceTower == towerEntity) {
			ENG_TRACE("Deselected {0}", towerEntity);
			m_SourceTower = Engine::Entity();

			if (tower.Faction == Faction::Blue)
			{
				tower.Selected = false;
				if (!towerEntity.HasComponent<UpdateView>())
					towerEntity.AddComponent<UpdateView>();
			}
		}
		else {
			Attack(m_SourceTower, towerEntity);
			m_SourceTower.GetComponent<Tower>().Selected = false;
			if (!m_SourceTower.HasComponent<UpdateView>())
				m_SourceTower.AddComponent<UpdateView>();
			
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
	float angle = Engine::DegToRad(10.0f * index);
	float radius = 50.0f;
	Engine::Vec3 position = towerPosition + Engine::Vec3(std::cos(angle), std::sin(angle), 0.0f) * radius;

	Engine::Entity unit = scene.CreateEntity();
	unit.AddComponent<Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)  // scale
		);
	unit.AddComponent<Unit>(faction, targetTower);
	unit.AddComponent<Renderable2D>(GetUnitSpriteID(faction), "mesh/quad/unit");
	unit.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);
}

void TowerBattleLayer::Attack(Engine::Entity source, Engine::Entity target)
{
	ENG_TRACE("Attack from {0} to {1}", source, target);
	ENG_TRACE("Deselected {0}", source);
	auto& srcRenderable = source.GetComponent<Engine::Components::Renderable2D>();
	auto& srcTower = source.GetComponent<Tower>();
	srcRenderable.SpriteID = GetFactionSpriteID(srcTower.Faction, false);

	// take units from source tower
	int units = std::min((int)srcTower.Units, 5);
	ChangeTowerUnits(source, -units);
	ENG_TRACE("{0} units move from source (Faction: {1}, Units: {2})", units, srcTower.Faction, srcTower.Units);

	for (size_t i=0; i<units; i++)
		SpawnUnit(m_Scene, source, target, i);
}

void TowerBattleLayer::CheckVictoryCondition(Engine::Scene* scene)
{
	int redTowerCount = 0;
	int blueTowerCount = 0;
	auto towerView = scene->GetView<Tower>();
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
	if(!towerEntity.HasComponent<UpdateView>())
		towerEntity.AddComponent<UpdateView>();
}

Engine::Entity TowerBattleLayer::CreateTower(Engine::Vec3 position, Faction faction)
{
	using namespace Engine::Components;
	
	auto tower = m_Scene.CreateEntity();
	tower.AddComponent<Transform>(
		position,
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)  // scale
	);
	tower.AddComponent<Renderable2D>(GetFactionSpriteID(faction, false), "mesh/quad/0");
	tower.AddComponent<Tower>(faction, (unsigned int)0, (unsigned int)36, 1.0f, 0.0f);
	tower.AddComponent<QuadCollider>(
		Engine::Vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);
	
	return tower;
}

Engine::Entity TowerBattleLayer::CreateCamera()
{
	auto camera = m_Scene.CreateEntity();
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
