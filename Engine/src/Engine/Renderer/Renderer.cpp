#include "epch.h"
#include "Renderer.h"

namespace Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new SceneData;

	void Renderer::BeginScene(Mat4 projectionViewMatrix)
	{
		s_SceneData->ViewProjectionMatrix = projectionViewMatrix;
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::SetShader(Shader* shader)
	{
		shader->Bind();
		shader->SetProperty("u_PV", s_SceneData->ViewProjectionMatrix);
	}

	void Renderer::Submit(const VertexArray* vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}