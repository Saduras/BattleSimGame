#include "ExamplesLayer.h"

#include <Engine/Renderer/Material.h>
#include <Engine/Renderer/Mesh.h>
#include <Engine/Renderer/Sprite.h>

static void CreateQuadRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("QuadRenderExample");

	// Create resources
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));

	auto material = new Engine::Material("res/shader/default.shader");
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
	scene->AddSystem<Engine::Systems::RenderSystem>();
}

static void CreateSpriteRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("SpriteRenderExample");

	// Create resources
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(Engine::PrimitiveMesh::Quad));

	auto sprite = new Engine::Sprite("res/shader/unlit_texture.shader", 
									"res/texture/pop-me.png");
	sprite->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::AssetRegistry::Add("sprite/pop", sprite);

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
	entity.AddComponent<Engine::Components::Renderable2D>("sprite/pop", "mesh/quad");

	// Setup systems
	//scene->AddSystem<Engine::Systems::RenderSystem>();
	// TODO: Create RenderSystem2D
	// - add alpha rendnering
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