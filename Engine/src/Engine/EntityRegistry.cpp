#include "epch.h"
#include "EntityRegistry.h"


namespace Engine {
	EntityRegistry* EntityRegistry::s_Instance = new EntityRegistry();

	entt::entity EntityRegistry::Create()
	{
		return s_Instance->m_Registry.create();
	}
}