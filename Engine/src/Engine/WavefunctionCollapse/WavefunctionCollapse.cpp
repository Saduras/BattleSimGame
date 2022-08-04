#include "epch.h"
#include "WavefunctionCollapse.h"

namespace Engine::WFC {
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

	int UpdateAdjecentCells(Grid<std::vector<int>>& options, int x, int y, const RuleSet& rules)
	{
		static GridPoint neighbours[4] = { {x - 1, y}, {x + 1, y}, {x, y - 1}, {x, y + 1} };
		
		int collapsed = 0;
		bool error = false;

		for (GridPoint neighbour : neighbours)
		{
			// Check if neighbour is inside grid bounds.
			if (neighbour.x >= 0 && neighbour.x < options.GetWidth()
				&& neighbour.y >= 0 && neighbour.y < options.GetHeight())
			{
				int size_before = (int)options.Get(neighbour).size();
				PropagateCell(options.Get(x, y), options.Get(neighbour), rules);
				if (size_before != 1 && (int)options.Get(neighbour).size() == 1)
					collapsed++;
				error |= options.Get(neighbour).size() == 0;
			}
		}

		return collapsed;
	}
}