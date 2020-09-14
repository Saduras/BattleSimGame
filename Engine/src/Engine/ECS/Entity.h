#pragma once

#include <unordered_map>
#include <type_traits>
#include <typeindex>

namespace Engine
{
	class Component;

	class Entity
	{
	public:
		Entity() {}
		~Entity() 
		{
			// delete all components
			for (const auto& pair : m_Components) {
				delete &pair.second;
			}
		}

		template<typename T>
		void AddComponent(T component)
		{
			using type = std::remove_pointer_t<T>;
			ENG_CORE_ASSERT((std::is_base_of<Component, type>::value), "Component to add must inherit from Component!");
			Component* comp = (Component*)component;
			comp->SetEntity(this);
			m_Components.insert({ typeid(T), comp });
		}

		template<typename T>
		void RemoveComponent(T component)
		{
			m_Components.erase(typeid(T));
		}

		template<typename T>
		bool HasComponent(std::type_index typeIndex)
		{
			return m_Components.find(typeIndex) != m_Components.end();
		}

		template<typename T>
		T GetComponent(std::type_index typeIndex)
		{
			return (T)m_Components.at(typeIndex);
		}
	private:
		std::unordered_map<std::type_index, Component*> m_Components;
	};
}
