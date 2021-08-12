#pragma once

#include "Engine/Log.h"
#include "Engine/Scene.h"

#include <entt/entt.hpp>

namespace Engine {
	class Entity {
	public:
		Entity(entt::entity entityID, Scene* contextScene);

		/// <summary>
		/// Constructs a new component of the given type and arguments on the entity.
		/// </summary>
		/// <typeparam name="Component">Type of component to construct</typeparam>
		/// <param name="...args">Constructor arguments of the component</param>
		/// <returns>Reference to the newly constructed component</returns>
		template<typename Component, typename... Args>
		Component& AddComponent(Args&&... args) 
		{
			ENG_CORE_ASSERT(!HasComponent<Component>(), "Entity already has component of this type!");
			return m_ContextScene->m_Registry.emplace<Component>(m_EntityID, std::forward<Args>(args)...);
		}

		/// <summary>
		/// Tests whether this entity already has a component of this type.
		/// </summary>
		/// <typeparam name="Component">Component to test for.</typeparam>
		/// <returns>True if entity has this component already.</returns>
		template<typename Component>
		bool HasComponent() {
			return m_ContextScene->m_Registry.has<Component>(m_EntityID);
		}

	private:
		entt::entity m_EntityID;
		Scene* m_ContextScene;
	};
}