#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Scene.h"
#include "Engine/ECS/Entity.h"
#include "Engine/ECS/System.h"
#include "Engine/ECS/Components/Transform.h"

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

			scene->Update(0.1f);
			scene->Update(0.1f);
			scene->Update(0.1f);

			Assert::AreEqual(s_SystemExecuteCount, 3);
		}
	};
}
