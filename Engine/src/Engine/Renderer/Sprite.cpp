#include "epch.h"
#include "Sprite.h"

namespace Engine
{
	Sprite::Sprite(std::string shaderPath, std::string spritePath)
		: m_Color(Vec4(1.0f, 1.0f, 1.0f, 1.0f))
	{
		m_Shader.reset(new Shader(shaderPath));
	}

	Sprite::~Sprite()
	{
	}
}
