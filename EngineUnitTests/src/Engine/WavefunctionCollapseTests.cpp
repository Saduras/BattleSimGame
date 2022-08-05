#include "pch.h"
#include "CppUnitTest.h"
#include "AssertUtils.h"

#include "Engine/WavefunctionCollapse/WavefunctionCollapse.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Engine;

namespace WavefunctionCollapseTests
{
	TEST_CLASS(WavefunctionCollapseTests)
	{
	public:
		TEST_METHOD(PropagateCellTest)
		{
			std::vector<int> source = { 0 };
			std::vector<int> target = { 0, 1, 2, 3 };
			WFC::RuleSet rules = {
				{0, { 0, 1 } },
				{1, { 0 } },
				{2, {} },
				{3, {} }
			};

			WFC::PropagateCell(source, target, rules);

			Assert::AreEqual(target, { {0, 1} });
		}

		TEST_METHOD(UpdateAdjecentCellsTest)
		{
			Grid<std::vector<int>> grid(2, 2);
			grid.Get(0, 0) = { 0 };
			grid.Get(1, 0) = { 0, 1, 2 };
			grid.Get(0, 1) = { 1, 2 };
			grid.Get(1, 1) = { 0, 1, 2, 3 };

			WFC::RuleSet rules = {
				{0, { 0, 1 } },
				{1, { 0 } },
				{2, {} },
				{3, {} }
			};

			WFC::UpdateResult result = WFC::UpdateAdjecentCells(grid, 0, 0, rules);

			Assert::AreEqual(grid.Get(0, 0), { 0 });
			Assert::AreEqual(grid.Get(1, 0), { 0, 1 });
			Assert::AreEqual(grid.Get(0, 1), { 1 });
			Assert::AreEqual(grid.Get(1, 1), { 0, 1, 2 ,3 });
			Assert::AreEqual(result.Collapsed, 1);
		}

		TEST_METHOD(UpdateAdjecentCellsErrorTest)
		{
			Grid<std::vector<int>> grid(2, 2);
			grid.Get(0, 0) = { 0 };
			grid.Get(1, 0) = { 2 };
			grid.Get(0, 1) = { 1, 2 };
			grid.Get(1, 1) = { 0, 1, 2, 3 };

			WFC::RuleSet rules = {
				{0, { 0, 1 } },
				{1, { 0 } },
				{2, {} },
				{3, {} }
			};

			WFC::UpdateResult result = WFC::UpdateAdjecentCells(grid, 0, 0, rules);

			Assert::IsTrue(result.Error);
		}

		TEST_METHOD(CollapseGridTest)
		{
			Grid<std::vector<int>> grid(2, 2, { 0, 1, 2 });

			WFC::RuleSet rules = {
				{0, { 0, 1 } },
				{1, { 0, 1 } },
				{2, { } }
			};

			WFC::CollapseGrid(grid, rules);

			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					Assert::AreEqual((int)grid.Get(x, y).size(), 1, L"Expected each cell to have only one value left!");
					Assert::AreNotEqual(grid.Get(x, y)[0], 2);
				}
			}

		}
	};
}
