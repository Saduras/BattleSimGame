#pragma once

#include "Engine/Core.h"
#include "Engine/Grid.h"

#include <unordered_map>
#include <set>
#include <vector>

namespace Engine::WFC
{
	using RuleSet = std::unordered_map<int, std::set<int>>;

	struct UpdateResult
	{
		/// <summary>
		/// Number of cells collapsed in this update.
		/// </summary>
		int Collapsed = 0;

		/// <summary>
		/// True if a cell became empty/invalid during this update.
		/// </summary>
		bool Error = false;
	};

	/// <summary>
	/// Iteratively collapsed grid until only one option is left per cell. Chosed random entry if next
	/// cell has multiple options left.
	/// </summary>
	/// <param name="grid"></param>
	/// <param name="rules"></param>
	void CollapseGrid(Grid<std::vector<int>>& grid, const RuleSet& rules);
	
	/// <summary>
	/// Removes all options from the target cell which would violate rules for any option in the source cell.
	/// </summary>
	/// <param name="source">The cell to base all rule checks on.</param>
	/// <param name="target">The cell to be updated.</param>
	/// <param name="rules">Rule set to apply.</param>
	void PropagateCell(const std::vector<int>& source, std::vector<int>& target, const RuleSet& rules);

	/// <summary>
	/// Propagets changes to all adjecent cells.
	/// </summary>
	/// <param name="options">Full grid to act upon.</param>
	/// <param name="x">Column of the source cell.</param>
	/// <param name="y">Row of the source cell.</param>
	/// <param name="rules">Rule set to apply.</param>
	/// <returns>Number of cells collapsed by this update or error if a cell ended up empty.</returns>
	UpdateResult UpdateAdjecentCells(Grid<std::vector<int>>& grid, int x, int y, const RuleSet& rules);

}