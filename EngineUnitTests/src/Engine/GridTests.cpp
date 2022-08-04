#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Grid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace GridTests
{
	static int s_SystemExecuteCount = 0;

	TEST_CLASS(GridTests)
	{
	public:
		TEST_METHOD(GetSizeTest)
		{
			Grid<int> grid(4, 5);

			Assert::AreEqual<size_t>(grid.GetWidth(), 4);
			Assert::AreEqual<size_t>(grid.GetHeight(), 5);
		}

		TEST_METHOD(SetCellTest)
		{
			Grid<int> grid(4, 5);

			grid(0, 0) = 3;
			grid(3, 4) = 2;

			Assert::AreEqual(grid(0, 0), 3);
			Assert::AreEqual(grid(3, 4), 2);
		}

		TEST_METHOD(InvalidIndexTest)
		{
			Grid<int> grid(4, 5);

			Assert::ExpectException<std::logic_error>([&grid] {
				grid(4, 5);
			});
		}
	};
}
