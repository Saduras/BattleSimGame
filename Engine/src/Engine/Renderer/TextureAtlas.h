#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Math.h"

#include <string>
#include <vector>

namespace Engine
{
	// Forward declarations
	struct MeshData;
	class TextureAtlas;

	struct SubTexture
	{
		std::string Name;
		int X;
		int Y;
		int Width;
		int Height;
	};

	MeshData GetMeshDataForSubTexture(const TextureAtlas& atlas, int index);

	class TextureAtlas : public Asset
	{
	public:
		/// <summary>
		/// Create TextureAtlas from file. Path points to texture file. Expects .xml file describing subdivision to be at same location and same name.
		/// </summary>
		/// <param name="filePath"></param>
		TextureAtlas(const std::string& filePath);
		~TextureAtlas();
		TextureAtlas(const TextureAtlas& other) = delete;
		TextureAtlas(TextureAtlas&& other) = delete;

		TextureAtlas& operator=(const TextureAtlas& other) = delete;
		TextureAtlas& operator=(TextureAtlas&& other) = delete;

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline Vec2 GetSize() const { return Vec2(m_Width, m_Height); }
		inline size_t GetSubTextureCount() const { return m_SubTextures.size(); }
		inline const SubTexture& GetSubTexture(int index) const { return m_SubTextures[index]; }
	private:
		std::string m_FilePath;
		std::vector<SubTexture> m_SubTextures;
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP; // BPP = bits per pixel
	};
}