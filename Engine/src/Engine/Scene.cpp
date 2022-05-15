#include "epch.h"
#include "Scene.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"

namespace Engine
{
	Scene::Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		return { m_Registry.create(), this };
	}

	void Scene::DestroyEntity(const Entity& entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Update(float deltaTime)
	{
		for (auto system : m_Systems)
			system->Execute(deltaTime);
	}

	std::vector<std::string> Scene::GetSystemNames() const
	{
		std::vector<std::string> names;
		std::transform(m_Systems.begin(), m_Systems.end(), std::back_inserter(names),
			[](const System* system) { return system->GetName(); });
		return names;;
	}

	size_t Scene::GetEntityCount() const
	{
		return m_Registry.alive();
	}

}