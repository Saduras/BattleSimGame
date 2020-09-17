#pragma once

#include <unordered_map>

#include "Asset.h"

namespace Engine
{
	class AssetRegistry
	{
	public:
		static void Add(const std::string& id, Asset asset);
		static void Delete(const std::string& id);

		template<typename T>
		static T& Get(const std::string& id)
		{
			return (T&)*s_Instance->m_AssetMap[id];
		}

	private:
		AssetRegistry() {}
		static AssetRegistry* s_Instance;

		std::unordered_map<std::string, std::unique_ptr<Asset>> m_AssetMap;


	};
}