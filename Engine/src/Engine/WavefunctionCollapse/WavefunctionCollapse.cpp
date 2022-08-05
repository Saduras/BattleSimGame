#include "epch.h"
#include "WavefunctionCollapse.h"

#include "Engine/Random.h"

#include <spdlog/fmt/bundled/core.h>
#include <spdlog/fmt/bundled/format.h>

#include <climits>
#include <vector>

namespace Engine::WFC {
	// For debugging
	static void PrintGrid(const Grid<std::vector<int>>& grid)
	{
		int cell_value_count = 5;
		std::stringstream ss;

		ss << "Grid:\n";
		for (int i = 0; i < grid.GetWidth() * (3 * cell_value_count + 2); i++)
			ss << "-";
		ss << "\n";

		for (int y = 0; y < grid.GetHeight(); y++)
		{
			for (int x = 0; x < grid.GetWidth(); x++)
			{
				std::vector<int> cell = grid.Get(x, y);
				for (int i = 0; i < cell_value_count; i++)
				{
					if (i < cell.size())
						ss << fmt::format("{}, ", cell[i]);
					else
						ss << "   ";
				}
				ss << "| ";
			}
			ss << "\n";
			for (int i = 0; i < grid.GetWidth() * (3 * cell_value_count + 2); i++)
				ss << "-";
			ss << "\n";
		}
		ENG_CORE_INFO(ss.str());
	}


	void CollapseGrid(Grid<std::vector<int>>& grid, const RuleSet& rules)
	{
		Engine::Grid<std::vector<int>> backup(grid);
		int cell_count = grid.GetWidth() * grid.GetHeight();

		// Count already collapsed cells
		int collapsed = 0;
		for (int x = 0; x < grid.GetWidth(); x += 1)
			for (int y = 0; y < grid.GetHeight(); y += 1)
				if (grid.Get(x, y).size() == 1)
					collapsed++;

		while (collapsed < cell_count)
		{
			Engine::GridPoint lowest_entropy_pt = { -1, -1 };
			int lowest_entropy = INT_MAX;
			for (int x = 0; x < grid.GetWidth(); x++)
			{
				for (int y = 0; y < grid.GetHeight(); y++)
				{
					int size = (int)grid.Get(x, y).size();
					if (size != 1 && size < lowest_entropy)
					{
						lowest_entropy = size;
						lowest_entropy_pt = { x, y };
					}
				}
			}

			ENG_CORE_ASSERT(lowest_entropy_pt.x != -1, "Failed to find a point to collapse!");

			backup = grid;

			// Collapse cell
			std::vector<int>& cell = grid.Get(lowest_entropy_pt);
			int index = Engine::GetRandomIndex(cell.size());
			cell = { cell[index] };
			UpdateResult result = Engine::WFC::UpdateAdjecentCells(grid, lowest_entropy_pt.x, lowest_entropy_pt.y, rules);

			if (result.Error)
			{
				// Undo change
				grid = backup;
				// Remove tested value
				std::vector<int>& restored_cell = grid.Get(lowest_entropy_pt);
				restored_cell.erase(restored_cell.begin() + index);

				if (restored_cell.size() == 1)
					collapsed += 1;
			}
			else
				collapsed += 1 + result.Collapsed;
		}
	}

	void PropagateCell(const std::vector<int>& source, std::vector<int>& target, const RuleSet& rules)
	{
		for (int i = (int)target.size() - 1; i >= 0; i--)
		{
			const int target_option = target[i];
			bool is_valid = false;
			for (int source_option : source)
			{
				if (rules.at(target_option).find(source_option) != rules.at(target_option).end())
				{
					is_valid = true;
					break;
				}
			}

			if (!is_valid)
				target.erase(target.begin() + i);
		}
	}

	UpdateResult UpdateAdjecentCells(Grid<std::vector<int>>& grid, int x, int y, const RuleSet& rules)
	{
		UpdateResult result;

		const GridPoint neighbours[4] = { {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1} };
		for (GridPoint neighbour : neighbours)
		{
			// Check if neighbour is inside grid bounds.
			if (neighbour.x >= 0 && neighbour.x < grid.GetWidth()
				&& neighbour.y >= 0 && neighbour.y < grid.GetHeight())
			{
				int size_before = (int)grid.Get(neighbour).size();
				PropagateCell(grid.Get(x, y), grid.Get(neighbour), rules);
				if (size_before != 1 && (int)grid.Get(neighbour).size() == 1)
					result.Collapsed++;
				if (grid.Get(neighbour).size() == 0)
				{
					result.Error = true;
					result.Collapsed = 0;
					break;
				}
			}
		}

		return result;
	}
}