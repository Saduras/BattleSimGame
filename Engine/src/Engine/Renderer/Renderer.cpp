#include "epch.h"
#include "Renderer.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "Engine/ECS/Components/Transform.h"

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::BeginScene(glm::mat4 projectionViewMatrix)
	{
		s_SceneData->ViewProjectionMatrix = projectionViewMatrix;
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::SetShader(Shader* shader)
	{
		shader->Bind();
		shader->SetUniformMat4f("u_PV", s_SceneData->ViewProjectionMatrix);
	}

	void Renderer::Submit(const VertexArray* vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}