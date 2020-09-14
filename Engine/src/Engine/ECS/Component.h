#pragma once

#include "Engine/ECS/Entity.h"

namespace Engine
{
	class Component
	{
	public:		
		Component()
			: m_Entity(nullptr) 
		{}
		~Component() {}

		inline virtual Entity* GetEntity() const { return m_Entity; }
	private:
		inline void SetEntity(Entity* entity) { m_Entity = entity; }

		Entity* m_Entity;
		friend class Entity;
	};
}