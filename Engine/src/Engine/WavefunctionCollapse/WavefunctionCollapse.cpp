#include "epch.h"
#include "WavefunctionCollapse.h"

namespace Engine::WFC {
	void PropagateCell(const std::vector<int>& source, std::vector<int>& target, const RuleSet& rules)
	{
		for (int i = target.size() - 1; i >= 0; i--)
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

	void UpdateAdjecentCells(Grid<std::vector<int>>& options, int x, int y, const RuleSet& rules)
	{
		if (x > 0)
			PropagateCell(options(x, y), options(x - 1, y), rules);
		if (x < options.GetWidth() - 1)
			PropagateCell(options(x, y), options(x + 1, y), rules);
		if (y > 0)
			PropagateCell(options(x, y), options(x, y - 1), rules);
		if (y < options.GetHeight() - 1)
			PropagateCell(options(x, y), options(x, y + 1), rules);
	}
}