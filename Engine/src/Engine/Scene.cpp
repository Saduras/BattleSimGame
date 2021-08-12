#include "epch.h"
#include "Scene.h"

#include "Engine/ECS/Entity.h"

namespace Engine
{
	Scene::Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		return { m_Registry.create(), this };
	}
}