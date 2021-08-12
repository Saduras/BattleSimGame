#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Scene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/Components/Transform.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SceneTests
{
	TEST_CLASS(SceneTests)
	{
	public:
		TEST_METHOD(CreateSceneTest)
		{
			Engine::Scene* scene = new Engine::Scene();

			Assert::IsNotNull(scene);
		}

		TEST_METHOD(CreateEntityTest)
		{
			Engine::Scene* scene = new Engine::Scene();
			Engine::Entity entity = scene->CreateEntity();
		}
	};
}
