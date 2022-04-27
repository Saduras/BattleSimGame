#include "ExamplesLayer.h"

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/Sprite.h>

static void CreateQuadRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("QuadRenderExample");

	// Create resources
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));
	Engine::AssetRegistry::Add("shader/default", new Engine::Shader("res/shader/default.shader"));

	auto material = new Engine::Material("shader/default");
	material->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::AssetRegistry::Add("mat/default", material);

	// Setup camera
	auto camera = scene->CreateEntity();
	camera.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(1.0f, 1.0f, 1.0f)  // scale
		);
	camera.AddComponent<Engine::Components::OrthographicCamera>(
		(float)-1280 / 2,	// left
		(float)1280 / 2,	// right
		(float)-720 / 2,	// bottom
		(float)720 / 2		// top
		);

	// Setup sprite
	auto entity = scene->CreateEntity();
	entity.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// position
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// rotation
		Engine::Vec3(50.0f, 100.0f, 1.0f)	// scale
	);
	entity.AddComponent<Engine::Components::Renderable3D>("mat/default", "mesh/quad");

	// Setup systems
	scene->AddSystem<Engine::Systems::Render3DSystem>();
}

static void CreateSpriteRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("SpriteRenderExample");

	// Create resources
	Engine::AssetRegistry::Add("shader/unlit_texture", new Engine::Shader("res/shader/unlit_texture.shader"));

	auto sprite = new Engine::Sprite("shader/unlit_texture", "res/sprite/alpha-test.png");
	sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	sprite->SetTextureCoordinates({ { 0.0f, 0.0f, 0.5f, 0.5f } });
	Engine::AssetRegistry::Add("sprite/pop", sprite);


	Engine::TextureCoordinates texCoords = sprite->GetTextureCoordinates(0);
	Engine::MeshData meshData = Engine::Mesh::PrimitiveToMeshData(Engine::PrimitiveMesh::TextureQuad);
	Engine::SetTextureCoordinatesOnMeshData(texCoords, meshData, 3, 5);
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(meshData));

	// Setup camera
	auto camera = scene->CreateEntity();
	camera.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(1.0f, 1.0f, 1.0f)  // scale
		);
	camera.AddComponent<Engine::Components::OrthographicCamera>(
		(float)-1280 / 2,	// left
		(float)1280 / 2,	// right
		(float)-720 / 2,	// bottom
		(float)720 / 2		// top
		);

	// Setup sprite
	auto entity = scene->CreateEntity();
	entity.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// position
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)	// scale
		);
	entity.AddComponent<Engine::Components::Renderable2D>("sprite/pop", "mesh/quad");

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
	// TODO: Create RenderSystem2D
	// - support switching sprite from atlas
}

ExampleLayer::ExampleLayer(ExampleScene example)
{
	switch (example) {
		case ExampleScene::QuadRender:
			m_Scene = new Engine::Scene();
			CreateQuadRenderExample(m_Scene);
			break;
		case ExampleScene::SpriteRender:
			m_Scene = new Engine::Scene();
			CreateSpriteRenderExample(m_Scene);
			break;
		default:
			ENG_FATAL("Unknown example value!");
	}
}

ExampleLayer::~ExampleLayer()
{
	if (m_Scene != nullptr)
		delete m_Scene;
}

void ExampleLayer::OnUpdate(float deltaTime)
{
	if (m_Scene != nullptr)
		m_Scene->Update(deltaTime);
}