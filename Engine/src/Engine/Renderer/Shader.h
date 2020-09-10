#pragma once

#include <string>
#include <glm\glm.hpp>

namespace Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		int GetUniformLocation(const std::string& name);

		std::unordered_map<std::string, int> m_UniformLocationCache;
		uint32_t m_RendererID;
	};
}