#pragma once

#include <entt/entt.hpp>

/**
 * This hold all entities for now and provides a thin layer to entt.
 * In the future the entity registry should be owned by the scene maybe...
 */
namespace Engine {
	class EntityRegistry
	{
	public:
		template<typename... Component>
		static entt::basic_view<entt::entity, entt::exclude_t<>, Component...> View() {
			return s_Instance->m_Registry.view<Component...>();
		}

		static entt::entity Create();

		template<typename Component>
		static Component& Get(const entt::entity entity) {
			return s_Instance->m_Registry.get<Component>(entity);
		}

		template<typename Component, typename... Args>
		static void Emplace(const entt::entity entity, Args &&... args) {
			s_Instance->m_Registry.emplace<Component>(entity, args...);
		}

	private:
		EntityRegistry() {};
		static EntityRegistry* s_Instance;

		entt::registry m_Registry;
	};
}

