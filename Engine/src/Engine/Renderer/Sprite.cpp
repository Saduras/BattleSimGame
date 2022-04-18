#include "epch.h"
#include "Sprite.h"

#include <std_image.h>
#include <glad/glad.h>

namespace Engine
{
	Sprite::Sprite(std::string shaderPath, std::string spritePath)
		: m_RendererID(0), m_FilePath(spritePath), m_LocalBuffer(nullptr),
		m_Width(0), m_Height(0), m_BPP(0), m_Color(Vec4(1.0f, 1.0f, 1.0f, 1.0f))
	{
		m_Shader.reset(new Shader(shaderPath));

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

		m_Shader->SetUniform1i("u_Texture", 0);
	}

	Sprite::~Sprite()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Sprite::Bind(unsigned int slot /*= 0*/) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void Sprite::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
