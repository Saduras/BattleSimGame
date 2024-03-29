#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Random.h"

#include <vector>
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RandomTests
{
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

		TEST_METHOD(GetRandomEntryTest)
		{
			std::vector<int> collection = { 3, 5, 7, 11 };

			for (size_t i = 0; i < 1000; i++)
			{
				int random = Engine::GetRandomEntry(collection);
				Assert::IsTrue(std::find(collection.begin(), collection.end(), random) != collection.end());
			}
		}
	};
}
