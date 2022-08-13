#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Buffer.h"
#include "Engine/Renderer/VertexArray.h"

namespace Engine
{
	enum class PrimitiveMesh
	{
		None = 0,
		Quad,
		Triangle,
		TextureQuad,
	};

	struct MeshData
	{
		std::vector<float> Verticies;
		BufferLayout Layout;
		std::vector<uint32_t> Indicies;
	};

	class Mesh : public Asset
	{
	public:
		Mesh(MeshData data);
		Mesh(const Mesh& mesh);
		Mesh(PrimitiveMesh primitive);

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
