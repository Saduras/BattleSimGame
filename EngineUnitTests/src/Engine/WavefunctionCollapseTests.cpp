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
			grid(0, 0) = { 0 };
			grid(1, 0) = { 0, 1, 2 };
			grid(0, 1) = { 1, 2 };
			grid(1, 1) = { 0, 1, 2, 3 };

			WFC::RuleSet rules = {
				{0, { 0, 1 } },
				{1, { 0 } },
				{2, {} },
				{3, {} }
			};

			WFC::UpdateAdjecentCells(grid, 0, 0, rules);

			Assert::AreEqual(grid(0, 0), { 0 });
			Assert::AreEqual(grid(1, 0), { 0, 1 });
			Assert::AreEqual(grid(0, 1), { 1 });
			Assert::AreEqual(grid(1, 1), { 0, 1, 2 ,3 });
		}
	};
}
