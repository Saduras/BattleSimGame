#include "epch.h"
#include "Material.h"

namespace Engine
{
	Material::Material(std::string shaderPath)
		: m_Color(Vec4(1.0f, 0.0f, 1.0f, 1.0f))
	{
		m_Shader.reset(new Shader(shaderPath));
	}

	Material::Material(const Material& material)
		: m_Color(material.m_Color)
	{
		m_Shader.reset(new Shader(material.m_Shader->GetFilePath()));
	}
	
	Material::~Material()
	{
	}
}