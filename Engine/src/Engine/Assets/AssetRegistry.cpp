#include "epch.h"
#include "AssetRegistry.h"

namespace Engine
{
	AssetRegistry* AssetRegistry::s_Instance = new AssetRegistry();

	void AssetRegistry::Add(const std::string& id, Asset* asset)
	{
		s_Instance->m_AssetMap.emplace(id, asset);
	}

	void AssetRegistry::Delete(const std::string& id)
	{
		s_Instance->m_AssetMap.erase(id);
	}
}