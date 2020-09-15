#include "epch.h"
#include "Material.h"

namespace Engine::Components
{
	Material::Material(std::string shaderPath)
	{
		m_Shader.reset(new Shader(shaderPath));
	}
	
	Material::~Material()
	{
	}
}