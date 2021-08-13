#pragma once

#include <entt/entt.hpp>

#include <vector>

namespace Engine
{
	class Entity;

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
		Entity CreateEntity();

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

		friend class Entity;
	};
}
