#include "epch.h"
#include "Entity.h"

namespace Engine {
	Entity::Entity(entt::entity entityID, Scene* contextScene)
		: m_EntityID(entityID), m_ContextScene(contextScene) {}
}