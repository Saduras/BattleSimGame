#include "epch.h"

#include "TextureAtlas.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Assets/AssetRegistry.h"

#include <std_image.h>
#include <glad/glad.h>
#include <pugixml.hpp>

namespace Engine
{
	std::vector<UUID> CreateMeshesForAtlas(const TextureAtlas& atlas)
	{
		std::vector<UUID> uuids(atlas.GetSubTextureCount());
		for (size_t i = 0; i < atlas.GetSubTextureCount(); i++)
			uuids[i] = AssetRegistry::Add(new Mesh(GetMeshDataForSubTexture(atlas, i)));
		return uuids;
	}

	MeshData GetMeshDataForSubTexture(const TextureAtlas& atlas, size_t index)
	{
		SubTexture subTexture = atlas.GetSubTexture(index);
		Vec2 size = atlas.GetSize();

		MeshData meshData = Mesh::PrimitiveToMeshData(PrimitiveMesh::TextureQuad);
		const int texCoordIndex = 3;
		const int vertexLength = 5;

		float startX = subTexture.X / size[0];
		float startY = (subTexture.Y - subTexture.Height) / size[1];
		float endX = (subTexture.X + subTexture.Width) / size[0];
		float endY = subTexture.Y / size[1];

		// Bottom left
		meshData.Verticies[texCoordIndex] = startX;
		meshData.Verticies[texCoordIndex + 1] = startY;
		// Bottom right
		meshData.Verticies[texCoordIndex + vertexLength] = endX;
		meshData.Verticies[texCoordIndex + vertexLength + 1] = startY;
		// Top right
		meshData.Verticies[texCoordIndex + 2 * vertexLength] = endX;
		meshData.Verticies[texCoordIndex + 2 * vertexLength + 1] = endY;
		// Top left
		meshData.Verticies[texCoordIndex + 3 * vertexLength] = startX;
		meshData.Verticies[texCoordIndex + 3 * vertexLength + 1] = endY;

		return meshData;
	}


	TextureAtlas::TextureAtlas(const std::string& filePath) 
		: m_RendererID(0), m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0),
		m_BPP(0)
	{
		// Load texture
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);

		// Load subtexture coordinates
		size_t fileEndingStart = filePath.find_last_of('.');
		std::string xmlPath = filePath;
		xmlPath.replace(fileEndingStart, filePath.length(), ".xml");

		using namespace pugi;

		xml_document doc;
		xml_parse_result result = doc.load_file(xmlPath.c_str());

		if (!result)
			ENG_CORE_ERROR(result.description());

		for (xml_node node : doc.child("TextureAtlas").children("sprite")) \
		{
			std::string name = node.attribute("n").as_string();
			int x = node.attribute("x").as_int();
			int y = node.attribute("y").as_int();
			int width = node.attribute("w").as_int();
			int height = node.attribute("h").as_int();

			// XML describes tex coordinates relative to top left.
			// Renderer wants tex coordinates relative to bottom left.
			// Hence we have to flip min y and max y and subtract them from 1.
			m_SubTextures.push_back({
				name,
				x,
				m_Height - y,
				width,
				height,
			});
		}
	}

	TextureAtlas::~TextureAtlas()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	size_t TextureAtlas::FindSubTexIndex(std::string name)
	{
		for (size_t i = 0; i < m_SubTextures.size(); i++)
			if (m_SubTextures[i].Name == name)
				return i;

		ENG_CORE_ASSERT_FMT(false, "Unable to find sub texture {}!", name)
		return -1;
	}

	void TextureAtlas::Bind(unsigned int slot /* = 0*/) const
	{
		// TODO: What would be a good place to put blend function?
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void TextureAtlas::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}