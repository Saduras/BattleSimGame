#include "epch.h"
#include "Renderer.h"

namespace Engine
{
	std::shared_ptr<Camera> Renderer::s_Camera = nullptr;

	void Renderer::BeginScene(std::shared_ptr<Camera> camera)
	{
		s_Camera = camera;
	}
	
	void Renderer::EndScene()
	{
	}
	
	void Renderer::SetShader(const std::shared_ptr<Shader>& shader)
	{
		ENG_CORE_ASSERT(s_Camera, "Renderer::BeginScene must be called be for a shader can be set!");

		shader->Bind();
		shader->SetUniformMat4f("u_PV", s_Camera->GetPV());
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}