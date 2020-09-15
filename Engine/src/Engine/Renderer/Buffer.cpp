#include "epch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Engine
{
	VertexBuffer* VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI()) 
		{
			case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(verticies, size);
			case RendererAPI::API::None:
					ENG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
					return nullptr;
		}

		ENG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indicies, uint32_t count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indicies, count);
			case RendererAPI::API::None:
					ENG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
					return nullptr;
		}

		ENG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}

