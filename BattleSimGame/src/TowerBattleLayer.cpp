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

TowerBattleLayer::TowerBattleLayer(Engine::Scene& scene)
	: Layer("TowerBattle"), m_Scene(scene)
{
	// Prepare assets
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::TextureQuad));
	Engine::AssetRegistry::Add("shader/sprite", new Engine::Shader("res/shader/unlit_texture.shader"));

	std::vector<Engine::TextureCoordinates> texCoords;
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			// TODO there is a bug in the tex coord calculation...
			texCoords.push_back({ 
				x * 16.0f / 256.0f, 
				(256 - y * 16.0f) / 256.0f, 
				(x + 1) * 16.0f / 256.0f, 
				(256 - (y + 1) * 16.0f) / 256.0f 
			});
		}
	}

	for (Faction faction : { Faction::Blue, Faction::Red, Faction::None })
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

	//auto matNoneBase = new Engine::Material("res/shader/default.shader");
	//matNoneBase->SetColor({0.4f, 0.4f, 0.4f, 1.0f});
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::None, false), matNoneBase);
	//auto matNoneSelect = new Engine::Material("res/shader/default.shader");
	//matNoneSelect->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::None, true), matNoneSelect);

	//auto matRedBase = new Engine::Material("res/shader/default.shader");
	//matRedBase->SetColor({ 0.7f, 0.2f, 0.2f, 1.0f });
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Red, false), matRedBase);
	//auto matRedSelect = new Engine::Material("res/shader/default.shader");
	//matRedSelect->SetColor({ 0.8f, 0.4f, 0.4f, 1.0f });
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Red, true), matRedSelect);

	//auto matBlueBase = new Engine::Material("res/shader/default.shader");
	//matBlueBase->SetColor({ 0.2f, 0.2f, 0.7f, 1.0f });
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Blue, false), matBlueBase);
	//auto matBlueSelect = new Engine::Material("res/shader/default.shader");
	//matBlueSelect->SetColor({ 0.4f, 0.4f, 0.8f, 1.0f });
	//Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Blue, true), matBlueSelect);

	CreateCamera();

	CreateTower({ 100.0f, 0.0f, 0.0f }, Faction::None);
	CreateTower({ 200.0f, 50.0f, 0.0f }, Faction::Red);
	CreateTower({ -50.0f, 75.0f, 0.0f }, Faction::Blue);
}

TowerBattleLayer::~TowerBattleLayer()
{
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	auto towerView = m_Scene.GetView<Tower>();
	towerView.each([&deltaTime, this](Tower& tower) { this->UpdateTower(tower, deltaTime); });

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
			m_Scene.GetView<QuadCollider, Tower, Renderable3D>().each([&worldPos, this](const entt::entity entityID, auto& collider, auto& tower, auto& renderable) {
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

	auto& renderable = towerEntity.GetComponent<Renderable3D>();
	auto& tower = towerEntity.GetComponent<Tower>();

	if (m_SourceTower.IsNull()) {
		ENG_TRACE("Selected {0}", towerEntity);
		m_SourceTower = towerEntity;
		renderable.MaterialID = GetFactionSpriteID(tower.Faction, true);
	} else {
		if (m_SourceTower == towerEntity) {
			ENG_TRACE("Deselected {0}", towerEntity);
			m_SourceTower = Engine::Entity();
			renderable.MaterialID = GetFactionSpriteID(tower.Faction, false);
		}
		else {
			Attack(m_SourceTower, towerEntity);
			m_SourceTower = Engine::Entity();
		}
	}
}

void TowerBattleLayer::UpdateTower(Tower& tower, float deltaTime)
{
	if (tower.Units >= tower.MaxUnits || tower.Faction == Faction::None) {
		return;
	}

	tower.NextProductionTime -= deltaTime;
	if (tower.NextProductionTime <= 0 ) {
		tower.NextProductionTime += tower.ProductionIntervall;
		tower.Units += 1;
		ENG_TRACE("Faction {0} got one unit. Currently {1}/{2}", tower.Faction, tower.Units, tower.MaxUnits);
	}
}

// TODO add visualization of attack
void TowerBattleLayer::Attack(Engine::Entity source, Engine::Entity target)
{
	ENG_TRACE("Attack from {0} to {1}", source, target);
	ENG_TRACE("Deselected {0}", source);
	auto& srcRenderable = source.GetComponent<Engine::Components::Renderable3D>();
	auto& srcTower = source.GetComponent<Tower>();
	srcRenderable.MaterialID = GetFactionSpriteID(srcTower.Faction, false);

	// take units from source tower
	unsigned int units = std::min(srcTower.Units, (unsigned int)5);
	srcTower.Units -= units;
	ENG_TRACE("{0} units move from source (Faction: {1}, Units: {2})", units, srcTower.Faction, srcTower.Units);

	auto& targetTower = target.GetComponent<Tower>();
	if (targetTower.Faction == srcTower.Faction) {
		// Add to target if same faction
		targetTower.Units += units;
	} else {
		int diff = (int)targetTower.Units - (int)units;
		if (diff >= 0) {
			targetTower.Units -= units;
		} else {
			// tower conquered
			targetTower.Faction = srcTower.Faction;
			targetTower.Units = (unsigned int)(-1 * diff);
			auto& targetRenderable = target.GetComponent<Engine::Components::Renderable3D>();
			targetRenderable.MaterialID = GetFactionSpriteID(targetTower.Faction, false);
		}
	}
	ENG_TRACE("{0} units reached target (Faction: {1}, Units {2})", units, targetTower.Faction, targetTower.Units);
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
	tower.AddComponent<Tower>(faction, (unsigned int)0, (unsigned int)10, 1.0f, 0.0f);
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
