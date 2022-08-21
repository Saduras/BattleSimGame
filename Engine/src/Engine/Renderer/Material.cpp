#include "epch.h"
#include "Material.h"

namespace Engine
{
	Material::Material(const UUID& shaderUUID)
		: m_Color(Vec4(1.0f, 0.0f, 1.0f, 1.0f)), m_ShaderUUID(shaderUUID)
	{
	}

	Material::Material(const Material& material)
		: m_Color(material.m_Color), m_ShaderUUID(material.m_ShaderUUID)
	{
	}
	
	Material::~Material()
	{
	}
}