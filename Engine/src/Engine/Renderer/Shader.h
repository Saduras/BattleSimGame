#pragma once

#include <string>

#include "Engine/Assets/Asset.h"
#include "Engine/Math.h" 

namespace Engine
{
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader : public Asset
	{
	public:
		Shader(const std::string& filepath);
		Shader(const Shader& shader);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniformMat4f(const std::string& name, const Mat4& matrix);
		void SetUniform4f(const std::string& name, const Vec4 values);
		inline std::string GetFilePath() { return m_FilePath; }
		void SetUniform1i(const std::string& name, int value);
	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		int GetUniformLocation(const std::string& name);

		std::unordered_map<std::string, int> m_UniformLocationCache;
		uint32_t m_RendererID;
		std::string m_FilePath;
	};
}