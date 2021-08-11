#include "TowerBattleLayer.h"

#include <algorithm> 

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>

static std::string GetFactionMaterialID(Faction faction, bool selected)
{
	switch (faction) {
	case Faction::Red: 
		return selected ? "material/tower/red/selected" : "material/tower/red/unselected";
	case Faction::Blue:
		return selected ? "material/tower/blue/selected" : "material/tower/blue/unselected";
	case Faction::None:
		return selected ? "material/tower/none/selected" : "material/tower/none/unselected";
	}

	ENG_ASSERT(false, "Unsupported Faction {0}", faction);
	return "";
}

struct QuadCollider
{
	glm::vec2 Center;
	float Width;
	float Height;

	bool IsInside(glm::vec2 position)
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
	: Layer("TowerBattle"), m_SourceTower(entt::null), m_Scene(scene)
{
	// Prepare assets
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));

	auto matNoneBase = new Engine::Material("res/shader/default.shader");
	matNoneBase->SetColor({0.4f, 0.4f, 0.4f, 1.0f});
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::None, false), matNoneBase);
	auto matNoneSelect = new Engine::Material("res/shader/default.shader");
	matNoneSelect->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::None, true), matNoneSelect);

	auto matRedBase = new Engine::Material("res/shader/default.shader");
	matRedBase->SetColor({ 0.7f, 0.2f, 0.2f, 1.0f });
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Red, false), matRedBase);
	auto matRedSelect = new Engine::Material("res/shader/default.shader");
	matRedSelect->SetColor({ 0.8f, 0.4f, 0.4f, 1.0f });
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Red, true), matRedSelect);

	auto matBlueBase = new Engine::Material("res/shader/default.shader");
	matBlueBase->SetColor({ 0.2f, 0.2f, 0.7f, 1.0f });
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Blue, false), matBlueBase);
	auto matBlueSelect = new Engine::Material("res/shader/default.shader");
	matBlueSelect->SetColor({ 0.4f, 0.4f, 0.8f, 1.0f });
	Engine::AssetRegistry::Add(GetFactionMaterialID(Faction::Blue, true), matBlueSelect);

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
			glm::vec4 worldPos = camera.ScreenToWorld({ x, y }, screenWidth, screenHeight);

			// Check collision
			m_Scene.GetView<QuadCollider, Tower, Renderable>().each([&worldPos, this](const entt::entity entity, auto& collider, auto& tower, auto& renderable) {
				ENG_TRACE("Hit {0}", collider.IsInside({ worldPos.x, worldPos.y }));
				if (collider.IsInside({ worldPos.x, worldPos.y })) {
					this->OnTowerClick(entity);
				}
			});
		});
	}

	return true;
}

void TowerBattleLayer::OnTowerClick(entt::entity towerEntity)
{
	using namespace Engine::Components;

	auto& renderable = m_Scene.GetComponent<Renderable>(towerEntity);
	auto& tower = m_Scene.GetComponent<Tower>(towerEntity);

	if (m_SourceTower == entt::null) {
		ENG_TRACE("Selected {0}", towerEntity);
		m_SourceTower = towerEntity;
		renderable.MaterialID = GetFactionMaterialID(tower.Faction, true);
	} else {
		if (m_SourceTower == towerEntity) {
			ENG_TRACE("Deselected {0}", towerEntity);
			m_SourceTower = entt::null;
			renderable.MaterialID = GetFactionMaterialID(tower.Faction, false);
		}
		else {
			Attack(m_SourceTower, towerEntity);
			m_SourceTower = entt::null;
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
void TowerBattleLayer::Attack(entt::entity source, entt::entity target)
{
	ENG_TRACE("Attack from {0} to {1}", source, target);
	ENG_TRACE("Deselected {0}", source);
	auto& srcRenderable = m_Scene.GetComponent<Engine::Components::Renderable>(source);
	auto& srcTower = m_Scene.GetComponent<Tower>(source);
	srcRenderable.MaterialID = GetFactionMaterialID(srcTower.Faction, false);

	// take units from source tower
	unsigned int units = std::min(srcTower.Units, (unsigned int)5);
	srcTower.Units -= units;
	ENG_TRACE("{0} units move from source (Faction: {1}, Units: {2})", units, srcTower.Faction, srcTower.Units);

	auto& targetTower = m_Scene.GetComponent<Tower>(target);
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
			auto& targetRenderable = m_Scene.GetComponent<Engine::Components::Renderable>(target);
			targetRenderable.MaterialID = GetFactionMaterialID(targetTower.Faction, false);
		}
	}
	ENG_TRACE("{0} units reached target (Faction: {1}, Units {2})", units, targetTower.Faction, targetTower.Units);
}

entt::entity TowerBattleLayer::CreateTower(glm::vec3 position, Faction faction)
{
	using namespace Engine::Components;
	
	auto tower = m_Scene.CreateEntity();
	m_Scene.EmplaceComponent<Transform>(tower,
		position,
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation
		glm::vec3(50.0f, 100.0f, 1.0f)  // scale
	);
	m_Scene.EmplaceComponent<Renderable>(tower, GetFactionMaterialID(faction, false), "mesh/quad");
	m_Scene.EmplaceComponent<Tower>(tower, faction, (unsigned int)0, (unsigned int)10, 1.0f, 0.0f);
	m_Scene.EmplaceComponent<QuadCollider>(tower,
		glm::vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);
	
	return tower;
}

entt::entity TowerBattleLayer::CreateCamera()
{
	auto camera = m_Scene.CreateEntity();
	m_Scene.EmplaceComponent<Engine::Components::Transform>(camera,
		glm::vec3(0.0f, 0.0f, 0.0f), // position
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation
		glm::vec3(1.0f, 1.0f, 1.0f)  // scale
	);
	m_Scene.EmplaceComponent<Engine::Components::OrthographicCamera>(camera,
		(float)-1280 / 2, // left
		(float)1280 / 2, // right
		(float)-720 / 2, // bottom
		(float)720 / 2  // top
	);
	return camera;
}
