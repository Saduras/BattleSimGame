#include "epch.h"
#include "Material.h"

namespace Engine::Components
{
	Material::Material(std::string shaderPath)
	{
		m_Shader.reset(new Shader(shaderPath));
	}

	Material::Material(const Material& material)
	{
		m_Shader.reset(new Shader(material.m_Shader->GetFilePath()));
	}
	
	Material::~Material()
	{
	}
}