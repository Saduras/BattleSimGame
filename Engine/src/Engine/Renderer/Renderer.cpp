#include "epch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "Engine/ECS/Components/Transform.h"

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::BeginScene(Components::Camera& camera)
	{
		auto transform = camera.GetEntity()->GetComponent<Components::Transform*>(typeid(Components::Transform*));
		auto viewMat = glm::inverse(transform->GetTransformationMatrix());

		s_SceneData->ViewProjectionMatrix = camera.GetProjectionMatrix() * viewMat;
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::SetShader(const std::shared_ptr<Shader>& shader)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_PV", s_SceneData->ViewProjectionMatrix);
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}