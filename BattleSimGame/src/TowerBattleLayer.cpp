#include "TowerBattleLayer.h"


TowerBattleLayer::TowerBattleLayer()
	: Layer("TowerBattle")
{
	// Prepare assets
	auto mat = new Engine::Material("res/shader/default.shader");
	mat->SetColor({0.4f, 0.4f, 0.4f, 1.0f});
	Engine::AssetRegistry::Add("material/tower", mat);
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));

	m_CameraEntity = CreateCamera();

	CreateTower({ 100.0f, 0.0f, 0.0f });
	CreateTower({ 200.0f, 50.0f, 0.0f });
	CreateTower({ -50.0f, 75.0f, 0.0f });
}

TowerBattleLayer::~TowerBattleLayer()
{
}

void TowerBattleLayer::OnUpdate(float deltaTime)
{
	using namespace Engine::Components;

	Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Engine::RenderCommand::Clear();

	// For simplicity assume the first camera is the main camera
	auto proj = m_Registry.get<OrthographicCamera>(m_CameraEntity).GetProjectionMatrix();
	auto view = glm::inverse(m_Registry.get<Transform>(m_CameraEntity).GetTransformationMatrix());
	Engine::Renderer::BeginScene(proj * view);

	// Render all entities with transform, mesh and material
	auto renderView = m_Registry.view<Transform, Renderable>();
	renderView.each([](auto& transform, auto& renderable) {
		auto& material = Engine::AssetRegistry::GetMaterial(renderable.MaterialID);
		auto& mesh = Engine::AssetRegistry::GetMesh(renderable.MeshID);

		auto shader = material.GetShader();
		Engine::Renderer::SetShader(shader);
		auto modelMatrix = transform.GetTransformationMatrix();
		shader->SetUniformMat4f("u_Model", modelMatrix);
		shader->SetUniform4f("u_Color", material.GetColor());
		Engine::Renderer::Submit(mesh.GetVertexArray());
		});

	Engine::Renderer::EndScene();
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
