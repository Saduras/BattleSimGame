#pragma once

#include "Engine/Assets/Asset.h"
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

	class Sprite : public Asset
	{
	public:
		Sprite(const std::string& shaderID, const std::string& spritePath);
		~Sprite();

		Sprite& operator=(const Sprite&) { return *this; }

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline const std::string& GetShaderID() const { return m_ShaderID; }
		inline Vec4 GetColor() const { return m_Color; }
		inline void SetColor(Vec4 color) { m_Color = color; }
		inline TextureCoordinates GetTextureCoordinates(int index) { return m_textureCoordinates[index]; }
		inline void SetTextureCoordinates(std::vector<TextureCoordinates> textureCoordinates) { m_textureCoordinates = textureCoordinates; }
	private:
		std::string m_ShaderID;
		std::string m_FilePath;
		std::vector<TextureCoordinates> m_textureCoordinates;
		unsigned int m_RendererID;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP; // BPP = bits per pixel
		Vec4 m_Color;
	};
}