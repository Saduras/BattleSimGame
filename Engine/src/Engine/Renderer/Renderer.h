#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Engine 
{
	class Renderer
	{
	public:
		static void BeginScene(const std::shared_ptr<Camera> camera);
		static void EndScene();

		static void SetShader(const std::shared_ptr<Shader>& shader);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static std::shared_ptr<Camera> s_Camera;
	};

}