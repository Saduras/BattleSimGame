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
		Sprite(const UUID& shaderUUID, const UUID& atlasUUID, const UUID& meshUUID);
		Sprite(const UUID& shaderUUID, const UUID& atlasUUID, const UUID& meshUUID, Vec4 color);

		inline const UUID& GetShaderUUID() const { return m_ShaderUUID; }
		inline const UUID& GetAtlasUUID() const { return m_AtlasUUID; }

		inline const UUID& GetMeshUUID() const { return m_MeshUUID; }
		inline void SetMeshUUID(UUID meshUUID) { m_MeshUUID = meshUUID; }

		inline Vec4 GetColor() const { return m_Color; }
		inline void SetColor(Vec4 color) { m_Color = color; }
	private:
		UUID m_ShaderUUID;
		UUID m_AtlasUUID;
		UUID m_MeshUUID;
		Vec4 m_Color;
	};
}