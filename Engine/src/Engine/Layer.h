#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"
#include "Engine/ECS/Entity.h"

namespace Engine
{
	class ENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		Entity* CreateEntity();
		void DeleteEntity(Entity* entity);

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
		std::vector<Entity*> m_Entities;
	};
}
