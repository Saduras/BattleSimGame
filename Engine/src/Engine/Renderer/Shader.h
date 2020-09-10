#pragma once

#include <string>
#include <glm\glm.hpp>

namespace Engine
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	public:
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		int GetUniformLocation(const std::string& name);

		std::unordered_map<std::string, int> m_UniformLocationCache;
		uint32_t m_RendererID;
		std::string m_FilePath;
	};
}