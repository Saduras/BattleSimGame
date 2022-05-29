#include "pch.h"
#include "CppUnitTest.h"

#include "Engine/Log.h"
#include "Engine/Scene.h"
#include "Engine/ECS/Entity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace EntityTests
{
	struct TestComponent {
		TestComponent() {}
		TestComponent(int data) : m_Data(data) {}

		int m_Data;
	};

	TEST_CLASS(EntityTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Setup)
		{
			if(!Log::IsInitialized())
				Log::Init();
		}

		TEST_METHOD(AddComponentTest)
		{
			Scene scene;
			Entity entity = scene.CreateEntity();

			entity.AddComponent<TestComponent>();

			Assert::IsTrue(entity.HasComponent<TestComponent>());

			// Already has this component
			Assert::ExpectException<std::logic_error>([&entity]() {
				entity.AddComponent<TestComponent>();
			});
		}

		TEST_METHOD(RemoveComponentTest)
		{
			Scene scene;
			Entity entity = scene.CreateEntity();

			entity.AddComponent<TestComponent>();
			entity.RemoveComponent<TestComponent>();

			Assert::IsFalse(entity.HasComponent<TestComponent>());

			// Doesn't have to component
			Assert::ExpectException<std::logic_error>([&entity]() {
				entity.RemoveComponent<TestComponent>();
			});
		}

		TEST_METHOD(GetComponentTest)
		{
			Scene scene;
			Entity entity = scene.CreateEntity();

			// Doesn't have the component
			Assert::ExpectException<std::logic_error>([&entity]() {
				entity.GetComponent<TestComponent>();
			});															   

			entity.AddComponent<TestComponent>(42);
			TestComponent& component = entity.GetComponent<TestComponent>();
			Assert::AreEqual(component.m_Data, 42);
		}

		TEST_METHOD(IsNullTest)
		{
			Scene scene;
			Entity entity = { entt::null, &scene };

			Assert::IsTrue(entity.IsNull());

			entity = { (entt::entity)1, nullptr };

			Assert::IsTrue(entity.IsNull());

			entity = { (entt::entity)1, &scene };

			Assert::IsFalse(entity.IsNull());
		}

		TEST_METHOD(IsValidTest)
		{
			Scene scene;
			Entity entity = { entt::null, &scene };

			Assert::IsFalse(entity.IsValid());

			entity = { (entt::entity)1, nullptr };

			Assert::IsFalse(entity.IsValid());

			entity = scene.CreateEntity();

			Assert::IsTrue(entity.IsValid());
		}

		TEST_METHOD(DestoryTest)
		{
			Scene scene;
			Entity entity = scene.CreateEntity();

			Assert::IsTrue(entity.IsValid());

			entity.Destroy();

			Assert::IsFalse(entity.IsValid());
		}
	};
}
