#include "epch.h"
#include "Renderer.h"

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::BeginScene(Camera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::SetShader(const std::shared_ptr<Shader>& shader)
	{
		ENG_CORE_ASSERT(s_Camera, "Renderer::BeginScene must be called be for a shader can be set!");

		shader->Bind();
		shader->SetUniformMat4f("u_PV", s_SceneData->ViewProjectionMatrix);
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}