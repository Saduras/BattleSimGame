#include "epch.h"
#include "Layer.h"

#include <algorithm>

namespace Engine
{
	Layer::Layer(const std::string& debugName)
		: m_DebugName(debugName)
	{

	}

	Layer::~Layer() {}
	
	Entity* Layer::CreateEntity()
	{
		Entity* entity = new Entity();
		m_Entities.push_back(entity);
		return entity;
	}

	void Layer::DeleteEntity(Entity* entity)
	{
		std::remove(m_Entities.begin(), m_Entities.end(), entity);
	}
}