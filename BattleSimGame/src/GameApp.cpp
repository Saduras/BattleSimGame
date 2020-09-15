#include <Engine.h>

/// <summary>
/// This file shows an example on how to render a quad with the current API of my custom game engine.
/// Currently the engine supports Windows and used OpenGL for rendering.
/// The project is still in an early stage and actively worked on. The API is subject to change.
/// The latest version and the full code of the project is available here: 
///		https://github.com/Saduras/BattleSimGame
/// 
/// Layers work similar to layers in Photoshop and determin render order as well as input event handling.
/// </summary>
class ExampleLayer : public Engine::Layer
{
private:
	Engine::Components::Camera* m_Camera;
public:
	ExampleLayer()
		: Layer("Example")
	{
		// Each Layer maintains a pool of entities.
		// Each Entity has a set of components that hold the data on each entity
		// TODO: Add Systems to hold the logic that manipulates the data of componets
		Engine::Entity* entity = CreateEntity();
		// The Transform component holds data about position, rotation and scaling of an entity
		entity->AddComponent(new Engine::Components::Transform(
			{ 0.0f, 0.0f, 0.0f }, // position
			{ 0.0f, 0.0f, 0.0f }, // rotation (euler angles in degree)
			{ 100.0f, 100.0f, 1.0f }  // scale
		));
		// The Mesh component holds data such as verticies, vertex buffer layout and indicies for rendering
		entity->AddComponent(new Engine::Components::Mesh(Engine::PrimitiveMesh::Quad));
		// The Material component holds a reference to the shader to be used for rendering
		// TODO: Store data for uniforms associated with shader
		entity->AddComponent(new Engine::Components::Material("res/shader/default.shader"));

		// Here we create a orthographic camera
		Engine::Entity* cameraEntity = CreateEntity();
		// The Transform on the camera entity determins the view matrix
		cameraEntity->AddComponent(new Engine::Components::Transform(
			{ 0.0f, 0.0f, 0.0f }, // position
			{ 0.0f, 0.0f, 0.0f }, // rotation
			{ 1.0f, 1.0f, 1.0f }  // scale
		));
		m_Camera = new Engine::Components::OrthographicCamera(
			(float)-1280/2, // left
			(float) 1280/2, // right
			(float) -720/2, // bottom
			(float)  720/2  // top
		);
		cameraEntity->AddComponent(m_Camera);
	}

	/// <summary>
	/// This function is an example on how to submit all renderable enities to the renderer in this layer.
	/// This kind of logic will eventually be handled by a System
	/// </summary>
	void OnUpdate() override
	{
		Engine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Engine::RenderCommand::Clear();

		// Set scene data for render pass. Right now only which camera to use
		Engine::Renderer::BeginScene(*m_Camera);

		for (const auto& entity : m_Entities) {
			// Only process entities that have the required components for rendering
			// TODO: Cache renderable subset of entities
			if (entity->HasComponent<Engine::Components::Mesh*>(typeid(Engine::Components::Mesh*))
				&& entity->HasComponent<Engine::Components::Material*>(typeid(Engine::Components::Material*))
				&& entity->HasComponent<Engine::Components::Transform*>(typeid(Engine::Components::Transform*)))
			{
				// Submit the shader to be used to the Renderer
				// The submission automatically send the projection and view matrix to the shader since these are scene data
				auto material = entity->GetComponent<Engine::Components::Material*>(typeid(Engine::Components::Material*));
				auto shader = material->GetShader();
				Engine::Renderer::SetShader(shader);
				// Send model matrix to the shader
				// Renderer::SetShader must be called before since it binds the shader
				auto transform = entity->GetComponent<Engine::Components::Transform*>(typeid(Engine::Components::Transform*));
				auto modelMatrix = transform->GetTransformationMatrix();
				shader->SetUniformMat4f("u_Model", modelMatrix);
				// Lastly submit vertex array (holds vertex buffer, vertex buffer layout and index buffer)
				// This submission causes a draw call
				// TODO: Implement batching
				auto mesh = entity->GetComponent<Engine::Components::Mesh*>(typeid(Engine::Components::Mesh*));
				Engine::Renderer::Submit(mesh->GetVertexArray());
			}
		}

		Engine::Renderer::EndScene();
	}
};

/// <summary>
/// The client has to create it's own implementation of Engine::Application.
/// Engine::Application containts the window handle and the update loop.
/// </summary>
class BattleSimGame : public Engine::Application
{
public:
	BattleSimGame()
	{
		PushLayer(new ExampleLayer());
	}

	~BattleSimGame()
	{

	}
};

/// <summary>
/// By overriding Engine::CreateApplication the client defines which application implementation is used.
/// </summary>
/// <returns>This clients application implementation</returns>
Engine::Application* Engine::CreateApplication()
{
	return new BattleSimGame();
}