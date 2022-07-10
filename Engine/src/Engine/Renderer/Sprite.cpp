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

	Sprite::Sprite(const std::string& shaderID, const std::string& atlasID, int index /* = 0 */) 
		: Sprite(shaderID, atlasID, Vec4(1.0f, 1.0f, 1.0f, 1.0f), index)
	{ }

	Sprite::Sprite(const std::string& shaderID, const std::string& atlasID, Vec4 color, int index /* = 0 */)
		: m_ShaderID(shaderID), m_AtlasID(atlasID), m_Color(color), m_Index(index)
	{
		Shader& shader = AssetRegistry::Get<Shader>(shaderID);
		shader.SetProperty("u_Texture", 0);
	}
}
