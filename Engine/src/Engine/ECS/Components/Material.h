#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/Renderer/Shader.h"

namespace Engine::Components
{
    class Material : public Component
    {
    public:
        Material(std::string shaderPath);
        Material(const Material& material);
        ~Material();

        Material& operator=(const Material&) { return *this; }

        inline Shader* GetShader() const { return m_Shader.get(); }
    private:
        std::unique_ptr<Shader> m_Shader;
    };
}
