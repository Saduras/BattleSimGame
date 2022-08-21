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

	std::vector<UUID> CreateMeshesForAtlas(const TextureAtlas& atlas);
	MeshData GetMeshDataForSubTexture(const TextureAtlas& atlas, size_t index);

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

		/// <summary>
		/// Searches through all subtextures to find the one with the given name (slow).
		/// </summary>
		/// <param name="name">Name to search for.</param>
		/// <returns>Index of the subtexture with matching name. Returns -1 if no match was found.</returns>
		size_t FindSubTexIndex(std::string name);

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline Vec2 GetSize() const { return Vec2(m_Width, m_Height); }
		inline size_t GetSubTextureCount() const { return m_SubTextures.size(); }
		inline const SubTexture& GetSubTexture(size_t index) const { return m_SubTextures[index]; }
	private:
		std::string m_FilePath;
		std::vector<SubTexture> m_SubTextures;
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP; // BPP = bits per pixel
	};
}