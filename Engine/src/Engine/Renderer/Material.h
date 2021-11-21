#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
    class Material : public Asset
    {
    public:
        Material(std::string shaderPath);
        Material(const Material& material);
        ~Material();

        Material& operator=(const Material&) { return *this; }

        inline Shader* GetShader() const { return m_Shader.get(); }
        inline Vec4 GetColor() const { return m_Color; }
        inline void SetColor(Vec4 color) { m_Color = color; }
    private:
        std::unique_ptr<Shader> m_Shader;
        Vec4 m_Color;
    };
}
