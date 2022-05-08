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

		inline void SetProperty(const std::string& name, const Mat4& value) { m_PropertiesMat4[name] = value; }
		inline void SetProperty(const std::string& name, const Vec4& value) { m_PropertiesVec4[name] = value; }
		inline void SetProperty(const std::string& name, const Vec3& value) { m_PropertiesVec3[name] = value; }
		inline void SetProperty(const std::string& name, const Vec2& value) { m_PropertiesVec2[name] = value; }
		inline void SetProperty(const std::string& name, const float& value) { m_PropertiesFloat[name] = value; }
		inline void SetProperty(const std::string& name, const int& value) { m_PropertiesInt[name] = value; }

		void ApplyProperties();

		inline std::string GetFilePath() { return m_FilePath; }
	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		void SetUniformMat4f(const std::string& name, const Mat4& matrix);
		void SetUniform4f(const std::string& name, const Vec4 values);
		void SetUniform3f(const std::string& name, const Vec3 values);
		void SetUniform2f(const std::string& name, const Vec2 values);
		void SetUniform1f(const std::string& name, const float value);
		void SetUniform1i(const std::string& name, int value);
		int GetUniformLocation(const std::string& name);

		std::unordered_map<std::string, int> m_UniformLocationCache;
		uint32_t m_RendererID;
		std::string m_FilePath;

		std::unordered_map<std::string, Mat4> m_PropertiesMat4;
		std::unordered_map<std::string, Vec4> m_PropertiesVec4;
		std::unordered_map<std::string, Vec3> m_PropertiesVec3;
		std::unordered_map<std::string, Vec2> m_PropertiesVec2;
		std::unordered_map<std::string, float> m_PropertiesFloat;
		std::unordered_map<std::string, int> m_PropertiesInt;
	};
}