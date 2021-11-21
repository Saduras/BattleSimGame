#pragma once

#include <Engine.h>

enum class ExampleScene
{
	QuadRender,
	SpriteRender,
};

class ExampleLayer : public Engine::Layer
{
public:
	ExampleLayer(ExampleScene example);
	~ExampleLayer();

	void OnUpdate(float deltaTime) override;
private:
	Engine::Scene* m_Scene = nullptr;
};