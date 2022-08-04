#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/Grid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace GridTests
{
	TEST_CLASS(GridTests)
	{
	public:
		TEST_METHOD(GetSizeTest)
		{
			Grid<int> grid(4, 5);

			Assert::AreEqual(grid.GetWidth(), 4);
			Assert::AreEqual(grid.GetHeight(), 5);
		}

		TEST_METHOD(SetCellTest)
		{
			Grid<int> grid(4, 5);

			grid.Get(0, 0) = 3;
			grid.Get(3, 4) = 2;

			Assert::AreEqual(grid.Get(0, 0), 3);
			Assert::AreEqual(grid.Get(3, 4), 2);
		}

		TEST_METHOD(WideGridTest)
		{
			Grid<int> grid(10, 1);

			grid.Get(9, 0) = 4;

			Assert::AreEqual(grid.Get(9, 0), 4);
		}

		TEST_METHOD(HeighGridTest)
		{
			Grid<int> grid(1, 10);

			grid.Get(0, 9) = 4;

			Assert::AreEqual(grid.Get(0, 9), 4);
		}

		TEST_METHOD(InvalidIndexTest)
		{
			Grid<int> grid(4, 5);

			Assert::ExpectException<std::logic_error>([&grid] {
				grid.Get(4, 5);
			});
		}
	};
}
