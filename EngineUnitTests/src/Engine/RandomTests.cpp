#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Random.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RandomTests
{
	static int s_SystemExecuteCount = 0;

	TEST_CLASS(RandomTests)
	{
	public:
		TEST_METHOD(GetRandomFloatTest)
		{
			for (size_t i=0; i < 1000; i++)
			{
				float random = Engine::GetRandomFloat();
				Assert::IsTrue(random >= 0.0f && random <= 1.0f);
			}
		}

		TEST_METHOD(GetRandomIntTest)
		{
			for (size_t i = 0; i < 1000; i++)
			{
				int random = Engine::GetRandomInt(5, 32);
				Assert::IsTrue(random >= 5 && random <= 32);
			}
		}

		TEST_METHOD(GetRandomIndexTest)
		{
			for (size_t i = 0; i < 1000; i++)
			{
				int random = Engine::GetRandomIndex(17);
				Assert::IsTrue(random >= 0 && random < 17);
			}
		}
	};
}
