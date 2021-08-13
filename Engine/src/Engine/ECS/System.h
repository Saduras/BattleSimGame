#pragma once

#include "Engine/Scene.h"

#include <string>

namespace Engine {

	class System {
	public:
		System(Scene* scene) : m_Scene(scene) {}

		virtual void Execute(float deltaTime) = 0;
		virtual std::string GetName() const = 0;

	protected:
		Scene* m_Scene;
	};
}