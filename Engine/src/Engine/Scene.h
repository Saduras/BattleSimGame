#pragma once

#include <entt/entt.hpp>

#include <vector>

namespace Engine
{
	class Entity;
	class System;

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
		/// Destroys given entity and removes it from the scene.
		/// </summary>
		/// <param name="entity"></param>
		void DestroyEntity(const Entity& entity);

		/// <summary>
		/// Adds a system to the scene.
		/// </summary>
		/// <typeparam name="System"></typeparam>
		template<typename TSystem>
		void AddSystem() {
			m_Systems.push_back(new TSystem(this));
		}

		/// <summary>
		/// Executes all system in the scene once.
		/// </summary>
		/// <param name="deltaTime">Time since last update</param>
		void Update(float deltaTime);

		/// <summary>
		/// Constructs and returns a vector with the names of all systems in this scene.
		/// </summary>
		/// <returns>Vector of system names</returns>
		std::vector<std::string> GetSystemNames() const;

		/// <summary>
		/// Creates a view on all entities with a given set of components in the scene.
		/// </summary>
		/// <typeparam name="...Component">Set of component types to filter by</typeparam>
		/// <returns>A view (not copy) of the entities with these components in the scene.</returns>
		template<typename... Component>
		entt::basic_view<entt::entity, entt::exclude_t<>, Component...> GetView() {
			return m_Registry.view<Component...>();
		}

		/// <summary>
		/// Sorts entities according to a given compare operation. The compare operation should return true if the left hand side is "less" than the right hand side.
		/// </summary>
		/// <typeparam name="Component">Component type to compare</typeparam>
		/// <typeparam name="Compare"></typeparam>
		/// <param name="compare">Compare operators with signature: bool compare(const Component& lhs, const Component& rhs);</param>
		template<typename Component, typename Compare>
		void Sort(Compare compare)
		{
			return m_Registry.sort<Component>(compare);
		}

		/// <summary>
		/// Executes a system function on the view of all entities with the specified components.
		/// </summary>
		/// <typeparam name="...Component">Components each entity must have.</typeparam>
		/// <param name="deltaTime">Time since last update in seconds</param>
		/// <param name="system">Function to be executed for each entity</param>
		template<typename... Component>
		void ExecuteSystem(float deltaTime, void (*system)(float, Entity, Component&...))
		{
			auto view = GetView<Component...>();
			view.each([this, deltaTime, system](entt::entity entity, Component&... components) {
				system(deltaTime, Entity(entity, this), components...);
			});
		}

		/// <summary>
		/// Executes a system function on the view of all entities with the specified components.
		/// </summary>
		/// <typeparam name="...Component">Components each entity must have.</typeparam>
		/// <param name="deltaTime">Time since last update in seconds</param>
		/// <param name="system">Function to be executed for each entity</param>
		template<typename... Component>
		void ExecuteSystem(float deltaTime, void (*system)(float, Scene*, Entity, Component&...))
		{
			auto view = GetView<Component...>();
			view.each([this, deltaTime, system](entt::entity entity, Component&... components) {
				system(deltaTime, this, Entity(entity, this), components...);
				});
		}

		/// <summary>
		/// Returns count of (alive) entities in this scene.
		/// </summary>
		/// <returns></returns>
		size_t GetEntityCount() const;

	private:
		entt::registry m_Registry;
		std::vector<System*> m_Systems;

		friend class Entity;
	};
}
