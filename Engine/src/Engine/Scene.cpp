#include "epch.h"
#include "Scene.h"

namespace Engine
{
	Scene::Scene()
	{
	}

	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}