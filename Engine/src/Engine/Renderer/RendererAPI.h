#pragma once


#include "Engine/Math.h"
#include "VertexArray.h"

namespace Engine
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1,
		};
	public:
		virtual void SetClearColor(const Vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const VertexArray* vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;

	};
}