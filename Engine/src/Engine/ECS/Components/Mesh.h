#pragma once

#include "Engine/ECS/Component.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	enum class PrimitiveMesh
	{
		None = 0,
		Quad,
		Triangle,
	};
}

namespace Engine::Components
{
	struct MeshData
	{
		std::vector<float> Verticies;
		BufferLayout Layout;
		std::vector<uint32_t> Indicies;
	};

	class Mesh : public Component
	{
	public:
		Mesh();
		Mesh(MeshData data);
		Mesh(const Mesh& mesh);
		Mesh(PrimitiveMesh primitive);
		~Mesh();

		Mesh& operator=(const Mesh&) { return *this; }

		inline VertexArray* GetVertexArray() const { return m_VertexArray.get(); }
		inline MeshData GetMeshData() const { return m_MeshData; }

		static MeshData PrimitiveToMeshData(PrimitiveMesh primitive);
	private:
		MeshData m_MeshData;
		std::unique_ptr<VertexArray> m_VertexArray;

		void CreateVertexArray();
	};
}
