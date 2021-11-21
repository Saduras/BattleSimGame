#pragma once

#include "RenderCommand.h"
#include "Shader.h"

namespace Engine 
{
	class Renderer
	{
	public:
		static void BeginScene(Mat4 projectionViewMatrix);
		static void EndScene();

		static void SetShader(Shader* shader);
		static void Submit(const VertexArray* vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			Mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}