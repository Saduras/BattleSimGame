#pragma once

#include "Engine/Assets/Asset.h"
#include "Engine/Renderer/Shader.h"

namespace Engine
{
    class Material : public Asset
    {
    public:
        Material(const UUID& shaderUUID);
        Material(const Material& material);
        ~Material();

        Material& operator=(const Material&) { return *this; }

        inline const UUID& GetShaderUUID() const { return m_ShaderUUID; }
        inline Vec4 GetColor() const { return m_Color; }
        inline void SetColor(Vec4 color) { m_Color = color; }
    private:
        UUID m_ShaderUUID;
        Vec4 m_Color;
    };
}
