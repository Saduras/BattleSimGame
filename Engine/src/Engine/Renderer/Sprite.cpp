#include "epch.h"

#include "Sprite.h"
#include "Engine/Assets/AssetRegistry.h"

#include <std_image.h>
#include <glad/glad.h>
#include <pugixml.hpp>

namespace Engine
{
	std::vector<TextureCoordinates> TexCoordsFromXML(const std::string& xmlPath, Vec2 textureSize)
	{
		using namespace pugi;

		xml_document doc;
		xml_parse_result result = doc.load_file(xmlPath.c_str());
		
		if(!result)
			ENG_CORE_ERROR(result.description());

		std::vector<TextureCoordinates> texCoords;
		for (xml_node node : doc.child("TextureAtlas").children("SubTexture")) \
		{
			int x = node.attribute("x").as_int();
			int y = node.attribute("y").as_int();
			int width = node.attribute("width").as_int();
			int height = node.attribute("height").as_int();

			// XML describes tex coordinates relative to top left.
			// Renderer wants tex coordinates relative to bottom left.
			// Hence we have to flip min y and max y and subtract them from 1.
			texCoords.push_back({
				x / textureSize[0],
				1 - (y + height) / textureSize[1],
				(x + width) / textureSize[0],
				1 - y / textureSize[1],
			});
		}

		return texCoords;
	}

	void SetTextureCoordinatesOnMeshData(const TextureCoordinates& texCoords, MeshData& meshData, int texCoordIndex, int vertexLength)
	{
		// Bottom left
		meshData.Verticies[texCoordIndex] = texCoords.StartX;
		meshData.Verticies[texCoordIndex + 1] = texCoords.StartY;
		// Bottom right
		meshData.Verticies[texCoordIndex + vertexLength] = texCoords.EndX;
		meshData.Verticies[texCoordIndex + vertexLength + 1] = texCoords.StartY;
		// Top right
		meshData.Verticies[texCoordIndex + 2 * vertexLength] = texCoords.EndX;
		meshData.Verticies[texCoordIndex + 2 * vertexLength + 1] = texCoords.EndY;
		// Top left
		meshData.Verticies[texCoordIndex + 3 * vertexLength] = texCoords.StartX;
		meshData.Verticies[texCoordIndex + 3 * vertexLength + 1] = texCoords.EndY;
	}

	Sprite::Sprite(const std::string& shaderID, const std::string& spritePath)
		: m_RendererID(0), m_FilePath(spritePath), m_LocalBuffer(nullptr), m_ShaderID(shaderID),
		m_Width(0), m_Height(0), m_BPP(0), m_Color(Vec4(1.0f, 1.0f, 1.0f, 1.0f)), 
		m_textureCoordinates{ { 0.0f, 0.0f, 1.0f, 1.0f } }
	{
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(spritePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

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

		Shader& shader = AssetRegistry::Get<Shader>(shaderID);
		shader.SetProperty("u_Texture", 0);
	}

	Sprite::~Sprite()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Sprite::Bind(unsigned int slot /*= 0*/) const
	{
		// TODO: What would be a good place to put blend function?
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Sprite::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
