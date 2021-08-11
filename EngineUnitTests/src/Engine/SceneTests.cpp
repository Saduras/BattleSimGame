#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Scene.h"
#include "Engine/Components/Transform.h"

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
			entt::entity entity = scene->CreateEntity();
			
			Assert::IsTrue(entity != entt::null, L"Newly created entity is null!");
		}

		TEST_METHOD(EmplaceComponentTest)
		{
			using namespace Engine::Components;

			Engine::Scene* scene = new Engine::Scene();
			entt::entity entity = scene->CreateEntity();

			glm::vec3 position(1.0f, 2.0f, 3.0f);
			glm::vec3 rotation(3.0f, 2.0f, 1.0f);
			glm::vec3 scale(1.0f, 1.0f, 1.0f);

			scene->EmplaceComponent<Transform>(entity, position, rotation, scale);

			Transform& transform = scene->GetComponent<Transform>(entity);

			AssertUtils::glmAreEqual(transform.Position, position);
			AssertUtils::glmAreEqual(transform.Rotation, rotation);
			AssertUtils::glmAreEqual(transform.Scale, scale);
		}

		TEST_METHOD(GetViewTest)
		{
			using namespace Engine::Components;

			Engine::Scene* scene = new Engine::Scene();
			entt::entity entity = scene->CreateEntity();

			glm::vec3 position(1.0f, 2.0f, 3.0f);
			glm::vec3 rotation(3.0f, 2.0f, 1.0f);
			glm::vec3 scale(1.0f, 1.0f, 1.0f);

			scene->EmplaceComponent<Transform>(entity, position, rotation, scale);

			entt::basic_view<entt::entity, entt::exclude_t<>, Transform> view = scene->GetView<Transform>();

			Assert::AreEqual(view.size(), (size_t)1, L"View size doesn't match the number of entities added!");
		}
	};
}
