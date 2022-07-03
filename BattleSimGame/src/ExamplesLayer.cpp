#include "ExamplesLayer.h"

#include <Engine.h>

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
				Engine::Sprite& sprite = Engine::AssetRegistry::Get<Engine::Sprite>(renderable.Data[0].SpriteID);
				Engine::TextureAtlas& atlas = Engine::AssetRegistry::Get<Engine::TextureAtlas>(sprite.GetAtlasID());
				int newIndex = (spriteAnimation.CurrentIndex + 1) % atlas.GetSubTextureCount();
				sprite.SetIndex(newIndex);

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
	Engine::AssetRegistry::Add("shader/unlit_texture", new Engine::Shader("res/shader/unlit_texture.shader"));

	Engine::TextureAtlas* atlas = new Engine::TextureAtlas("res/sprite/animation-test.png");
	Engine::AssetRegistry::Add("atlas", atlas);
	auto sprite = new Engine::Sprite("shader/unlit_texture", "atlas");
	sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	Engine::AssetRegistry::Add("sprite", sprite);

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
	entity.AddComponent<Engine::Components::Renderable2D>("sprite");
	entity.AddComponent<SpriteAnimation>(0, 0.0f, 0.5f);

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
	scene->AddSystem<AnimateSpriteSystem>();
}

static void CreatePixelShaderExample(Engine::Scene* scene)
{
	// Prepare assets
	Engine::AssetRegistry::Add("shader/sprite", new Engine::Shader("res/shader/pixel_sprite.shader"));
	Engine::AssetRegistry::Add("atlas", new Engine::TextureAtlas("res/sprite/medieval_sprite_pack.png"));
	Engine::Sprite* sprite = new Engine::Sprite("shader/sprite", "atlas");
	sprite->SetIndex(102);
	Engine::AssetRegistry::Add("sprite", sprite);

	// Prepare camera
	auto camera = scene->CreateEntity();
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

	// Prepare pixel shaded sprite
	auto tower = scene->CreateEntity();
	tower.AddComponent<Engine::Components::Transform>(
		Engine::Vec3(100.0f, 0.0f, 0.0f), // position
		Engine::Vec3(0.0f, 0.0f, 0.0f), // rotation
		Engine::Vec3(100.0f, 100.0f, 1.0f)  // scale
		);
	tower.AddComponent<Engine::Components::Renderable2D>("sprite");

	Engine::Shader& shader = Engine::AssetRegistry::Get<Engine::Shader>("shader/sprite");
	shader.SetProperty("u_TexelPerPixel", 5.0f); // Controls the mix between nearest neighbor and bilinear filtering

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
}

ExampleLayer::ExampleLayer(ExampleScene example)
{
	m_Scene = new Engine::Scene();
	switch (example) {
		case ExampleScene::QuadRender:		CreateQuadRenderExample(m_Scene);	break;
		case ExampleScene::SpriteRender:	CreateSpriteRenderExample(m_Scene);	break;
		case ExampleScene::PixelShader:		CreatePixelShaderExample(m_Scene);	break;
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