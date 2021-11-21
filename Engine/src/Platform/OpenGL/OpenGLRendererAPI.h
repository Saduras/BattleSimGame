#pragma once

#include "Engine/Renderer/RendererAPI.h"

namespace Engine
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const Vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const VertexArray* vertexArray) override;
	};
}