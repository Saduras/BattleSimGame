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
		int data;
	};

	TEST_CLASS(EntityTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Setup)
		{
			Log::Init();
		}

		TEST_METHOD(AddComponentTest)
		{
			Scene scene;
			Entity entity = scene.CreateEntity();

			entity.AddComponent<TestComponent>();

			Assert::IsTrue(entity.HasComponent<TestComponent>());

			Assert::ExpectException<std::logic_error>([&entity]() {
				entity.AddComponent<TestComponent>();
			});
		}
	};
}
