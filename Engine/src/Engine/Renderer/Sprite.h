#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	struct TextureCoordinates
	{
		float StartX;
		float StartY;
		float EndX;
		float EndY;
	};

	std::vector<TextureCoordinates> TexCoordsFromXML(const std::string& xmlPath, Vec2 textureSize);

	void SetTextureCoordinatesOnMeshData(const TextureCoordinates& texCoords, MeshData& meshData, int texCoordIndex, int vertexLength);

	class Sprite : public Asset
	{
	public:
		Sprite(const std::string& shaderID, const std::string& atlasID, int index = 0);

		inline const std::string& GetShaderID() const { return m_ShaderID; }
		inline const std::string& GetAtlasID() const { return m_AtlasID; }
		inline int GetIndex() { return m_Index; }
		inline void SetIndex(int index) { m_Index = index; }
		inline Vec4 GetColor() const { return m_Color; }
		inline void SetColor(Vec4 color) { m_Color = color; }
	private:
		std::string m_ShaderID;
		std::string m_AtlasID;
		int m_Index;
		Vec4 m_Color;
	};
}