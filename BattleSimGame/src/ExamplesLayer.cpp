#include "ExamplesLayer.h"

#include <Engine.h>

static void CreateQuadRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("QuadRenderExample");

	// Create resources
	Engine::UUID meshUUID = Engine::AssetRegistry::Add(new Engine::Mesh(Engine::PrimitiveMesh::Quad));
	Engine::UUID shaderUUID = Engine::AssetRegistry::Add(new Engine::Shader("res/shader/default.shader"));

	auto material = new Engine::Material(shaderUUID);
	material->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	Engine::UUID materialUUID = Engine::AssetRegistry::Add(material);

	// Setup camera
	auto camera = scene->CreateEntity();
	camera.AddComponent<Engine::Transform>(
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
	entity.AddComponent<Engine::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// position
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// rotation
		Engine::Vec3(50.0f, 100.0f, 1.0f)	// scale
	);
	entity.AddComponent<Engine::Components::Renderable3D>(materialUUID, meshUUID);

	// Setup systems
	scene->AddSystem<Engine::Systems::Render3DSystem>();
}

static std::vector<Engine::UUID> s_SpriteMeshUUIDs;

struct SpriteAnimation
{
	int CurrentIndex;
	float LastUpdate;
	float AnimationStepTime;
};

class AnimateSpriteSystem : public Engine::System
{
public:
	AnimateSpriteSystem(Engine::Scene* scene) : Engine::System(scene) {}

	void Execute(float deltaTime) override
	{
		auto view = m_Scene->GetView<Engine::Components::Renderable2D, SpriteAnimation>();
		view.each([this](Engine::Components::Renderable2D& renderable, SpriteAnimation& spriteAnimation) {

			float now = Engine::Time::GetTime();
			if (now - spriteAnimation.LastUpdate > spriteAnimation.AnimationStepTime)
			{
				Engine::Sprite& sprite = Engine::AssetRegistry::Get<Engine::Sprite>(renderable.Data[0].SpriteUUID);
				Engine::TextureAtlas& atlas = Engine::AssetRegistry::Get<Engine::TextureAtlas>(sprite.GetAtlasUUID());
				int newIndex = (spriteAnimation.CurrentIndex + 1) % atlas.GetSubTextureCount();
				sprite.SetMeshUUID(s_SpriteMeshUUIDs[newIndex]);

				spriteAnimation.LastUpdate = now;
				spriteAnimation.CurrentIndex = newIndex;
			}
		});
	}
	std::string GetName() const override { return "UpdateSpriteSystem"; }
};

static void CreateSpriteRenderExample(Engine::Scene* scene)
{
	ENG_TRACE("SpriteRenderExample");

	// Create resources
	Engine::UUID shaderUUID = Engine::AssetRegistry::Add(new Engine::Shader("res/shader/unlit_texture.shader"));

	Engine::TextureAtlas* atlas = new Engine::TextureAtlas("res/sprite/animation-test.png");
	s_SpriteMeshUUIDs = Engine::CreateMeshesForAtlas(*atlas);
	Engine::UUID atlasUUID = Engine::AssetRegistry::Add(atlas);
	auto sprite = new Engine::Sprite(shaderUUID, atlasUUID, s_SpriteMeshUUIDs[0]);
	sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	Engine::UUID spriteUUID = Engine::AssetRegistry::Add(sprite);

	// Setup camera
	auto camera = scene->CreateEntity();
	camera.AddComponent<Engine::Transform>(
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
	entity.AddComponent<Engine::Transform>(
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// position
		Engine::Vec3(0.0f, 0.0f, 0.0f),		// rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)	// scale
		);
	entity.AddComponent<Engine::Components::Renderable2D>(spriteUUID);
	entity.AddComponent<SpriteAnimation>(0, 0.0f, 0.5f);

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
	scene->AddSystem<AnimateSpriteSystem>();
}

static void CreatePixelShaderExample(Engine::Scene* scene)
{
	// Prepare assets
	Engine::UUID shaderUUID = Engine::AssetRegistry::Add(new Engine::Shader("res/shader/pixel_sprite.shader"));
	Engine::TextureAtlas* atlas = new Engine::TextureAtlas("res/sprite/medieval_sprite_pack.png");
	Engine::UUID atlasUUID = Engine::AssetRegistry::Add(atlas);
	std::vector<Engine::UUID> meshUUIDs = Engine::CreateMeshesForAtlas(*atlas);;
	Engine::Sprite* sprite = new Engine::Sprite(shaderUUID, atlasUUID, meshUUIDs[102]);
	Engine::UUID spriteUUID = Engine::AssetRegistry::Add(sprite);

	// Prepare camera
	auto camera = scene->CreateEntity();
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

	// Prepare pixel shaded sprite
	auto tower = scene->CreateEntity();
	tower.AddComponent<Engine::Transform>(
		Engine::Vec3(100.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)  // scale
		);
	tower.AddComponent<Engine::Components::Renderable2D>(spriteUUID);

	Engine::Shader& shader = Engine::AssetRegistry::Get<Engine::Shader>(shaderUUID);
	shader.SetProperty("u_TexelPerPixel", 5.0f); // Controls the mix between nearest neighbor and bilinear filtering

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
}

static void CreateDebugDrawingExample(Engine::Scene* scene)
{
	Engine::Debug::SetShader("res/shader/debug.shader");

	// Prepare camera
	auto camera = scene->CreateEntity();
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

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
}

static void UpdateDebugDrawingExample(float deltaTime)
{
	Engine::Debug::DrawLine(Engine::Vec2(0.0f, 0.0f), Engine::Vec2(50.0f, 50.0f), Engine::Vec3(1.0f, 0.0f, 0.0f), 10.0f);
}

ExampleLayer::ExampleLayer(ExampleId example)
{
	m_Scene = new Engine::Scene();
	m_ExampleId = example;
	switch (m_ExampleId) {
		case ExampleId::QuadRender:		CreateQuadRenderExample(m_Scene);	break;
		case ExampleId::SpriteRender:	CreateSpriteRenderExample(m_Scene);	break;
		case ExampleId::PixelShader:		CreatePixelShaderExample(m_Scene);	break;
		case ExampleId::DebugDrawings:	CreateDebugDrawingExample(m_Scene); break;
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

	if (m_ExampleId == ExampleId::DebugDrawings)
		UpdateDebugDrawingExample(deltaTime);
}