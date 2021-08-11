#pragma once

#include <entt/entt.hpp>

namespace Engine
{
	/// <summary>
	/// A scene contains a collection of entities.
	/// </summary>
	class Scene
	{
	public:
		Scene();

		/// <summary>
		/// Add a new entity to the scene.
		/// </summary>
		/// <returns>The newly created entity</returns>
		entt::entity CreateEntity();

		/// <summary>
		/// Retrieves a component of an entity.
		/// </summary>
		/// <typeparam name="Component">Type of the component</typeparam>
		/// <param name="entity">Entity for which to get the component</param>
		/// <returns>Reference to the component</returns>
		template<typename Component>
		Component& GetComponent(const entt::entity entity) {
			return m_Registry.get<Component>(entity);
		}

		/// <summary>
		/// Constructs a component with the given arguments on the entity.
		/// </summary>
		/// <typeparam name="Component">Type of component to construct</typeparam>
		/// <param name="entity">The entity</param>
		/// <param name="...args">Arguments for the component constructor</param>
		template<typename Component, typename... Args>
		void EmplaceComponent(const entt::entity entity, Args &&... args) {
			m_Registry.emplace<Component>(entity, args...);
		}

		/// <summary>
		/// Creates a view on all entities with a given set of components in the scene.
		/// </summary>
		/// <typeparam name="...Component">Set of component types to filter by</typeparam>
		/// <returns>A view (not copy) of the entities with these components in the scene.</returns>
		template<typename... Component>
		entt::basic_view<entt::entity, entt::exclude_t<>, Component...> GetView() {
			return m_Registry.view<Component...>();
		}

	private:
		entt::registry m_Registry;
	};
}
