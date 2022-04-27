#include "epch.h"
#include "Material.h"

namespace Engine
{
	Material::Material(const std::string& shaderID)
		: m_Color(Vec4(1.0f, 0.0f, 1.0f, 1.0f)), m_ShaderID(shaderID)
	{
	}

	Material::Material(const Material& material)
		: m_Color(material.m_Color), m_ShaderID(material.m_ShaderID)
	{
	}
	
	Material::~Material()
	{
	}
}