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
				Engine::Sprite& sprite = Engine::AssetRegistry::Get<Engine::Sprite>(renderable.SpriteID);
				int newIndex = (spriteAnimation.CurrentIndex + 1) % sprite.GetTileCount();
				renderable.MeshID.replace((int)renderable.MeshID.length() - 1, 1, std::to_string(newIndex));

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

	auto sprite = new Engine::Sprite("shader/unlit_texture", "res/sprite/animation-test.png");
	sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	sprite->SetTextureCoordinates({ 
			{ 0.0f, 0.5f, 0.5f, 1.0f },
			{ 0.5f, 0.5f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 0.5f, 0.5f },
			{ 0.5f, 0.0f, 1.0f, 0.5f },
		});
	Engine::AssetRegistry::Add("sprite/pop", sprite);



	for (size_t i = 0; i < sprite->GetTileCount(); i++)
	{
		Engine::TextureCoordinates texCoords = sprite->GetTextureCoordinates((int)i);
		Engine::MeshData meshData = Engine::Mesh::PrimitiveToMeshData(Engine::PrimitiveMesh::TextureQuad);
		Engine::SetTextureCoordinatesOnMeshData(texCoords, meshData, 3, 5);
		Engine::AssetRegistry::Add(Engine::FormatString("mesh/quad/{}", i), new Engine::Mesh(meshData));
	}

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
	entity.AddComponent<Engine::Components::Renderable2D>("sprite/pop", "mesh/quad/0");
	entity.AddComponent<SpriteAnimation>(0, 0.0f, 0.5f);

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
	scene->AddSystem<AnimateSpriteSystem>();
}

static void CreatePixelShaderExample(Engine::Scene* scene)
{
	// Prepare assets
	Engine::AssetRegistry::Add("shader/sprite", new Engine::Shader("res/shader/pixel_sprite.shader"));

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

	Engine::Sprite* towerSprite = new Engine::Sprite("shader/sprite", "res/sprite/Tower_Blue.png");
	towerSprite->SetTextureCoordinates(texCoords);
	Engine::AssetRegistry::Add("sprite/tower", towerSprite);

	Engine::MeshData meshData = Engine::Mesh::PrimitiveToMeshData(Engine::PrimitiveMesh::TextureQuad);
	Engine::SetTextureCoordinatesOnMeshData(towerSprite->GetTextureCoordinates(0), meshData, 3, 5);
	Engine::AssetRegistry::Add("mesh/quad", new Engine::Mesh(meshData));

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
	tower.AddComponent<Engine::Components::Renderable2D>("sprite/tower", "mesh/quad");

	Engine::Shader& shader = Engine::AssetRegistry::Get<Engine::Shader>("shader/sprite");
	shader.SetProperty("u_TexelPerPixel", 5.0f); // Controls the mix between nearest neighbor and bilinear filtering

	// Setup systems
	scene->AddSystem<Engine::Systems::Render2DSystem>();
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
		case ExampleScene::PixelShader:
			m_Scene = new Engine::Scene();
			CreatePixelShaderExample(m_Scene);
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