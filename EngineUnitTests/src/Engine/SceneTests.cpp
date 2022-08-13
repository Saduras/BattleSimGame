#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Scene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SceneTests
{
	static int s_SystemExecuteCount = 0;

	TEST_CLASS(SceneTests)
	{
		class TestSystem : public Engine::System
		{
		public:
			TestSystem(Engine::Scene* scene) : Engine::System(scene) {}
			
			void Execute(float deltaTime) override { s_SystemExecuteCount++; }
			std::string GetName() const { return "TestSystem"; }
		};

		struct TestComponent
		{
			bool IsValid = false;
		};

		static void RunTestSystem(float deltaTime, Engine::Entity entity, TestComponent& component)
		{
			s_SystemExecuteCount++;
		}

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

			Assert::IsFalse(entity.IsNull());
			Assert::IsTrue(scene->GetEntityCount() == 1);
		}

		TEST_METHOD(DestroyEntityTest)
		{
			Engine::Scene* scene = new Engine::Scene();
			Engine::Entity entity = scene->CreateEntity();
			scene->DestroyEntity(entity);

			Assert::IsTrue(scene->GetEntityCount() == 0);
		}

		TEST_METHOD(AddSystemTest)
		{
			Engine::Scene* scene = new Engine::Scene();
			scene->AddSystem<TestSystem>();

			std::vector<std::string> systemNames = scene->GetSystemNames();

			Assert::AreEqual(systemNames.size(), (size_t)1);
			Assert::AreEqual(systemNames[0].c_str(), "TestSystem");
		}

		TEST_METHOD(ExecuteSystemsTest)
		{
			Engine::Scene* scene = new Engine::Scene();
			scene->AddSystem<TestSystem>();
			s_SystemExecuteCount = 0;

			scene->Update(0.1f);
			scene->Update(0.1f);
			scene->Update(0.1f);

			Assert::AreEqual(s_SystemExecuteCount, 3);
		}

		TEST_METHOD(ExecuteSystemsFucntionTest)
		{
			Engine::Scene* scene = new Engine::Scene();
			s_SystemExecuteCount = 0;
			Engine::Entity e1 = scene->CreateEntity();
			Engine::Entity e2 = scene->CreateEntity();
			e1.AddComponent<TestComponent>(true);
			e2.AddComponent<TestComponent>(true);
			
			scene->ExecuteSystem<TestComponent>(0.2f, RunTestSystem);

			// RunTestSytem was executed once per entity with TestComponent component
			Assert::AreEqual(s_SystemExecuteCount, 2);
		}
	};
}
