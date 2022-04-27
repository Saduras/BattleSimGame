#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

//#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Sprite.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace SpriteTests
{
	static int s_SystemExecuteCount = 0;

	TEST_CLASS(SpriteTests)
	{
	public:
		TEST_METHOD(SetTextureCoordinatesOnMeshDataTest)
		{
			TextureCoordinates texCoords { 0.2f, 0.3f, 0.4f, 0.5f };
			MeshData meshData = Mesh::PrimitiveToMeshData(PrimitiveMesh::TextureQuad);
			SetTextureCoordinatesOnMeshData(texCoords, meshData, 3, 5);

			// Bottom left
			Assert::AreEqual(meshData.Verticies[3], 0.2f);
			Assert::AreEqual(meshData.Verticies[4], 0.3f);
			// Bottom right
			Assert::AreEqual(meshData.Verticies[8], 0.4f);
			Assert::AreEqual(meshData.Verticies[9], 0.3f);
			// Top right
			Assert::AreEqual(meshData.Verticies[13], 0.4f);
			Assert::AreEqual(meshData.Verticies[14], 0.5f);
			// Top left
			Assert::AreEqual(meshData.Verticies[18], 0.2f);
			Assert::AreEqual(meshData.Verticies[19], 0.5f);
		}

	};
}
