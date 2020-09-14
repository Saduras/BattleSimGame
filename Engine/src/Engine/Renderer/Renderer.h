#pragma once

#include "RenderCommand.h"
#include "Engine/ECS/Components/Camera.h"
#include "Shader.h"

namespace Engine 
{
	class Renderer
	{
	public:
		static void BeginScene(Components::Camera& camera);
		static void EndScene();

		static void SetShader(const std::shared_ptr<Shader>& shader);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};

}