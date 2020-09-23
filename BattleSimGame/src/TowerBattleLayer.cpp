#include "TowerBattleLayer.h"


struct Tower
{
	bool Selected;
	std::string SelectedMaterial;
	std::string UnselectedMaterial;
};

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

TowerBattleLayer::TowerBattleLayer()
	: Layer("TowerBattle")
{
	// Prepare assets
	auto mat = new Engine::Material("res/shader/default.shader");
	mat->SetColor({0.4f, 0.4f, 0.4f, 1.0f});
	Engine::AssetRegistry::Add("material/tower", mat);
	auto selectedMat = new Engine::Material("res/shader/default.shader");
	selectedMat->SetColor({ 0.6f, 0.6f, 0.6f, 1.0f });
	Engine::AssetRegistry::Add("material/tower/selected", selectedMat);
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));

	m_CameraEntity = CreateCamera();

	CreateTower({ 100.0f, 0.0f, 0.0f });
	CreateTower({ 200.0f, 50.0f, 0.0f });
	CreateTower({ -50.0f, 75.0f, 0.0f });
}

TowerBattleLayer::~TowerBattleLayer()
{
}

void RenderRenerable(const Engine::Components::Transform& transform, const Engine::Components::Renderable& renderable)
{
	auto& material = Engine::AssetRegistry::GetMaterial(renderable.MaterialID);
	auto& mesh = Engine::AssetRegistry::GetMesh(renderable.MeshID);

	auto shader = material.GetShader();
	Engine::Renderer::SetShader(shader);
	auto modelMatrix = transform.GetTransformationMatrix();
	shader->SetUniformMat4f("u_Model", modelMatrix);
	shader->SetUniform4f("u_Color", material.GetColor());
	Engine::Renderer::Submit(mesh.GetVertexArray());
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	using namespace Engine::Components;

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	auto proj = m_Registry.get<OrthographicCamera>(m_CameraEntity).GetProjectionMatrix();
	auto view = glm::inverse(m_Registry.get<Transform>(m_CameraEntity).GetTransformationMatrix());
	Engine::Renderer::BeginScene(proj * view);

	// Render all entities with transform, mesh and material
	auto renderView = m_Registry.view<Transform, Renderable>();
	renderView.each(RenderRenerable);

	Engine::Renderer::EndScene();
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
		
		auto camera = m_Registry.get<OrthographicCamera>(m_CameraEntity);
		float screenWidth = (float)Engine::Application::Get().GetWindow().GetWidth();
		float screenHeight = (float)Engine::Application::Get().GetWindow().GetHeight();
		glm::vec4 worldPos = camera.ScreenToWorld({ x, y }, screenWidth, screenHeight);

		ENG_TRACE("Mouse click at {0} {1}", worldPos.x, worldPos.y);

		m_Registry.view<QuadCollider, Tower, Renderable>().each([&worldPos](auto& collider, auto& tower, auto& renderable) {
			ENG_TRACE("Hit {0}", collider.IsInside({ worldPos.x, worldPos.y }));
			if (collider.IsInside({ worldPos.x, worldPos.y })) {
				tower.Selected = !tower.Selected;
				if (tower.Selected)
					renderable.MaterialID = tower.SelectedMaterial;
				else
					renderable.MaterialID = tower.UnselectedMaterial;
			}

		});
	}

	return true;
}


entt::entity TowerBattleLayer::CreateTower(glm::vec3 position)
{
	using namespace Engine::Components;

	auto tower = m_Registry.create();
	m_Registry.emplace<Transform>(tower,
		position,
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation
		glm::vec3(50.0f, 100.0f, 1.0f)  // scale
	);
	m_Registry.emplace<Renderable>(tower, "material/tower", "mesh/quad");
	m_Registry.emplace<Tower>(tower, false, "material/tower/selected", "material/tower");
	m_Registry.emplace<QuadCollider>(tower,
		glm::vec2{ position.x, position.y },
		50.0f, // width
		100.0f // height
	);

	return tower;
}

entt::entity TowerBattleLayer::CreateCamera()
{
	auto camera = m_Registry.create();
	m_Registry.emplace<Engine::Components::Transform>(camera,
		glm::vec3(0.0f, 0.0f, 0.0f), // position
		glm::vec3(0.0f, 0.0f, 0.0f), // rotation
		glm::vec3(1.0f, 1.0f, 1.0f)  // scale
	);
	m_Registry.emplace<Engine::Components::OrthographicCamera>(camera,
		(float)-1280 / 2, // left
		(float)1280 / 2, // right
		(float)-720 / 2, // bottom
		(float)720 / 2  // top
	);
	return camera;
}
