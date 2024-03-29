#pragma once

#include <unordered_map>

#include "Asset.h"

namespace Engine
{
	class AssetRegistry
	{
	public:
		static UUID Add(Asset* asset);
		static void Delete(const UUID& id);

		template<class T>
		static T& Get(const UUID& id)
		{
			if (s_Instance->m_AssetMap.find(id) == s_Instance->m_AssetMap.end())
				ENG_CORE_ERROR("No asset with id: {0}", id);

			auto asset = s_Instance->m_AssetMap[id];
			T* typed_pointer = static_cast<T*>(asset);
			return *typed_pointer;
		}

	private:
		AssetRegistry() {}
		static AssetRegistry* s_Instance;

		std::unordered_map<UUID, Asset*> m_AssetMap;
	};
}