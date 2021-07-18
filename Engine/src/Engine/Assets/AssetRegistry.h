#pragma once

#include <unordered_map>

#include "Asset.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

namespace Engine
{
	class AssetRegistry
	{
	public:
		static void Add(const std::string& id, Asset* asset);
		static void Delete(const std::string& id);

		template<class T>
		static T& Get(const std::string& id)
		{
			auto asset = s_Instance->m_AssetMap.at(id);
			T* typed_pointer = static_cast<T*>(asset);
			return *typed_pointer;
		}

	private:
		AssetRegistry() {}
		static AssetRegistry* s_Instance;

		std::unordered_map<std::string, Asset*> m_AssetMap;
	};
}