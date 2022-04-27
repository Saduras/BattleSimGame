#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
    class Material : public Asset
    {
    public:
        Material(const std::string& shaderID);
        Material(const Material& material);
        ~Material();

        Material& operator=(const Material&) { return *this; }

        inline const std::string& GetShaderID() const { return m_ShaderID; }
        inline Vec4 GetColor() const { return m_Color; }
        inline void SetColor(Vec4 color) { m_Color = color; }
    private:
        std::string m_ShaderID;
        Vec4 m_Color;
    };
}
