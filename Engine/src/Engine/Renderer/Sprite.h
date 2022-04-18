#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
	class Sprite : public Asset
	{
	public:
		Sprite(std::string shaderPath, std::string spritePath);
		~Sprite();

		Sprite& operator=(const Sprite&) { return *this; }

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		inline Shader* GetShader() const { return m_Shader.get(); }
		inline Vec4 GetColor() const { return m_Color; }
		inline void SetColor(Vec4 color) { m_Color = color; }
	private:
		std::unique_ptr<Shader> m_Shader;
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP; // BPP = bits per pixel
		Vec4 m_Color;
	};
}