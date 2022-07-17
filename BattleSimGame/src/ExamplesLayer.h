#pragma once

#include <Engine.h>

enum class ExampleId
{
	QuadRender,
	SpriteRender,
	PixelShader,
	DebugDrawings,
};

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer(ExampleId example);
	~ExampleLayer();

	void OnUpdate(float deltaTime) override;
private:
	Engine::Scene* m_Scene = nullptr;
	ExampleId m_ExampleId;
};