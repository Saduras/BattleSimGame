#include "epch.h"
#include "Mesh.h"

namespace Engine
{
	Mesh::Mesh(MeshData data)
		: m_MeshData(data)
	{
		CreateVertexArray();
	}
	
	Mesh::Mesh(const Mesh& mesh)
		: m_MeshData(mesh.GetMeshData())
	{
		CreateVertexArray();
	}

	Mesh::Mesh(PrimitiveMesh primitive)
	{
		m_MeshData = PrimitiveToMeshData(primitive);
		CreateVertexArray();
	}
	
	Mesh::~Mesh()
	{
	}

	MeshData GetQuadMeshData()
	{
		std::vector<float> verticies = {
						-0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.5f,  0.5f, 0.0f,
						-0.5f,  0.5f, 0.0f,
		};
		BufferLayout layout = {
				{ Engine::ShaderDataType::Float3, "a_Position" },
		};
		std::vector<uint32_t> indicies = {
				0, 1, 2,
				2, 3, 0,
		};
		return { verticies, layout, indicies };
	}

	MeshData GetTriangleMeshData()
	{
		std::vector<float> verticies = {
						-0.5f, -0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 0.0f, 0.5f, 0.0f,
		};
		BufferLayout layout = {
				{ Engine::ShaderDataType::Float3, "a_Position" },
		};
		std::vector<uint32_t> indicies = {
				0, 1, 2,
		};
		return { verticies, layout, indicies };
	}

	MeshData GetTexturedQuadMeshData()
	{
		std::vector<float> verticies = {
						// position				texture coordinates
						-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
						 0.5f, -0.5f, 0.0f,		1.0f, 0.0f,
						 0.5f,  0.5f, 0.0f,		1.0f, 1.0f,
						-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,
		};
		BufferLayout layout = {
				{ Engine::ShaderDataType::Float3, "a_Position" },
				{ Engine::ShaderDataType::Float2, "a_TexCoord" },
		};
		std::vector<uint32_t> indicies = {
				0, 1, 2,
				2, 3, 0,
		};
		return { verticies, layout, indicies };
	}

	MeshData Mesh::PrimitiveToMeshData(PrimitiveMesh primitive)
	{
		switch (primitive) {
			case PrimitiveMesh::Quad:			return GetQuadMeshData();
			case PrimitiveMesh::Triangle:		return GetTriangleMeshData();
			case PrimitiveMesh::TextureQuad:	return GetTexturedQuadMeshData();
		}

		ENG_CORE_ASSERT(false, "Unsupported PrimitiveMesh type {0}!", primitive);
		return {};
	}

	void Mesh::CreateVertexArray()
	{
		m_VertexArray.reset(Engine::VertexArray::Create());

		std::shared_ptr<Engine::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Engine::VertexBuffer::Create((float*)m_MeshData.Verticies.data(), m_MeshData.Verticies.size() * sizeof(float)));
		vertexBuffer->SetLayout(m_MeshData.Layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		std::shared_ptr<Engine::IndexBuffer> indexBuffer;
		indexBuffer.reset(Engine::IndexBuffer::Create((uint32_t*)m_MeshData.Indicies.data(), m_MeshData.Indicies.size()));
		m_VertexArray->SetIndexBuffer(indexBuffer);
	}
}