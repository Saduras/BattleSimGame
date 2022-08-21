#include "epch.h"
#include "AssetRegistry.h"

namespace Engine
{
	AssetRegistry* AssetRegistry::s_Instance = new AssetRegistry();

	UUID AssetRegistry::Add(Asset* asset)
	{
		s_Instance->m_AssetMap.emplace(asset->GetUUID(), asset);
		return asset->GetUUID();
	}

	void AssetRegistry::Delete(const UUID& id)
	{
		s_Instance->m_AssetMap.erase(id);
	}
}