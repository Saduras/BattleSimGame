#pragma once

#include "Engine/Log.h"
#include "Engine/Scene.h"

#include <entt/entt.hpp>

namespace Engine {
	class Entity {
	public:
		Entity() {}
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
		/// Removes a component from the entity.
		/// </summary>
		/// <typeparam name="Component">Type of the component to remove</typeparam>
		template<typename Component>
		void RemoveComponent() {
			ENG_CORE_ASSERT(HasComponent<Component>(), "Entity doesn't have a component of this type!");
			m_ContextScene->m_Registry.remove<Component>(m_EntityID);
		}

		/// <summary>
		/// Get a reference to a component 
		/// </summary>
		/// <typeparam name="Component">Type of the componet to get</typeparam>
		/// <returns>Reference to the component of this entity</returns>
		template<typename Component>
		Component& GetComponent() {
			ENG_CORE_ASSERT(HasComponent<Component>(), "Entity doesn't have a component of this type!");
			return m_ContextScene->m_Registry.get<Component>(m_EntityID);
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
		
		/// <summary>
		/// Checks if this entity is valid.
		/// </summary>
		/// <returns>True if the entity is invalid.</returns>
		inline bool IsNull() { return m_EntityID == entt::null || m_ContextScene == nullptr; }

		friend inline bool operator==(const Entity& lhs, const Entity& rhs) { return lhs.m_EntityID == rhs.m_EntityID; }
		friend inline bool operator!=(const Entity& lhs, const Entity& rhs) { return !(lhs == rhs); }
		friend inline std::ostream& operator<<(std::ostream& os, const Entity& entity) {
			return os << "Entity(" << (std::uint32_t)entity.m_EntityID << ")";
		}

	private:
		entt::entity m_EntityID = entt::null;
		Scene* m_ContextScene = nullptr;
	};

}