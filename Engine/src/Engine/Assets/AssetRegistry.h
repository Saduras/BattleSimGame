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

		static Material& GetMaterial(const std::string& id)
		{
			auto asset = s_Instance->m_AssetMap.at(id);
			auto mat = static_cast<Material*>(asset);
			return *mat;
		}

		static Mesh& GetMesh(const std::string& id)
		{
			auto asset = s_Instance->m_AssetMap.at(id);
			auto mesh = static_cast<Mesh*>(asset);
			return *mesh;
		}

		//template<typename T>
		//static T& Get(const std::string& id)
		//{
		//	auto asset = s_Instance->m_AssetMap.at(id);
		//	Asset& ref = *asset;
		//	T& refT = (T&)ref;
		//	return refT;
		//}

	private:
		AssetRegistry() {}
		static AssetRegistry* s_Instance;

		std::unordered_map<std::string, Asset*> m_AssetMap;
	};
}